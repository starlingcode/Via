/*
 * meta_interrupt_handlers.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


/*
 * meta_interrupt_handlers.c
 *
 *  Created on: Aug 29, 2018
 *      Author: willmitchell
 */

#include "meta.hpp"

const uint32_t phaseModPWMTables[33][65] = {phaseModPWM_0, phaseModPWM_1, phaseModPWM_2, phaseModPWM_3, phaseModPWM_4, phaseModPWM_5, phaseModPWM_6, phaseModPWM_7, phaseModPWM_8, phaseModPWM_9, phaseModPWM_10, phaseModPWM_11, phaseModPWM_12, phaseModPWM_13, phaseModPWM_14, phaseModPWM_15, phaseModPWM_16, phaseModPWM_17, phaseModPWM_18, phaseModPWM_19, phaseModPWM_20, phaseModPWM_21, phaseModPWM_22, phaseModPWM_23, phaseModPWM_24, phaseModPWM_25, phaseModPWM_26, phaseModPWM_27, phaseModPWM_28, phaseModPWM_29, phaseModPWM_30, phaseModPWM_31, phaseModPWM_32};

void ViaMeta::mainRisingEdgeCallback(void) {

	metaController.triggerSignal = 0;

	metaController.gateSignal = 1 * metaController.gateOn;

	ampEnvelope.trigger = 0;
	freqTransient.trigger = 0;
	morphEnvelope.trigger = 0;

	updateRGB = &ViaMeta::updateRGBBlink;

	#ifdef BUILD_F373

	TIM17->CR1 |= TIM_CR1_CEN;

	#endif
	#ifdef BUILD_VIRTUAL

	blinkTimerEnable = 1;

	#endif


}

void ViaMeta::mainFallingEdgeCallback(void) {

	metaController.gateSignal = 0;

}

void ViaMeta::auxRisingEdgeCallback(void) {

	metaController.freeze = 0;

}
void ViaMeta::auxFallingEdgeCallback(void)
{
	metaController.freeze = 1;

}

void ViaMeta::buttonPressedCallback(void) {

	metaController.triggerSignal = 0;

	metaController.gateSignal = 1 * metaController.gateOn;

	ampEnvelope.trigger = 0;
	freqTransient.trigger = 0;
	morphEnvelope.trigger = 0;

	this->metaUI.dispatch(EXPAND_SW_ON_SIG);

}
void ViaMeta::buttonReleasedCallback(void) {

	metaController.gateSignal = 0;

	this->metaUI.dispatch(EXPAND_SW_OFF_SIG);

}

void ViaMeta::ioProcessCallback(void) {

	// no need

}

void ViaMeta::halfTransferCallback(void) {

	setLogicOut(0, runtimeDisplay);

	metaController.generateIncrementsExternal(&inputs);
	metaController.advancePhaseExternal((uint32_t *) phaseModPWMTables);
	metaWavetable.phase = metaController.phaseBeforeIncrement;
	metaWavetable.increment = metaController.incrementUsed;
	metaWavetable.advance((uint32_t *) wavetableRead);
	(this->*outputStage)(0);
	calculateDelta(0);
	(this->*calculateDac3)(0);
	(this->*calculateLogicA)(0);
	(this->*calculateSH)(0);
	metaController.triggerSignal = 1;

}

void ViaMeta::transferCompleteCallback(void) {

	setLogicOut(1, runtimeDisplay);

	metaController.generateIncrementsExternal(&inputs);
	metaController.advancePhaseExternal((uint32_t *) phaseModPWMTables);
	metaWavetable.phase = metaController.phaseBeforeIncrement;
	metaWavetable.increment = metaController.incrementUsed;
	metaWavetable.advance((uint32_t *) wavetableRead);
	(this->*outputStage)(META_BUFFER_SIZE);
	calculateDelta(1);
	(this->*calculateDac3)(META_BUFFER_SIZE);
	(this->*calculateLogicA)(1);
	(this->*calculateSH)(1);
	metaController.triggerSignal = 1;

}

void ViaMeta::slowConversionCallback(void) {


	controls.update();
	metaWavetable.parseControls(&controls);
	metaController.parseControlsExternal(&controls, &inputs);
	ampEnvelope.parseControls(&controls, &inputs);
	freqTransient.attack = __USAT(ampEnvelope.release * freqAttackMultiplier, 19);
	if (freqTransient.attack < 524287) {
		freqTransient.attack = 524287;
	}
	freqTransient.release = __USAT(ampEnvelope.release * freqReleaseMultiplier - minTransientLength, 15);
	if (freqTransient.release < 32767) {
		freqTransient.attack = 32767;
	}
	morphEnvelope.release = ampEnvelope.release * morphReleaseMultiplier;
	if (morphEnvelope.release < morphReleaseClamp) {
		morphEnvelope.release = morphReleaseClamp;
	}
	morphEnvelope.attack = __USAT(ampEnvelope.release * morphAttackMultiplier, 19);
	if (morphEnvelope.attack < 143360) {
		morphEnvelope.attack = 200360;
	}




	(this->*updateRGB)();

}

void ViaMeta::auxTimer1InterruptCallback(void) {

	updateRGB = &ViaMeta::updateRGBBlank;

}

void ViaMeta::auxTimer2InterruptCallback(void) {

	updateRGB = currentRGBBehavior;

}


