/*
 * simple_wavetable.c
 *
 *  Created on: Aug 29, 2018
 *      Author: willmitchell
 */

#include <oscillators.hpp>

void MetaWavetable::parseControls(ViaControls * controls) {

	morphBase = controls->knob3Value << 4;

};

void MetaWavetable::advanceSingleSample(uint32_t * wavetable) {

	int32_t morphScaleLocal = (int32_t) morphScale[0];
	morphScaleLocal = fix16_mul(morphBase, morphScaleLocal << 1);
	int32_t morphModLocal = (int32_t) -morphMod[0];
	morphModLocal = __USAT(morphScaleLocal + morphModLocal + morphModOffset, 16) * tableSize;

	uint32_t ghostPhase = phase << 7;
	// scale increment to size of new phase space (<< 7) and down by oversampling factor
	int32_t localIncrement = increment << (7 - oversamplingFactor);

	ghostPhase += localIncrement;

	phaseOut[0] = ghostPhase;

	int32_t sample = getSampleQuinticSplineDeltaValue(ghostPhase >> 7, (uint32_t) morphModLocal,
			wavetable, &delta, 0);

	uint32_t writeIndex = 0;
	uint32_t samplesRemaining = bufferSize;

	while (samplesRemaining) {

		phaseOut[writeIndex] = ghostPhase;
		signalOut[writeIndex] = sample;

		writeIndex++;
		samplesRemaining--;

	}

};



void MetaWavetable::advanceOversampled(uint32_t * wavetable) {

	// combine knob and CV then to table size in 16.16 fixed point
	int32_t morphModLocal = -morphMod[0];
	uint32_t scaledMorph = __USAT((morphBase + morphModLocal + morphModOffset), 16) * tableSize;
	uint32_t morphIndex = scaledMorph >> 16;
	uint32_t morphFrac = scaledMorph & 0xFFFF;
	// assuming that each phase distortion lookup table is 517 samples long stored as int
	uint32_t * wavetable1 = wavetable + (morphIndex * 517) + 2;

	uint32_t ghostPhase = phase << 7;
	// scale increment to size of new phase space (<< 7) and down by oversampling factor
	int32_t localIncrement = increment << (7 - oversamplingFactor);
	uint32_t leftSample;

	uint32_t writeIndex = 0;
	uint32_t samplesRemaining = bufferSize - 1;


	while (samplesRemaining) {
		// phase pointer wraps at 32 bits
		ghostPhase = (ghostPhase + localIncrement);
		// write phase out
		phaseOut[writeIndex] = ghostPhase;
		// get the actual wavetable output sample as above
		// but with the appropriate scaling as phase is now 25 bits and table length is 9 bits
		leftSample = ghostPhase >> 23;
#define phaseFrac ((ghostPhase >> 7) & 0xFFFF)
		signalOut[writeIndex] = fast_15_16_bilerp_prediff(
				wavetable1[leftSample], wavetable1[leftSample + 1], morphFrac,
				phaseFrac);
		writeIndex++;
		samplesRemaining--;
	}

	ghostPhase = (ghostPhase + localIncrement);
	// write phase out
	phaseOut[writeIndex] = ghostPhase;
	// get the actual wavetable output sample as above
	// but with the appropriate scaling as phase is now 25 bits and table length is 9 bits
	leftSample = ghostPhase >> 23;
	signalOut[writeIndex] = fast_15_16_bilerp_prediff_deltaValue(
			wavetable1[leftSample], wavetable1[leftSample + 1], morphFrac,
			phaseFrac, &delta);

}


