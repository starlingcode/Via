#include "sync.hpp"


void ViaSync::mainRisingEdgeCallback(void) {

#ifdef BUILD_VIRTUAL
	uint32_t reading = readMeasurementTimer();
#endif
#ifdef BUILD_F373
	uint32_t reading = TIM2->CNT;
#endif

	if (reading < 4 * 1440) {

		pileUp += 1;

	} else {

		measureFrequency(reading);
#ifdef BUILD_VIRTUAL
		uint32_t playbackPosition = (reading % 1440)/180;
#endif
#ifdef BUILD_F373
		uint32_t playbackPosition = (SYNC_BUFFER_SIZE * 2) - DMA1_Channel5->CNDTR;
#endif

		phaseSignal = syncWavetable.purePhaseOut[playbackPosition];
		doCorrection = 0;
		clockDiv = pileUp + 1;
		pileUp = 0;

		int32_t multKey = fracMultiplier + intMultiplier;
		ratioChange = (lastMultiplier != multKey);
		lastMultiplier = multKey;
		outputs.auxLogic[0] = GET_EXPAND_LOGIC_MASK(ratioChange);

		if (runtimeDisplay & showYChange) {
			setLEDD(yIndexChange);
	#ifdef BUILD_F373
			TIM17->CR1 |= TIM_CR1_CEN;
	#endif
		}

	}

	simultaneousTrigFlag = 1;
#ifdef BUILD_VIRTUAL
	virtualTimerEnable = 1;
	virtualTimer = 0;
#endif
#ifdef BUILD_F373
	TIM18->CNT = 1;
	TIM18->CR1 |= TIM_CR1_CEN;
#endif

	tapTempo = 0;

}

void ViaSync::mainFallingEdgeCallback(void) {

	ratioChange = 0;
	outputs.auxLogic[0] = GET_EXPAND_LOGIC_MASK(ratioChange);

}

void ViaSync::auxRisingEdgeCallback(void) {

	if (!simultaneousTrigFlag) {
		pllReset = 0;
	} else {
		pllReset = 0;
		phaseSignal = syncWavetable.phase;
		phaseModSignal = syncWavetable.phaseMod;

		doCorrection = 0;
		// should these be initialized to point to the same address?

		syncWavetable.increment = increment;
		syncWavetable.phase = phaseSignal;
	}

}
void ViaSync::auxFallingEdgeCallback(void) {
	;
}

void ViaSync::buttonPressedCallback(void) {

	if (tapTempo != 0) {
		// store the length of the last period
		// average against the current length

#ifdef BUILD_F373

		int32_t tap = TIM2->CNT;
		// reset the timer value
		TIM2->CNT = 0;
#endif

#ifdef BUILD_VIRTUAL

		int32_t tap = readMeasurementTimer();
		// reset the timer value
		resetMeasurementTimer();
#endif

		tapSum += tap - readBuffer(&tapStore, 1);
		writeBuffer(&tapStore, tap);

		periodCount = tapSum >> 1;

		lastTap = tap;

		clockDiv = 1;

		// doPLL();
		generateFrequency();

		// should these be initialized to point to the same address?

		syncWavetable.increment = increment;
		syncWavetable.phaseReset = phaseReset;

		outputs.auxLogic[0] = GET_EXPAND_LOGIC_MASK(ratioChange);

	} else {
		tapTempo = 1;
	}

	this->syncUI.dispatch(EXPAND_SW_ON_SIG);

}
void ViaSync::buttonReleasedCallback(void) {

	this->syncUI.dispatch(EXPAND_SW_OFF_SIG);

}

void ViaSync::auxTimer1InterruptCallback(void) {

	yIndexChange = 0;
	setLEDD(yIndexChange);

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

	updateFrequency();

	syncWavetable.advance((uint32_t *)wavetableRead, 0);

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

	updateFrequency();

	syncWavetable.advance((uint32_t *)wavetableRead, SYNC_BUFFER_SIZE);

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
	parseControls(&controls, &inputs);

	if (tapTempo) {
		generateFrequency();
		syncWavetable.increment = increment;
	}

	int32_t sample = outputs.dac2Samples[0];

	int32_t redSignal = fix16_mul(sample << 4, scaleColor.r);
	int32_t blueSignal = fix16_mul(sample << 4, scaleColor.b);
	int32_t greenSignal = fix16_mul(sample << 4, scaleColor.g);

	updateRGBDisplay(redSignal, greenSignal, blueSignal, runtimeDisplay);
	updateRGBPreset(syncUI.timerRead(), syncUI.presetNumber);

}

