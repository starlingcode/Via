/*
 * single_frame_wavetable.c
 *
 *  Created on: Aug 26, 2018
 *      Author: willmitchell
 */

#include "oscillators.hpp"

void SyncWavetable::parseControls(ViaControls * controls) {

	morphBase = controls->knob3Value << 4;

}



void SyncWavetable::spline(uint32_t * wavetable, uint32_t * phaseDistTable) {

	uint32_t localPhase = phase;

	int32_t localIncrement = increment << oversamplingFactor;

	int32_t pmAmount = (int32_t) -pm[0];

	pmAmount += 32767 + cv2Offset;

	int32_t phaseModulationValue = (pmAmount - previousPhaseMod) << 16;

	previousPhaseMod = pmAmount;

	phaseMod += phaseModulationValue;

	localIncrement += phaseModulationValue;

	localPhase += localIncrement;

	localPhase *= phaseReset;
	phaseReset = 1;

	// log a -1 if the max value index of the wavetable is traversed from the left
	// log a 1 if traversed from the right
	// do this by subtracting the sign bit of the last phase from the current phase, both less the max phase index
	// this adds cruft to the wrap indicators, but that is deterministic and can be parsed out

	phase = localPhase;

	int32_t localPWM = (int32_t) pwm[0];
	localPWM = __USAT(localPWM + cv2Offset + 32768, 16);
	uint32_t pwmIndex = (localPWM >> 11);
	uint32_t pwmFrac = (localPWM & 0x7FF) << 4;
	// assuming that each phase distortion lookup table is 65 samples long stored as int32_t
	uint32_t * pwmTable1 = phaseDistTable + pwmIndex * 65;
	uint32_t * pwmTable2 = pwmTable1 + 65;
	uint32_t leftSample = localPhase >> 26;

#define SPLINE_PWM_PHASE_FRAC (localPhase & 0x3FFFFFF) >> 11
		// use this with the precalculated pwm to perform bilinear interpolation
		// this accomplishes the
	int32_t	localGhostPhase = fix15_bilerp(pwmTable1[leftSample], pwmTable2[leftSample],
				pwmTable1[leftSample + 1], pwmTable2[leftSample + 1], pwmFrac,
				SPLINE_PWM_PHASE_FRAC);


	int32_t morph = (int32_t) -morphMod[0];
	morph += cv3Offset;
	morph = __USAT(morph + morphBase, 16);

	morph *= tableSize;

	ghostPhase = localGhostPhase;

	int32_t sample = __USAT(getSampleQuinticSplineDeltaValue(localGhostPhase, morph, wavetable, &delta, 0), 12);

	int32_t samplesRemaining = bufferSize;
	int32_t writeIndex = 0;

	while (samplesRemaining) {

		signalOut[writeIndex] = sample;
		phaseOut[writeIndex] = localGhostPhase;

		writeIndex ++;
		samplesRemaining --;

	}

}

void SyncWavetable::oversample(uint32_t * wavetable, uint32_t * phaseDistTable) {

	int32_t pmAmount = (int32_t) -pm[0];
	pmAmount += 32767 + cv2Offset;
	int32_t phaseModulationValue = (pmAmount - previousPhaseMod) << (16 - oversamplingFactor);
	previousPhaseMod = pmAmount;
	phaseMod += phaseModulationValue;

	// now oversample

	int32_t localPWM = (int32_t) pwm[0];
	localPWM = __USAT(localPWM + cv2Offset + 32768, 16);
	uint32_t pwmIndex = (localPWM >> 11);
	uint32_t pwmFrac = (localPWM & 0x7FF) << 4;
	// assuming that each phase distortion lookup table is 65 samples long stored as int32_t
	uint32_t * pwmTable1 = phaseDistTable + pwmIndex * 65;
	uint32_t * pwmTable2 = pwmTable1 + 65;

	// combine knob and CV then to table size in 16.16 fixed point
	int32_t morphModLocal = -morphMod[0];
	morphModLocal += cv3Offset;
	uint32_t scaledMorph = __USAT((morphBase + morphModLocal), 16) * tableSize;
	uint32_t morphIndex = scaledMorph >> 16;
	uint32_t morphFrac = scaledMorph & 0xFFFF;
	// assuming that each phase distortion lookup table is 517 samples long stored as int
	wavetable += (morphIndex * 517) + 2;

	// scale increment to size of new phase space (<< 7) and down by oversampling factor
	int32_t localIncrement = increment + phaseModulationValue;
	uint32_t leftSample;

	uint32_t localPhase = phase * phaseReset;

	int32_t samplesRemaining = bufferSize - 1;
	int32_t writeIndex = 0;

	int32_t localGhostPhase = 0;

	while (samplesRemaining) {

		// phase pointer wraps at 32 bits
		localPhase = (localPhase + localIncrement);
		// treat the msb of phase as a 6.15 (tablesize.interpolationbits) fixed point number
		// divide by right shifting phase size (32 bits) less table size (6 bits) to find the nearest sample to the left
		leftSample = localPhase >> 26;
		// extract the less significant bits as fractional phase
#define OS_PWM_PHASE_FRAC (localPhase & 0x3FFFFFF) >> 11
		// use this with the precalculated pwm to perform bilinear interpolation
		// this accomplishes the
		localGhostPhase = fix15_bilerp(pwmTable1[leftSample], pwmTable2[leftSample],
				pwmTable1[leftSample + 1], pwmTable2[leftSample + 1], pwmFrac,
				OS_PWM_PHASE_FRAC);

		// write phase out
		phaseOut[writeIndex] = localGhostPhase;
		// get the actual wavetable output sample as above
		// but with the appropriate scaling as phase is now 25 bits and table length is 9 bits
		leftSample = localGhostPhase >> 16;
#define OS_PHASE_FRAC (localGhostPhase & 0xFFFF)
		signalOut[writeIndex] = fast_15_16_bilerp_prediff(
				wavetable[leftSample], wavetable[leftSample + 1], morphFrac,
				OS_PHASE_FRAC);
		writeIndex++;
		samplesRemaining--;
	}

	localPhase = (localPhase + localIncrement);

	leftSample = localPhase >> 26;

	localGhostPhase = fix15_bilerp(pwmTable1[leftSample], pwmTable2[leftSample],
			pwmTable1[leftSample + 1], pwmTable2[leftSample + 1], pwmFrac,
			OS_PWM_PHASE_FRAC);

	phaseOut[writeIndex] = localGhostPhase;

	leftSample = localGhostPhase >> 16;

	// get the delta

	signalOut[writeIndex] = fast_15_16_bilerp_prediff_deltaValue(
			wavetable[leftSample], wavetable[leftSample + 1], morphFrac,
			OS_PHASE_FRAC, &delta);

	// this should give a sample at 0 phase for hard sync
	phase = localPhase;

	ghostPhase = localGhostPhase;


}
