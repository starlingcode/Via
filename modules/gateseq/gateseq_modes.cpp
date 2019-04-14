/*
 * osc_modes.c
 *
 *  Created on: Aug 21, 2018
 *      Author: willmitchell
 */

#include <gateseq.hpp>

void ViaGateseq::handleButton1ModeChange(int32_t mode) {

	switch (mode) {
	case 0:
		sequencer.sampleA = 0;
		sequencer.trackA = 0;
		break;
	case 1:
		sequencer.sampleA = 1;
		sequencer.trackA = 0;
		break;
	case 2:
		sequencer.sampleA = 0;
		sequencer.trackA = 1;
		break;
	}

}

void ViaGateseq::handleButton2ModeChange(int32_t mode) {

	switch (mode) {
	case 0:
		sequencer.gateAEvent = SOFT_GATE_HIGH;
		sequencer.andA = 0;
		softGateAOn = 0;
		break;
	case 1:
		sequencer.andA = 1;
		sequencer.gateAEvent = SOFT_GATE_LOW;
		softGateAOn = 0;
		break;
	case 2:
		sequencer.andA = 1;
		sequencer.gateAEvent = SOFT_GATE_LOW;
		softGateAOn = 1;
		break;
	}

}

void ViaGateseq::handleButton3ModeChange(int32_t mode) {

	sequencer.currentABank = seq1Banks[mode];

	setRedLED((mode >> 1) * 4095);
	setGreenLED(!(mode >> 1) * 4095);


	if (mode == 1 || mode == 3) {
		SET_BLUE_LED_ONOFF(1);

	} else {
		SET_BLUE_LED_ONOFF(0);

	}

	switch (mode) {
	case 0:

#ifdef BUILD_F373
		TIM2->CR1 &= ~TIM_CR1_CEN;
#endif
#ifdef BUILD_VIRTUAL
		sequencer.virtualTimer2Enable = 0;
		sequencer.virtualTimer2Count = 0;
#endif
		sequencer.clockOn = 0;
		sequencer.modulateMultiplier = 0;
		sequencer.perStepReset = 0;
		sequencer.multReset = 2;
		sequencer.multiplier = 1;
		sequencer.shuffleOn = 0;
		sequencer.shuffle = 0;
		break;
	case 1:
#ifdef BUILD_F373
		TIM2->CR1 |= TIM_CR1_CEN;
#endif
#ifdef BUILD_VIRTUAL
		sequencer.virtualTimer2Enable = 1;
		sequencer.virtualTimer2Count = 1;
#endif
		sequencer.clockOn = 1;
		sequencer.modulateMultiplier = 0;
		sequencer.perStepReset = 0;
		sequencer.multReset = 6;
		sequencer.multiplier = 3;
		sequencer.shuffleOn = 0;
		sequencer.shuffle = 0;
		break;
	case 2:
#ifdef BUILD_F373
		TIM2->CR1 |= TIM_CR1_CEN;
#endif
#ifdef BUILD_VIRTUAL
		sequencer.virtualTimer2Enable = 1;
		sequencer.virtualTimer2Count = 1;
#endif
		sequencer.clockOn = 1;
		sequencer.modulateMultiplier = 0;
		sequencer.perStepReset = 1;
		sequencer.multReset = 4;
		sequencer.multiplier = 4;
		sequencer.shuffleOn = 1;
		sequencer.shuffle = 0;
		break;
	case 3:
#ifdef BUILD_F373
		TIM2->CR1 |= TIM_CR1_CEN;
#endif
#ifdef BUILD_VIRTUAL
		sequencer.virtualTimer2Enable = 1;
		sequencer.virtualTimer2Count = 1;
#endif
		sequencer.clockOn = 1;
		sequencer.modulateMultiplier = 1;
		sequencer.perStepReset = 1;
		sequencer.offset = 0;
		sequencer.shuffleOn = 0;
		sequencer.shuffle = 0;
		break;
	}

	sequencer.aCounter = 0;
	sequencer.bCounter = 0;


}

void ViaGateseq::handleButton4ModeChange(int32_t mode) {

	switch (mode) {
	case 0:
		sequencer.sampleB = 0;
		sequencer.trackB = 0;
		break;
	case 1:
		sequencer.sampleB = 1;
		sequencer.trackB = 0;
		break;
	case 2:
		sequencer.sampleB = 0;
		sequencer.trackB = 1;
		break;
	}

}

void ViaGateseq::handleButton5ModeChange(int32_t mode) {

	switch (mode) {
	case 0:
		sequencer.gateBEvent = SOFT_GATE_HIGH;
		sequencer.andB = 0;
		softGateBOn = 0;
		break;
	case 1:
		sequencer.andB = 1;
		sequencer.gateBEvent = SOFT_GATE_LOW;
		softGateBOn = 0;
		break;
	case 2:
		sequencer.andB = 1;
		sequencer.gateBEvent = SOFT_GATE_LOW;
		softGateBOn = 1;
		break;
	}

}

void ViaGateseq::handleButton6ModeChange(int32_t mode) {

	sequencer.currentBBank = seq2Banks[mode];

	sequencer.aCounter = 0;
	sequencer.bCounter = 0;

}

void ViaGateseq::handleAux2ModeChange(int32_t mode) {

	sequencer.auxLogicMode = mode;

}
