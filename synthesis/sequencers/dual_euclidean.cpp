/*
 * dual_euclidean.c
 *
 *  Created on: Aug 21, 2018
 *      Author: willmitchell
 */

#include <sequencers.hpp>

#include "dsp.hpp"

#define RUNTIME_DISPLAY 1



void DualEuclidean::advanceSequencerA(void) {

	uint32_t aPatternValue;

	aPatternIndex = (aCounter + offset) % aLength;

	aPatternValue = currentABank->patternBank[aPatternMorph][aPatternIndex];

	aCounter = (aCounter + 1) % aLength;

	aOutput = aPatternValue;

}

void DualEuclidean::advanceSequencerB(void) {

	uint32_t bPatternValue;

	//TODO relate offset to scale lengths
	bPatternIndex = bCounter;

	//lookup the logic values
	bPatternValue = currentBBank->patternBank[bPatternMorph][bPatternIndex];

	//increment the sequence counter
	bCounter = (bCounter + 1) % bLength;

	bOutput = bPatternValue;

}

void DualEuclidean::updateLogicOutput(void) {

	// jump to the conditional check appropriate for the currently selected logic operation

#define __AND 0
#define __OR 1
#define __XOR 2
#define __NOR 3

	switch (auxLogicMode) {
	case __OR:
		if (aOutput || bOutput) {
			logicOutput = 1;
		} else {
			logicOutput = 0;
		}
		break;
	case __AND:
		if (aOutput && bOutput) {
			logicOutput = 1;
		} else {
			logicOutput = 0;
		}
		break;
	case __XOR:
		if (aOutput ^ bOutput) {
			logicOutput = 1;
		} else {
			logicOutput = 0;
		}
		break;
	case __NOR:
		// nand instead?
		if (!aOutput && !bOutput) {
			logicOutput = 1;
		} else {
			logicOutput = 0;
		}
		break;
	}

	logicOutput &= virtualGateHigh;

}


void DualEuclidean::parseControls(ViaControls * controls,
		ViaInputStreams * inputs) {

	//determine a pattern index for the a and b grids and the offset
	//the positve going excursion of the cv scales all the way to the maximum value
	//the negative does the same to the maximum value

	int32_t cv2Sample = (int32_t) -inputs->cv2Samples[0];
	cv2Sample += cv2Offset;
	cv2Sample = cv2Sample >> 4;
	cv2Sample += 2048;
	int32_t cv3Sample = (int32_t) -inputs->cv3Samples[0];
	cv3Sample += cv3Offset;
	cv3Sample = cv3Sample >> 4;
	cv3Sample += 2048;

	uint32_t cv1WOffset;

#ifdef BUILD_VIRTUAL
	cv1WOffset = controls->cv1Value;
#endif
#ifdef BUILD_F373
	cv1WOffset = __USAT(controls->cv1Value - cv1Offset, 12);
#endif


	if (cv1WOffset >= 2048) {
		aPatternMorph = (fix16_lerp(controls->knob1Value, 4095,
				(cv1WOffset - 2048) << 5)) >> 8;
	} else {
		aPatternMorph = (fix16_lerp(0, controls->knob1Value,
				cv1WOffset << 5)) >> 8;
	}

	if (modulateMultiplier) {

		if (cv2Sample >= 2048) {
			multiplier = (fix16_lerp(controls->knob2Value, 4095,
					(cv2Sample - 2048) << 5)) >> 9;
		} else {
			multiplier = (fix16_lerp(0, controls->knob2Value,
					cv2Sample << 5)) >> 9;
		}
		//0-7 -> 1-8
		multiplier = multipliers[multiplier];

	} else if (shuffleOn) {

		if (cv2Sample >= 2048) {
			shuffle = ((fix16_lerp(controls->knob2Value, 4095,
					(cv2Sample - 2048) << 5)) - 2048) << 3;
		} else {
			shuffle = ((fix16_lerp(0, controls->knob2Value,
					cv2Sample << 5)) - 2048) << 3;
		}

	} else {

		if (cv2Sample >= 2048) {
			offset = (fix16_lerp(controls->knob2Value, 4095,
					(cv2Sample - 2048) << 5)) >> 9;
		} else {
			offset = (fix16_lerp(0, controls->knob2Value,
					cv2Sample << 5)) >> 9;
		}

	}

	if (cv3Sample >= 2048) {
		bPatternMorph = (fix16_lerp(controls->knob3Value, 4095,
				(cv3Sample - 2048) << 5)) >> 8;
	} else {
		bPatternMorph = (fix16_lerp(0, controls->knob3Value,
				cv3Sample << 5)) >> 8;
	}

	//get the lengths of the currently indexed patterns

	aLength = currentABank->lengths[aPatternMorph];
	bLength = currentBBank->lengths[bPatternMorph];

}

void DualEuclidean::processMainRisingEdge(void) {

	advanceSequencerB();

	if (skipClock & clockOn) {
		skipClock = 0;
	} else {
#ifdef BUILD_VIRTUAL
	periodCount = virtualTimer1Count;
	virtualTimer1Count = 0;
	if (!clockOn || virtualTimer2Count > 16) {
		virtualTimer3Enable = 0;
		virtualTimer3Count = 0;
		//virtualTimer2Prescaler = divider - 1; // no division implemented yet
		virtualTimer2Count = 0;
		shuffledStep = 1;
		processSeq1 = 1;
	}
	skipClock = 1;
	}
#endif
#ifdef BUILD_F373
	periodCount = TIM5->CNT;
	TIM5->CNT = 0;

	if (!clockOn || TIM2->CNT > (clockPeriod >> 1)) {

		TIM2->PSC = divider-1;
		TIM17->CR1 &= ~TIM_CR1_CEN;
		TIM17->CNT = 0;

		TIM2->CNT = 0;
		TIM2->EGR = TIM_EGR_UG;
		shuffledStep = 1;
	}

	skipClock = 1;

	}
	updateLogicOutput();
#endif

	// update the simple sequencer sample and hold control

	if (sampleB) {
		shBSignal = (!bOutput);
	} else if (trackB) {
		shBSignal = (bOutput);
	} else {
		shBSignal = 0;
	}

}

void DualEuclidean::processInternalRisingEdge(void) {


	// complex sequencer "rising edge"

	// signal a rising edge
	virtualGateHigh = 1;

	// update the complex sequencer, s&h, gates, leds
	advanceSequencerA();
	updateLogicOutput();

	if (sampleA) {
		shASignal = (!aOutput);
	} else if (trackA) {
		shASignal = (aOutput);
	} else {
		shASignal = 0;
	}

	// prepare the next step, set the gate timer

	clockPeriod = periodCount/multiplier;

	if (shuffledStep) {
		shuffleDelay = fix16_mul(clockPeriod, shuffle);
		shuffledStep = 0;
	} else {
		shuffleDelay = -fix16_mul(clockPeriod, shuffle);
		shuffledStep = 1;
	}
	clockPeriod += shuffleDelay;

#ifdef BUILD_F373

	TIM2->ARR = clockPeriod;
	TIM17->ARR = __USAT(clockPeriod >> 13, 16);
	TIM17->CNT = 1;
	//if (clockOn) {
		TIM17->CR1 = TIM_CR1_CEN;
	//}
#endif
#ifdef BUILD_VIRTUAL
	virtualTimer2Overflow = clockPeriod;
	virtualTimer3Overflow = clockPeriod >> 1;
	virtualTimer3Count = 0;
	//if (clockOn) {
		virtualTimer3Enable = 1;
	//}
#endif


}

void DualEuclidean::processMainFallingEdge(void) {

	// gate low on the simple sequencer
//	if (!clockOn) {
//		aOutput = 0;
//		virtualGateHigh = 0;
//		shASignal = sampleA;
//	}

	bOutput = 0;
	updateLogicOutput();

}

void DualEuclidean::processInternalFallingEdge(void) {

	// virtual gate low, much like the simple sequencer


	virtualGateHigh = 0;
	aOutput = 0;
	shASignal = sampleA;
	updateLogicOutput();


	// disable the gate timer
#ifdef BUILD_F373
	TIM17->CR1 &= ~TIM_CR1_CEN;
#endif
#ifdef BUILD_VIRTUAL
	virtualTimer3Enable = 0;
#endif

}

