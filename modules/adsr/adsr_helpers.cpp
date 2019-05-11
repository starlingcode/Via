/*
 * calibration_helpers.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: willmitchell
 */

#include <adsr.hpp>

constexpr int32_t ViaAdsr::expoSlope[4097];
constexpr int32_t ViaAdsr::logSlope[4097];
constexpr int32_t ViaAdsr::linSlope[4097];
constexpr int32_t ViaAdsr::sigmoidSlope[4097];

void ViaAdsr::render(int32_t writePosition) {

	setLogicOut(0, 0);

	adsrState->step();

	uint32_t aLevel = adsrState->aLevel;
	uint32_t bLevel = adsrState->bLevel;
	int32_t aPulseWidth = (aLevel & 15) >> 1;
	int32_t bPulseWidth = (bLevel & 15) >> 1;
	aLevel >>= 4;
	bLevel >>= 4;

	int32_t loopGate = !releasing & !sustaining;
	int32_t assignableLogicOut = 2048 - loopGate * 2048;
	int32_t auxLogic = sustaining;

	outputs.logicA[0] = GET_ALOGIC_MASK(*assignableLogic);
	outputs.auxLogic[0] = GET_EXPAND_LOGIC_MASK(auxLogic);
	outputs.shA[0] = GET_SH_A_MASK((aLevel != 0) * shOn);
	outputs.shB[0] = GET_SH_B_MASK((bLevel != 0) * shOn);

	pwmCounter += 1;
	pwmCounter &= 255;

	if (runtimeDisplay) {
		setLEDA((pwmCounter < (aLevel >> 4)) * !shOn);
		setLEDB((pwmCounter < (bLevel >> 4)) * !shOn);
		setLEDD(loopGate);
		setLEDC(*assignableLogic);
	}

	int32_t samplesRemaining = VIA_ADSR_BUFFER_SIZE;
	int32_t pwmCounter = 0;

	while (samplesRemaining) {

		outputs.dac1Samples[writePosition] = __USAT(aLevel + (pwmCounter < aPulseWidth), 12);
		outputs.dac2Samples[writePosition] = __USAT(bLevel + (pwmCounter < bPulseWidth), 12);
		outputs.dac3Samples[writePosition] = assignableLogicOut;

		pwmCounter ++;
		pwmCounter &= 7;
		samplesRemaining --;
		writePosition ++;
	}

}

