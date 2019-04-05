/*
 * simple_envelope.cpp
 *
 *  Created on: Sep 17, 2018
 *      Author: willmitchell
 */

#include "oscillators.hpp"

void SimpleEnvelope::parseControls(ViaControls * controls, ViaInputStreams * inputs) {

	int32_t releaseMod = inputs->cv2Samples[0];
	releaseMod += 32768;

	releaseMod = releaseMod >> 4;

	release = fix16_mul(expo.convert(((4095 - controls->knob2Value) >> 2) * 3) >> 11,
			expo.convert(((releaseMod) >> 2) * 3) >> 4);

//	release >>= 2;

}
void SimpleEnvelope::advance(ViaInputStreams * inputs,
		uint32_t * wavetable) {

	int32_t phaseWrapper;

	increment = (this->*incrementArbiter)();

	trigger = 1;

	int32_t localPhase = phase;

	localPhase = (localPhase + increment);

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

	int32_t atBIndicator = ((uint32_t)(localPhase - AT_B_PHASE) >> 31) - ((uint32_t)(previousPhase - AT_B_PHASE) >> 31);

	phaseWrapper += atBIndicator;

	phaseEvent = phaseWrapper;

	previousPhase = localPhase;

	phase = localPhase;

	uint32_t leftSample = localPhase >> 16;
#define phaseFrac (localPhase & 0xFFFF)
	output[0] = fast_15_16_lerp(wavetable[leftSample], wavetable[leftSample + 1], phaseFrac);

}

int32_t SimpleEnvelope::attackState(void) {

	switch (phaseEvent) {

	case (AT_B_FROM_ATTACK):
		incrementArbiter = &SimpleEnvelope::releaseState;
		return release;

	default:
		return attack;

	}
}

int32_t SimpleEnvelope::releaseState(void) {

	if (trigger == 0) {
		incrementArbiter = &SimpleEnvelope::retriggerState;
		return -attack;
	}

	switch (phaseEvent) {

	case (AT_A_FROM_RELEASE):
		incrementArbiter = &SimpleEnvelope::restingState;
		phase = 1;
		return 0;
	default:
		return release;

	};
}

int32_t SimpleEnvelope::retriggerState(void) {

	switch (phaseEvent) {

	case (AT_B_FROM_RELEASE):
		incrementArbiter = &SimpleEnvelope::releaseState;
		return release;

	default:
		return -attack;

	}
}

int32_t SimpleEnvelope::restingState(void) {

	if (trigger == 0) {
		incrementArbiter = &SimpleEnvelope::attackState;
		return attack;
	} else {
		return 0;
	}
}


