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
void ViaDelay::process(int32_t writePosition) {

	int32_t newSample = (((int32_t) inputs.cv3Samples[0])) >> 2;

	int32_t delayMod = (int32_t) inputs.cv2Samples[0];
	delayMod *= delayModOn;
	delayMod += 32767 + (32768 * !delayModOn);

	int32_t feedbackMod = (int32_t) inputs.cv2Samples[0];
	feedbackMod *= feedbackModOn;

	delayTime = fix16_mul(delayTimeTarget, delayMod);

	int32_t readPosition = ((delayWrite) << 16) - delayTime;
	readPosition &= 0xFFFFFFF;

	int32_t readIndex = readPosition >> 16;
	int32_t readFractional = readPosition & 0xFFFF;
	int32_t outputSample = 0;

	outputSample = fast_15_16_lerp(delayLine[readIndex], delayLine[(readIndex + 1) & 0xFFF], readFractional);

	outputs.dac3Samples[writePosition] = __USAT((outputSample >> 3) + 2048 + 110, 12);

//	outputs.dac3Samples[writePosition] = (((int32_t) inputs.cv3Samples[0]) + 32767) >> 4;

	delayWrite += 1;
	delayWrite &= 0xFFF;

	int32_t noise;

	if (burstCounter > 0) {
		burstCounter --;
		advanceLFSR();
		noise = lfsrState - 2048;
	} else {
		noise = 0;
	}

	delayLine[delayWrite] = __SSAT(newSample + noise +
								fix16_mul(outputSample,
										__USAT((controls.knob3Value << 4) + feedbackMod, 16)),
											15);

}

