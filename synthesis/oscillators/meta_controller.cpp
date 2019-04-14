/*
 * meta_controller.cpp
 *
 *  Created on: Sep 17, 2018
 *      Author: willmitchell
 */

#include "oscillators.hpp"

/// defined for linkage
constexpr uint32_t ExpoConverter::expoTable[];

/*
 *
 * Meta oscillator controller
 *
 */

void MetaController::parseControlsExternal(ViaControls * controls, ViaInputStreams * inputs) {
	(this->*parseControls)(controls, inputs);
}


void MetaController::parseControlsAudio(ViaControls * controls, ViaInputStreams * inputs) {
	// time1 is coarse, time2 is fine, release time = attack time

	timeBase1 = fix16_mul(fix16_mul(fix16_mul(expo.convert((controls->knob1Value >> 2)*3) >> 5, // 2 << 11
			65535 + (controls->knob2Value << 4)), // 2 << 16
			expo.convert(__USAT((int32_t)controls->cv1Value - cv1Offset, 12))), // expoTable[2048]
			audioBaseIncrement);
	timeBase2 = timeBase1;

	dutyCycleBase = 32767;

}

void MetaController::parseControlsDrum(ViaControls * controls, ViaInputStreams * inputs) {
	// time1 is coarse, time2 is not used, release time = attack time

	timeBase1 = fix16_mul(fix16_mul(expo.convert(((controls->knob1Value >> 3)*3) + 1024) >> 5,
			expo.convert(__USAT((int32_t)controls->cv1Value - cv1Offset, 12))) >> 4,
			drumBaseIncrement);

	timeBase2 = timeBase1;

	dutyCycleBase = 32767;

}

void MetaController::parseControlsEnv(ViaControls * controls, ViaInputStreams * inputs) {
	// t1 is attack time (attackIncrements), t2 is release time (releaseIncrements)

	int32_t releaseMod = inputs->cv2Samples[0];
	releaseMod += 32767 - cv2Offset;

	releaseMod = __USAT(releaseMod >> 4, 12);

	timeBase1 = fix16_mul(expo.convert(4095 - controls->knob1Value) >> 8,
			expo.convert((4095 - __USAT((int32_t)controls->cv1Value - cv1Offset, 12))) >> 7);
	timeBase2 = fix16_mul(expo.convert(4095 - controls->knob2Value) >> 9,
			expo.convert(releaseMod) >> 8);

	dutyCycleBase = 32767;

}

void MetaController::parseControlsSeq(ViaControls * controls, ViaInputStreams * inputs) {
	// t1 is cycle time, t2 is used to feed the duty cycle input for getSamples

	timeBase1 = fix16_mul(expo.convert(4095 - controls->knob1Value) >> 9,
			expo.convert((4095 - __USAT(controls->cv1Value - cv1Offset, 12)) >> 1) >> 9);
	timeBase2 = timeBase1;

	dutyCycleBase = controls->knob2Value << 4;

}

void MetaController::generateIncrementsExternal(ViaInputStreams * inputs) {
	(this->*generateIncrements)(inputs);
}

// fill incrementValues 1 and 2 with the attack and release time increments, respectively

void MetaController::generateIncrementsAudio(ViaInputStreams * inputs) {

	int32_t localFm = (int32_t) -fm[0];
	localFm += 16384 + cv2Offset;
	increment1 = fix16_mul(timeBase1, localFm);
	increment2 = increment1;

	dutyCycle = dutyCycleBase;

}

void MetaController::generateIncrementsDrum(ViaInputStreams * inputs) {

	int32_t localFm = (int32_t) fm[0];
	localFm <<= 1;
	localFm += (1 << 15);
	int32_t localTimeBase = timeBase1 + fix16_mul(timeBase1, expoFM[0]);
	increment1 = fix16_mul(localTimeBase, localFm);
	increment2 = increment1;

	dutyCycle = dutyCycleBase;

}

void MetaController::generateIncrementsEnv(ViaInputStreams * inputs) {

	increment1 = timeBase1;
	increment2 = timeBase2;

	dutyCycle = dutyCycleBase;

}


void MetaController::generateIncrementsSeq(ViaInputStreams * inputs) {

	increment1 = timeBase1;
	increment2 = timeBase2;

	int32_t dutyCycleMod = -inputs->cv2Samples[0];

	dutyCycle = __USAT(dutyCycleBase + dutyCycleMod + cv2Offset, 16);

}

void MetaController::advancePhaseExternal(uint32_t * phaseDistTable) {
	(this->*advancePhase)(phaseDistTable);
}

int32_t MetaController::advancePhasePWM(uint32_t * phaseDistTable) {

	int32_t phaseWrapper;

	incrementUsed = (this->*incrementArbiter)() * oscillatorOn;

	int32_t increment = incrementUsed * freeze;

	int32_t localPhase = phase + !triggerSignal;

	localPhase = (localPhase + __SSAT(increment, 24)) * (oscillatorOn);

	phaseWrapper = 0;

	// add wavetable length if phase < 0

	phaseWrapper += ((uint32_t)(localPhase) >> 31) * WAVETABLE_LENGTH;

	// subtract wavetable length if phase > wavetable length

	phaseWrapper -= ((uint32_t)(WAVETABLE_LENGTH - localPhase) >> 31) * WAVETABLE_LENGTH;

	// apply the wrapping
	// no effect if the phase is in bounds

	localPhase += phaseWrapper;

	phase = localPhase;

	uint32_t pwm = dutyCycle;
	uint32_t pwmIndex = (pwm >> 11);
	uint32_t pwmFrac = (pwm & 0x7FF) << 4;
	// assuming that each phase distortion lookup table is 65 samples long stored as int32_t
	uint32_t * pwmTable1 = phaseDistTable + pwmIndex * 65;
	uint32_t * pwmTable2 = pwmTable1 + 65;
	uint32_t leftSample = localPhase >> 19;

#define pwmPhaseFrac (localPhase & 0x7FFFF) >> 4
		// use this with the precalculated pwm to perform bilinear interpolation
		// this accomplishes the
	localPhase = fix15_bilerp(pwmTable1[leftSample], pwmTable2[leftSample],
				pwmTable1[leftSample + 1], pwmTable2[leftSample + 1], pwmFrac,
				pwmPhaseFrac);

	// log a -1 if the max value index of the wavetable is traversed from the left
	// log a 1 if traversed from the right
	// do this by subtracting the sign bit of the last phase from the current phase, both less the max phase index
	// this adds cruft to the wrap indicators, but that is deterministic and can be parsed out

	int32_t atBIndicator = ((uint32_t)(localPhase - AT_B_PHASE) >> 31) - ((uint32_t)(ghostPhase - AT_B_PHASE) >> 31);

	phaseWrapper += atBIndicator;

	phaseEvent = phaseWrapper;

	(this->*loopHandler)();

	// store the current phases
	phaseBeforeIncrement = ghostPhase;

	ghostPhase = localPhase;

	return phase;

}

int32_t MetaController::advancePhaseOversampled(uint32_t * phaseDistTable) {

	int32_t phaseWrapper;

	incrementUsed = (this->*incrementArbiter)();

	int32_t increment = incrementUsed * freeze;

	int32_t localPhase = phase + !triggerSignal;

	localPhase = (localPhase + __SSAT(increment, 24)) * (oscillatorOn);

	phaseWrapper = 0;

	// add wavetable length if phase < 0

	phaseWrapper += ((uint32_t)(localPhase) >> 31) * WAVETABLE_LENGTH;

	// subtract wavetable length if phase > wavetable length

	phaseWrapper -= ((uint32_t)(WAVETABLE_LENGTH - localPhase) >> 31) * WAVETABLE_LENGTH;

	// apply the wrapping
	// no effect if the phase is in bounds

	localPhase += phaseWrapper;

	// log a -1 if the max value index of the wavetable is traversed from the left
	// log a 1 if traversed from the right
	// do this by subtracting the sign bit of the last phase from the current phase, both less the max phase index
	// this adds cruft to the wrap indicators, but that is deterministic and can be parsed out

	int32_t atBIndicator = ((uint32_t)(localPhase - AT_B_PHASE) >> 31) - ((uint32_t)(ghostPhase - AT_B_PHASE) >> 31);

	phaseWrapper += atBIndicator;

	phaseEvent = phaseWrapper;

	(this->*loopHandler)();

	phaseBeforeIncrement = phase;

	phase = localPhase;

	ghostPhase = localPhase;

	return phase;

}



void MetaController::handleLoopOn(void) {
	oscillatorOn = 1;
}

void MetaController::handleLoopOff(void) {

	oscillatorOn &= !(int32_abs(phaseEvent) >> 24);

	oscillatorOn |= !(triggerSignal);

}

/*
 *
 * Meta Controller Increment Arbiter
 * Based on the phase and the logic inputs, handle
 *
 */

int32_t MetaController::noRetrigAttackState(void) {

	if (phaseEvent == AT_B_FROM_ATTACK) {
		incrementArbiter = &MetaController::noRetrigReleaseState;
		return increment2;
	} else {
		return increment1;
	}

}

int32_t MetaController::noRetrigReleaseState(void) {

	if (phaseEvent == AT_A_FROM_RELEASE) {
		incrementArbiter = &MetaController::noRetrigAttackState;
		return increment1;
	} else {
		return increment2;
	};

}

/*
 *
 * hardSyncStateMachine
 *
 */

int32_t MetaController::hardSyncAttackState(void) {

	phase *= triggerSignal;

	if (phaseEvent == AT_B_FROM_ATTACK) {
		incrementArbiter = &MetaController::hardSyncReleaseState;
		return increment2;
	} else {
		return increment1;
	}

}

int32_t MetaController::hardSyncReleaseState(void) {

	phase *= triggerSignal;

	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::hardSyncAttackState;
	}

	if (phaseEvent == AT_A_FROM_RELEASE) {
		incrementArbiter = &MetaController::hardSyncAttackState;
		return increment1;
	} else {
		return increment2;
	};
}

/*
 *
 * envStateMachine
 *
 */

int32_t MetaController::envAttackState(void) {

	if (phaseEvent == AT_B_FROM_ATTACK) {
		incrementArbiter = &MetaController::envReleaseState;
		return increment2;
	} else {
		return increment1;
	}
}

int32_t MetaController::envReleaseState(void) {

	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::envRetriggerState;
		return -increment1;
		// if at a from attack or at a from release
	} else if (abs(phaseEvent) == AT_A_FROM_ATTACK) {
		incrementArbiter = &MetaController::envAttackState;
		return increment1;
	} else {
		return increment2;
	};
}

int32_t MetaController::envRetriggerState(void) {

	// if at b from attack or at b from release
	if (abs(phaseEvent) == AT_B_FROM_ATTACK) {

		incrementArbiter = &MetaController::envReleaseState;
		return increment2;

	// hack to catch when skew modulation skips over the B phase event ?

	} else if (phaseEvent == AT_A_FROM_ATTACK) {
		incrementArbiter = &MetaController::envAttackState;
		return increment1;

	} else {
		return -increment1;

	}
}

/*
 *
 * gateStateMachine loop
 *
 */

int32_t MetaController::gateAttackState(void) {

	int32_t gateWLoopProtection = gateSignal | loopMode;

	if (gateWLoopProtection == 0) {
		incrementArbiter = &MetaController::gateReleaseReverseState;
		return -increment2;
	} else if (phaseEvent == AT_B_FROM_ATTACK) {
		incrementArbiter = &MetaController::gatedState;
		return 0;
	} else {
		return increment1;
	}
}

int32_t MetaController::gateReleaseReverseState(void) {

	if (gateSignal) {
		incrementArbiter = &MetaController::gateAttackState;
		return increment1;
	} else if (phaseEvent == AT_A_FROM_ATTACK) {
		incrementArbiter = &MetaController::gateAttackState;
		return increment1;
	} else {
		return -increment2;
	};
}

int32_t MetaController::gatedState(void) {

	if (gateSignal == 0) {
		atB = 0;
		incrementArbiter = &MetaController::gateReleaseState;
		return increment2;
	} else {
		atB = 1;
		return 0;
	}

}

int32_t MetaController::gateReleaseState(void) {

	if (gateSignal) {
		incrementArbiter = &MetaController::gateRetriggerState;
		return -increment1;
	} else if (phaseEvent == AT_A_FROM_RELEASE) {
		incrementArbiter = &MetaController::gateAttackState;
		return increment1;
	} else {
		return increment2;
	};
}

int32_t MetaController::gateRetriggerState(void) {

	int32_t gateWLoopProtection = gateSignal | loopMode;

	if (gateWLoopProtection == 0) {
		incrementArbiter = &MetaController::gateReleaseState;
		return increment2;
	} else if (phaseEvent == AT_B_FROM_RELEASE) {
		incrementArbiter = &MetaController::gatedState;
		return 0;
	} else {
		return -increment1;
	}

}

/*
 *
 * pendulumStateMachine
 *
 */

int32_t MetaController::pendulumRestingState(void) {
	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::pendulumForwardAttackState;
		return increment1;
	} else {
		return 0;
	}
}

int32_t MetaController::pendulumForwardAttackState(void) {

	if (triggerSignal == 0 && oscillatorOn) {
		incrementArbiter = &MetaController::pendulumReverseAttackState;
		return 0;
	} else if (phaseEvent == AT_B_FROM_ATTACK) {
		incrementArbiter = &MetaController::pendulumForwardReleaseState;
		return increment1;
	} else {
		return increment1;
	}

}

int32_t MetaController::pendulumReverseAttackState(void) {


	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::pendulumForwardAttackState;
		return increment1;
	} else if (phaseEvent == AT_A_FROM_ATTACK) {
		incrementArbiter = &MetaController::pendulumReverseReleaseState;
		return -increment2;
	} else {
		return -increment1;
	}

}

int32_t MetaController::pendulumForwardReleaseState(void) {

	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::pendulumReverseReleaseState;
		return -increment2;
	} else if (phaseEvent == AT_A_FROM_RELEASE) {
		incrementArbiter = &MetaController::pendulumForwardAttackState;
		return increment1;
	} else {
		return increment2;
	}

}

int32_t MetaController::pendulumReverseReleaseState(void) {

	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::pendulumForwardReleaseState;
		return increment2;
	} else if (phaseEvent == AT_B_FROM_RELEASE) {
		incrementArbiter = &MetaController::pendulumReverseAttackState;
		return -increment1;
	} else {
		return -increment2;
	}

}

/*
 *
 * stickyPendulumStateMachine
 *
 */

int32_t MetaController::stickyPendulumRestingState(void) {
	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::stickyPendulumForwardAttackState;
		return increment1;
	} else {
		return 0;
	}
}

int32_t MetaController::stickyPendulumAtBState(void) {

	if (triggerSignal == 0) {
		atB = 0;
		incrementArbiter = &MetaController::stickyPendulumForwardReleaseState;
		return increment2;
	} else {
		atB = 1;
		return 0;
	}

}

int32_t MetaController::stickyPendulumForwardAttackState(void) {

	if (triggerSignal == 0 && oscillatorOn) {
		incrementArbiter = &MetaController::stickyPendulumReverseAttackState;
		return 0;
	} else if (phaseEvent == AT_B_FROM_ATTACK) {
		incrementArbiter = &MetaController::stickyPendulumAtBState;
		return 0;
	} else {
		return increment1;
	}

}

int32_t MetaController::stickyPendulumReverseAttackState(void) {

	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::stickyPendulumForwardAttackState;
		return increment1;
	} else if (phaseEvent == AT_A_FROM_ATTACK) {
		incrementArbiter = &MetaController::stickyPendulumRestingState;
		return 0;
	} else {
		return -increment1;
	}

}

int32_t MetaController::stickyPendulumForwardReleaseState(void) {

	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::stickyPendulumReverseReleaseState;
		return -increment2;
	} else if (phaseEvent == AT_A_FROM_RELEASE) {
		incrementArbiter = &MetaController::stickyPendulumRestingState;;
		return 0;
	} else {
		return increment2;
	}

}

int32_t MetaController::stickyPendulumReverseReleaseState(void) {

	if (triggerSignal == 0) {
		incrementArbiter = &MetaController::stickyPendulumForwardReleaseState;
		return increment2;
	} else if (phaseEvent == AT_B_FROM_RELEASE) {
		incrementArbiter = &MetaController::stickyPendulumAtBState;
		return 0;
	} else {
		return -increment2;
	}

}
