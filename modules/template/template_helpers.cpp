/*
 * calibration_helpers.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: willmitchell
 */

#include "template.hpp"

//constexpr int32_t Sine::big_sine[4097];

/// Iterate over the relevant section of the dac output buffers like in renderFixedOutputs().
void ViaTemplate::renderTestOutputs(int32_t writePosition) {


	byteBeatExecute(byteBeatCounter);

//	int32_t pmInput = ((int32_t) inputs.cv2Samples[0]);

//	oscillator.updatePM(fix16_mul(byteBeatResult, controls.knob2 << 4));

	oscillator.freq = fix16_mul(oscillator.freq, byteBeatResult);

//	oscillator.updatePM(byteBeatResult);

	uint32_t sample;

	int32_t samplesRemaining = TEMPLATE_BUFFER_SIZE;



	while (samplesRemaining) {

		/// Magic number alert, the sine output is 15 bits so we scale down to 12 bits.
		sample = oscillator.evaluatePM() >> 3;

		/// Write the 12 bit "inverse" (4095 - sample) of the sine sample to the channel A VCA control.
		outputs.dac1Samples[writePosition] = 4095 - sample;
		/// Write the sample to the to the channel A VCA control. This accomplishes the crossfade contour output method.
		outputs.dac2Samples[writePosition] = sample;
		/// Write the dac3Output sample rendered in advanceLFO().
		outputs.dac3Samples[writePosition] = (oscillator.phase >> 20);

		samplesRemaining --;
		writePosition ++;
	}

	setLEDC((byteBeatCounter & (1 << 31)) == 0);

}

