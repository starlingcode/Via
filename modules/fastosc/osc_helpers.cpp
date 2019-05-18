/*
 * calibration_helpers.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: willmitchell
 */

#include <osc.hpp>

void ViaOsc::renderSaw(int32_t writePosition) {

//	setLogicOut(0, 1);

	int32_t aFreq = fix16_mul(basePitch * octave, 65536 - (detune >> 6) * !unity);
	int32_t bFreq = fix16_mul(basePitch * octave, 65536 + (detune >> 5) * !unity);
	int32_t cFreq = basePitch * octave;

	int32_t pmInput = inputs.cv2Samples[0];

	int32_t pm = (pmInput - lastPM) << 11;

	lastPM = pmInput;

	int32_t aLevel;
	int32_t bLevel;
	int32_t cLevel;

	uint32_t aPhaseWorker = aPhase;
	uint32_t bPhaseWorker = bPhase;
	uint32_t cPhaseWorker = cPhase;

	int32_t samplesRemaining = TEMPLATE_BUFFER_SIZE;

	while (samplesRemaining) {

		aPhaseWorker += aFreq + pm;
		bPhaseWorker += bFreq + pm;
		cPhaseWorker += cFreq + pm;

		aLevel = aPhaseWorker >> 20;
		bLevel = bPhaseWorker >> 20;
		cLevel = cPhaseWorker >> 20;

		/// Write the 12 bit "inverse" (4095 - sample) of the sine sample to the channel A VCA control.
		outputs.dac1Samples[writePosition] = aLevel;
		/// Write the sample to the to the channel A VCA control. This accomplishes the crossfade contour output method.
		outputs.dac2Samples[writePosition] = bLevel;
		/// Write the dac3Output sample rendered in advanceLFO().
		outputs.dac3Samples[writePosition] = cLevel;

		samplesRemaining --;
		writePosition ++;
	}

	aPhase = aPhaseWorker;
	bPhase = bPhaseWorker;
	cPhase = cPhaseWorker;

	int32_t thisLogicA = GET_ALOGIC_MASK((aLevel > 2048));
	int32_t thisLogicB = GET_EXPAND_LOGIC_MASK((bLevel > 2048));

	outputs.logicA[0] = thisLogicA;
	outputs.auxLogic[0] = thisLogicB;

}

void ViaOsc::renderSquare(int32_t writePosition) {

//	setLogicOut(0, 1);

	int32_t aFreq = fix16_mul(basePitch * octave, 65536 - (detune >> 6) * !unity);
	int32_t bFreq = fix16_mul(basePitch * octave, 65536 + (detune >> 5) * !unity);
	int32_t cFreq = basePitch * octave;

	int32_t pmInput = inputs.cv2Samples[0];

	int32_t pm = (pmInput - lastPM) << 11;

	lastPM = pmInput;

	int32_t aLevel;
	int32_t bLevel;
	int32_t cLevel;

	uint32_t aPhaseWorker = aPhase;
	uint32_t bPhaseWorker = bPhase;
	uint32_t cPhaseWorker = cPhase;

	int32_t samplesRemaining = TEMPLATE_BUFFER_SIZE;

	while (samplesRemaining) {

		aPhaseWorker += aFreq + pm;
		bPhaseWorker += bFreq + pm;
		cPhaseWorker += cFreq + pm;

		aLevel = (aPhaseWorker >> 31) * 4095;
		bLevel = (bPhaseWorker >> 31) * 4095;
		cLevel = (cPhaseWorker >> 31) * 4095;

		/// Write the 12 bit "inverse" (4095 - sample) of the sine sample to the channel A VCA control.
		outputs.dac1Samples[writePosition] = aLevel;
		/// Write the sample to the to the channel A VCA control. This accomplishes the crossfade contour output method.
		outputs.dac2Samples[writePosition] = bLevel;
		/// Write the dac3Output sample rendered in advanceLFO().
		outputs.dac3Samples[writePosition] = cLevel;

		samplesRemaining --;
		writePosition ++;
	}

	aPhase = aPhaseWorker;
	bPhase = bPhaseWorker;
	cPhase = cPhaseWorker;

	int32_t thisLogicA = GET_ALOGIC_MASK((aLevel > 2048));
	int32_t thisLogicB = GET_EXPAND_LOGIC_MASK((bLevel > 2048));

	outputs.logicA[0] = thisLogicA;
	outputs.auxLogic[0] = thisLogicB;

}

void ViaOsc::renderTri(int32_t writePosition) {

//	setLogicOut(0, 1);

	int32_t aFreq = fix16_mul(basePitch * octave, 65536 - (detune >> 6) * !unity);
	int32_t bFreq = fix16_mul(basePitch * octave, 65536 + (detune >> 5) * !unity);
	int32_t cFreq = basePitch * octave;

	int32_t pmInput = inputs.cv2Samples[0];

	int32_t pm = (pmInput - lastPM) << 11;

	lastPM = pmInput;

	int32_t aLevel;
	int32_t bLevel;
	int32_t cLevel;

	uint32_t aPhaseWorker = aPhase;
	uint32_t bPhaseWorker = bPhase;
	uint32_t cPhaseWorker = cPhase;

	int32_t samplesRemaining = TEMPLATE_BUFFER_SIZE;

	while (samplesRemaining) {

		aPhaseWorker += aFreq + pm;
		bPhaseWorker += bFreq + pm;
		cPhaseWorker += cFreq + pm;

		aLevel = (aPhaseWorker >> 31) ? aPhaseWorker >> 19 : 8191 - (aPhaseWorker >> 19);
		bLevel = (bPhaseWorker >> 31) ? bPhaseWorker >> 19 : 8191 - (bPhaseWorker >> 19);
		cLevel = (cPhaseWorker >> 31) ? cPhaseWorker >> 19 : 8191 - (cPhaseWorker >> 19);

		/// Write the 12 bit "inverse" (4095 - sample) of the sine sample to the channel A VCA control.
		outputs.dac1Samples[writePosition] = aLevel;
		/// Write the sample to the to the channel A VCA control. This accomplishes the crossfade contour output method.
		outputs.dac2Samples[writePosition] = bLevel;
		/// Write the dac3Output sample rendered in advanceLFO().
		outputs.dac3Samples[writePosition] = cLevel;

		samplesRemaining --;
		writePosition ++;
	}

	aPhase = aPhaseWorker;
	bPhase = bPhaseWorker;
	cPhase = cPhaseWorker;

	int32_t thisLogicA = GET_ALOGIC_MASK((aLevel > 2048));
	int32_t thisLogicB = GET_EXPAND_LOGIC_MASK((bLevel > 2048));

	outputs.logicA[0] = thisLogicA;
	outputs.auxLogic[0] = thisLogicB;

}

