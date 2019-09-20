/*
 * calibration_helpers.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: willmitchell
 */

#include "delay.hpp"

/// defined for linkage
constexpr uint32_t RevExpoConverter::expoTable[];

/// Iterate over the relevant section of the dac output buffers like in renderFixedOutputs().
void ViaDelay::render(int32_t writePosition) {

	int32_t newSample = (((int32_t) inputs.cv3Samples[0])) >> 2;

	int32_t delayMod = (int32_t) inputs.cv2Samples[0];
	int32_t feedbackMod = (int32_t) inputs.cv2Samples[0];

	delayMod *= delayModOn;
	delayMod += 32767 + (32768 * !delayModOn);

	feedbackMod *= feedbackModOn;

	if (upsamplesLeft) {
		lastTarget += slewFactor;
		upsamplesLeft --;
	} else {
		lastTarget = delayTimeTarget;
	}

//	int32_t noise;

	if (burstCounter > 0) {
		burstCounter --;
		advanceLFSR();
//		noise = lfsrState - 32767;
	} else {
//		noise = 0;
	}

	delay.delayTime = fix16_mul(lastTarget, delayMod);
	delay.feedback = __USAT((controls.knob3Value << 4) + feedbackMod, 16);

	int32_t outputSample = delay.read();
	delay.write(newSample, outputSample);

	outputs.dac3Samples[writePosition] = __USAT((outputSample >> 3) + 2048, 12);

}

