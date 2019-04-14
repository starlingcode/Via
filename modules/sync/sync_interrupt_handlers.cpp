#include "sync.hpp"


const int32_t phaseModPWMTables[33][65] = {phaseModPWM_0, phaseModPWM_1, phaseModPWM_2, phaseModPWM_3, phaseModPWM_4, phaseModPWM_5, phaseModPWM_6, phaseModPWM_7, phaseModPWM_8, phaseModPWM_9, phaseModPWM_10, phaseModPWM_11, phaseModPWM_12, phaseModPWM_13, phaseModPWM_14, phaseModPWM_15, phaseModPWM_16, phaseModPWM_17, phaseModPWM_18, phaseModPWM_19, phaseModPWM_20, phaseModPWM_21, phaseModPWM_22, phaseModPWM_23, phaseModPWM_24, phaseModPWM_25, phaseModPWM_26, phaseModPWM_27, phaseModPWM_28, phaseModPWM_29, phaseModPWM_30, phaseModPWM_31, phaseModPWM_32};

void ViaSync::mainRisingEdgeCallback(void) {

	simultaneousTrigFlag = 1;
#ifdef BUILD_VIRTUAL
	virtualTimerEnable = 1;
	virtualTimer = 0;
#endif
#ifdef BUILD_F373
	TIM18->CNT = 1;
	TIM18->CR1 |= TIM_CR1_CEN;
#endif


	pllController.measureFrequency();
	pllController.phaseSignal = syncWavetable.phase;
	pllController.phaseModSignal = syncWavetable.phaseMod;
	pllController.doPLL();
	pllController.generateFrequency();

	// should these be initialized to point to the same address?

	syncWavetable.increment = pllController.increment;
	syncWavetable.phase = pllController.phaseSignal;


	outputs.auxLogic[0] = GET_EXPAND_LOGIC_MASK(pllController.ratioChange);
	if (runtimeDisplay & showYChange) {
		setLEDD(pllController.yIndexChange);
#ifdef BUILD_F373
		TIM17->CR1 |= TIM_CR1_CEN;
#endif
	}
	pllController.tapTempo = 0;

}

void ViaSync::mainFallingEdgeCallback(void) {

	pllController.ratioChange = 0;
	outputs.auxLogic[0] = GET_EXPAND_LOGIC_MASK(pllController.ratioChange);

}

void ViaSync::auxRisingEdgeCallback(void) {

	if (!simultaneousTrigFlag) {
		pllController.pllReset = 0;
	} else {
		pllController.pllReset = 0;
		pllController.phaseSignal = syncWavetable.phase;
		pllController.phaseModSignal = syncWavetable.phaseMod;
		pllController.doPLL();
		pllController.generateFrequency();

		// should these be initialized to point to the same address?

		syncWavetable.increment = pllController.increment;
		syncWavetable.phase = pllController.phaseSignal;
	}

}
void ViaSync::auxFallingEdgeCallback(void) {
	;
}

void ViaSync::buttonPressedCallback(void) {

	if (pllController.tapTempo != 0) {
		// store the length of the last period
		// average against the current length

#ifdef BUILD_F373

		int32_t tap = TIM2->CNT;
		// reset the timer value
		TIM2->CNT = 0;
#endif

#ifdef BUILD_VIRTUAL

		int32_t tap = pllController.virtualTimer;
		// reset the timer value
		pllController.virtualTimer = 0;
#endif

		tapSum += tap - readBuffer(&tapStore, 3);
		writeBuffer(&tapStore, tap);

		pllController.periodCount = tapSum >> 2;

		lastTap = tap;

		// pllController.doPLL();
		pllController.generateFrequency();

		// should these be initialized to point to the same address?

		syncWavetable.increment = pllController.increment;
		syncWavetable.phaseReset = pllController.phaseReset;

		outputs.auxLogic[0] = GET_EXPAND_LOGIC_MASK(pllController.ratioChange);

	} else {
		pllController.tapTempo = 1;
	}

	this->syncUI.dispatch(EXPAND_SW_ON_SIG);

}
void ViaSync::buttonReleasedCallback(void) {

	this->syncUI.dispatch(EXPAND_SW_OFF_SIG);

}

void ViaSync::auxTimer1InterruptCallback(void) {

	pllController.yIndexChange = 0;
	setLEDD(pllController.yIndexChange);

}

void ViaSync::auxTimer2InterruptCallback(void) {

	simultaneousTrigFlag = 0;
#ifdef BUILD_VIRTUAL
	virtualTimer = 0;
	virtualTimerEnable = 0;
#endif
#ifdef BUILD_F373
	TIM18->CNT = 1;
	TIM18->CR1 &= ~TIM_CR1_CEN;
#endif

}

void ViaSync::ioProcessCallback(void) {
}

void ViaSync::halfTransferCallback(void) {

	setLogicOut(0, runtimeDisplay);

	syncWavetable.advance((uint32_t *)wavetableRead, (uint32_t *) phaseModPWMTables);

	int32_t samplesRemaining = outputBufferSize;
	int32_t writeIndex = 0;
	int32_t readIndex = 0;

	while (samplesRemaining) {

		int32_t sample = syncWavetable.signalOut[readIndex];
		outputs.dac1Samples[writeIndex] = 4095 - sample;
		outputs.dac2Samples[writeIndex] = sample;
		//outputs.dac3Samples[writeIndex] = pllController.errorSig;

		writeIndex ++;
		readIndex ++;
		samplesRemaining --;

	}

	int32_t thisSample = syncWavetable.ghostPhase >> 16;
	int32_t thisState = !(thisSample >> 8);
	hemisphereHysterisis(thisState, thisSample);

	(this->*calculateDac3)(0);
	(this->*calculateLogicA)(0);
	(this->*calculateSH)(0);

	hemisphereLastSample = hemisphereState;

}

void ViaSync::transferCompleteCallback(void) {

	setLogicOut(0, runtimeDisplay);

	syncWavetable.advance((uint32_t *)wavetableRead, (uint32_t *) phaseModPWMTables);

	int32_t samplesRemaining = outputBufferSize;
	int32_t writeIndex = SYNC_BUFFER_SIZE;
	int32_t readIndex = 0;

	while (samplesRemaining) {

		int32_t sample = syncWavetable.signalOut[readIndex];
		outputs.dac1Samples[writeIndex] = 4095 - sample;
		outputs.dac2Samples[writeIndex] = sample;
		//outputs.dac3Samples[writeIndex] = pllController.errorSig;

		writeIndex ++;
		readIndex ++;
		samplesRemaining --;

	}

	int32_t thisSample = syncWavetable.ghostPhase >> 16;
	int32_t thisState = !(thisSample >> 8);
	hemisphereHysterisis(thisState, thisSample);

	(this->*calculateDac3)(SYNC_BUFFER_SIZE);
	(this->*calculateLogicA)(0);
	(this->*calculateSH)(0);

	hemisphereLastSample = hemisphereState;

}

void ViaSync::slowConversionCallback(void) {

	controls.update();
	syncWavetable.parseControls(&controls);
	pllController.parseControls(&controls, &inputs);

	if (pllController.tapTempo) {
		pllController.generateFrequency();
		syncWavetable.increment = pllController.increment;
	}

	int32_t sample = outputs.dac2Samples[0];

	int32_t redSignal = fix16_mul(sample << 4, scaleColor.r);
	int32_t blueSignal = fix16_mul(sample << 4, scaleColor.b);
	int32_t greenSignal = fix16_mul(sample << 4, scaleColor.g);

	updateRGBDisplay(redSignal, greenSignal, blueSignal, runtimeDisplay);

}

