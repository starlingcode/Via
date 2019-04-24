/*
 * calibration_helpers.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: willmitchell
 */

#include <osc.hpp>

//constexpr int32_t Sine::big_sine[4097];

/// Iterate over the relevant section of the dac output buffers like in renderFixedOutputs().
void ViaOsc::renderTestOutputs(int32_t writePosition) {


	int32_t pmInput = ((int32_t) inputs.cv2Samples[0]);

	oscillator2.updatePM(pmInput << 11);

	int32_t fmInput = ((oscillator2.evaluatePM() - 16383)) << 2;

	fmInput = fix16_mul(fmInput, __USAT((controls.knob3 << 4) - ((int32_t) inputs.cv3Samples[0]), 16));

//	int32_t fmInput = ((oscillator2.evaluatePM() - 16383)) >> 3;
//	fmInput = expo.convert(__USAT(2048 + ((fmInput * controls.knob3Value) >> 12), 12)) >> 5;

	oscillator.updatePM(pmInput << 11);

	uint32_t sample;

	int32_t samplesRemaining = TEMPLATE_BUFFER_SIZE;

	while (samplesRemaining) {

		/// Magic number alert, the sine output is 15 bits so we scale down to 12 bits.
		sample = oscillator.evaluatePMFM(fmInput) >> 3;

		/// Write the 12 bit "inverse" (4095 - sample) of the sine sample to the channel A VCA control.
		outputs.dac1Samples[writePosition] = 4095 - sample;
		/// Write the sample to the to the channel A VCA control. This accomplishes the crossfade contour output method.
		outputs.dac2Samples[writePosition] = sample;
		/// Write the dac3Output sample rendered in advanceLFO().
		outputs.dac3Samples[writePosition] = (oscillator.phase >> 20);

		samplesRemaining --;
		writePosition ++;
	}

}

