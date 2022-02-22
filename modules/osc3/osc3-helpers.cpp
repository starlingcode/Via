/*
 * calibration_helpers.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: willmitchell
 */

#include <osc3.hpp>

void ViaOsc3::renderSaw(int32_t writePosition) {

	updateFrequencies();

	int32_t aFreqHere = aFreq;
	int32_t bFreqHere = bFreq;
	int32_t cFreqHere = cFreq;

	int32_t pmHere = pm;

	uint32_t aPhaseWorker = aPhase;
	uint32_t bPhaseWorker = bPhase;
	uint32_t cPhaseWorker = cPhase;

	uint32_t aLevel;
	uint32_t bLevel;
	uint32_t cLevel;

	int32_t samplesRemaining = OSC3_BUFFER_SIZE;

	while (samplesRemaining) {

		aPhaseWorker += aFreqHere + pmHere;
		bPhaseWorker += bFreqHere + pmHere;
		cPhaseWorker += cFreqHere;

		aLevel = aPhaseWorker >> 20;
		bLevel = bPhaseWorker >> 20;
		cLevel = cPhaseWorker >> 20;

		outputs.dac1Samples[writePosition] = aLevel;
		outputs.dac2Samples[writePosition] = bLevel;
		outputs.dac3Samples[writePosition] = 4095 - cLevel;

		samplesRemaining --;
		writePosition ++;
	}

	parsePhase(aPhaseWorker, bPhaseWorker, cPhaseWorker);

}

void ViaOsc3::renderSquare(int32_t writePosition) {

	updateFrequencies();

	int32_t aFreqHere = aFreq;
	int32_t bFreqHere = bFreq;
	int32_t cFreqHere = cFreq;

	int32_t pmHere = pm;

	uint32_t aPhaseWorker = aPhase;
	uint32_t bPhaseWorker = bPhase;
	uint32_t cPhaseWorker = cPhase;

	uint32_t aLevel;
	uint32_t bLevel;
	uint32_t cLevel;

	int32_t samplesRemaining = OSC3_BUFFER_SIZE;

	while (samplesRemaining) {

		aPhaseWorker += aFreqHere + pmHere;
		bPhaseWorker += bFreqHere + pmHere;
		cPhaseWorker += cFreqHere;

		aLevel = (aPhaseWorker >> 31) * 4095;
		bLevel = (bPhaseWorker >> 31) * 4095;
		cLevel = (cPhaseWorker >> 31) * 4095;

		outputs.dac1Samples[writePosition] = aLevel;
		outputs.dac2Samples[writePosition] = bLevel;
		outputs.dac3Samples[writePosition] = 4095 - cLevel;

		samplesRemaining --;
		writePosition ++;
	}

	parsePhase(aPhaseWorker, bPhaseWorker, cPhaseWorker);

}

void ViaOsc3::renderTri(int32_t writePosition) {

	updateFrequencies();

	int32_t aFreqHere = aFreq;
	int32_t bFreqHere = bFreq;
	int32_t cFreqHere = cFreq;

	int32_t pmHere = pm;

	uint32_t aPhaseWorker = aPhase;
	uint32_t bPhaseWorker = bPhase;
	uint32_t cPhaseWorker = cPhase;

	uint32_t aLevel;
	uint32_t bLevel;
	uint32_t cLevel;

	int32_t samplesRemaining = OSC3_BUFFER_SIZE;

	while (samplesRemaining) {

		aPhaseWorker += aFreqHere + pmHere;
		bPhaseWorker += bFreqHere + pmHere;
		cPhaseWorker += cFreqHere;

		aLevel = abs((int32_t) aPhaseWorker) >> 19;
		bLevel = abs((int32_t) bPhaseWorker) >> 19;
		cLevel = abs((int32_t) cPhaseWorker) >> 19;

		outputs.dac1Samples[writePosition] = aLevel;
		outputs.dac2Samples[writePosition] = bLevel;
		outputs.dac3Samples[writePosition] = 4095 - cLevel;

		samplesRemaining --;
		writePosition ++;

	}

	parsePhase(aPhaseWorker, bPhaseWorker, cPhaseWorker);

}

void ViaOsc3::renderTrap(int32_t writePosition) {

	updateFrequencies();

	int32_t aFreqHere = aFreq << 1;
	int32_t bFreqHere = bFreq << 1;
	int32_t cFreqHere = cFreq << 1;

	int32_t pmHere = pm << 1;

	uint32_t aPhaseWorker = aPhase;
	uint32_t bPhaseWorker = bPhase;
	uint32_t cPhaseWorker = cPhase;

	uint32_t aLevel;
	uint32_t bLevel;
	uint32_t cLevel;

	int32_t samplesRemaining = OSC3_BUFFER_SIZE >> 1;

	while (samplesRemaining) {

		aPhaseWorker += aFreqHere + pmHere;
		bPhaseWorker += bFreqHere + pmHere;
		cPhaseWorker += cFreqHere;

		aLevel = __SSAT((abs((int32_t) aPhaseWorker) >> 18) - 4096, 12);
		bLevel = __SSAT((abs((int32_t) bPhaseWorker) >> 18) - 4096, 12);
		cLevel = __SSAT((abs((int32_t) cPhaseWorker) >> 18) - 4096, 12);

		outputs.dac1Samples[writePosition] = aLevel + 2048;
		outputs.dac1Samples[writePosition + 1] = aLevel + 2048;
		outputs.dac2Samples[writePosition] = bLevel + 2048;
		outputs.dac2Samples[writePosition + 1] = bLevel + 2048;
		outputs.dac3Samples[writePosition] = 2047 - cLevel;
		outputs.dac3Samples[writePosition + 1] = 2047 - cLevel;

		samplesRemaining --;
		writePosition += 2;

	}

	parsePhase(aPhaseWorker, bPhaseWorker, cPhaseWorker);

}

