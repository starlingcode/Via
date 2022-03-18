/*
 * modes.c
 *
 *  Created on: Aug 21, 2018
 *      Author: willmitchell
 */

#include "sync.hpp"

void ViaSync::handleButton1ModeChange(int32_t mode) {

	switch (mode) {
	case noSH:
		calculateSH = &ViaSync::calculateSHMode1;
		break;
	case sampletrack:
		calculateSH = &ViaSync::calculateSHMode2;
		break;
	case resample:
		calculateSH = &ViaSync::calculateSHMode3;
		break;
	default:
		break;
	}

}

void ViaSync::handleButton2ModeChange(int32_t mode) {

	selectedScale = (Scale *) scaleArray[syncUI.GROUP_MODE][mode];
	scaleHue =  mode + syncUI.GROUP_MODE * 4;
	scaleColor.r = hueSpace[scaleHue].r;
	scaleColor.g = hueSpace[scaleHue].g;
	scaleColor.b = hueSpace[scaleHue].b;
	updateRGBDisplay(scaleColor.r, scaleColor.g, scaleColor.b, 1);


}

void ViaSync::handleButton3ModeChange(int32_t mode) {

	switch (mode) {
	case root:
		rootMod = inputs.cv2Samples;
		syncWavetable.fm = inputs.cv2VirtualGround;
		syncWavetable.pm = inputs.cv2VirtualGround;
		syncWavetable.pwm = inputs.cv2VirtualGround;
		showYChange = 1;
		break;
	case pm:
		rootMod = inputs.cv2VirtualGround;
		syncWavetable.fm = inputs.cv2VirtualGround;
		syncWavetable.pm = inputs.cv2Samples;
		syncWavetable.pwm = inputs.cv2VirtualGround;
		showYChange = 0;
		break;
	case pwm:
		syncWavetable.pm = 0;
		rootMod = inputs.cv2VirtualGround;
		syncWavetable.fm = inputs.cv2VirtualGround;
		syncWavetable.pm = inputs.cv2VirtualGround;
		syncWavetable.pwm = inputs.cv2Samples;
		showYChange = 0;
		break;
	default:
		break;
	}


}

void ViaSync::handleButton4ModeChange(int32_t mode) {

	// see pllMultiplierMeasureFrequency for why this is in range 1, 2, 3

	nudgeSum = 0;

	for (int i = 0; i < 32; i ++) {
		writeBuffer(&nudgeBuffer, 0);
	}

	syncMode = mode;

}

void ViaSync::handleButton5ModeChange(int32_t mode) {

	syncUI.SCALE_MODE = 0;
	selectedScale = (Scale *) scaleArray[mode][syncUI.SCALE_MODE];
	scaleHue = mode * 4 + syncUI.SCALE_MODE;
	scaleColor.r = hueSpace[scaleHue].r;
	scaleColor.g = hueSpace[scaleHue].g;
	scaleColor.b = hueSpace[scaleHue].b;
	updateRGBDisplay(scaleColor.r, scaleColor.g, scaleColor.b, 1);
	if (syncUI.TABLE_GROUP_MODE) {
		switchWavetable(wavetableArray + 4 * 4 + syncUI.TABLE_MODE);
	} else {
		syncUI.TABLE_MODE = 0;
		switchWavetable(wavetableArray + 4 * mode + syncUI.TABLE_MODE);
	}

}

void ViaSync::handleButton5ModeInit(int32_t mode) {

	selectedScale = (Scale *) scaleArray[mode][syncUI.SCALE_MODE];
	scaleHue = mode * 4 + syncUI.SCALE_MODE;
	scaleColor.r = hueSpace[scaleHue].r;
	scaleColor.g = hueSpace[scaleHue].g;
	scaleColor.b = hueSpace[scaleHue].b;
	updateRGBDisplay(scaleColor.r, scaleColor.g, scaleColor.b, 1);
	if (syncUI.TABLE_GROUP_MODE) {
		switchWavetable(wavetableArray + 4 * 4 + syncUI.TABLE_MODE);
	} else {
		switchWavetable(wavetableArray + 4 * mode + syncUI.TABLE_MODE);
	}

}

void ViaSync::handleButton6ModeChange(int32_t mode) {

	if (syncUI.TABLE_GROUP_MODE) {
		switchWavetable(wavetableArray + 4 * 4 + mode);
	} else {
		switchWavetable(wavetableArray + 4 * syncUI.GROUP_MODE + mode);
	}

}

void ViaSync::handleAux1ModeChange(int32_t mode) {

	switch (mode) {
	case sync_gate:
		calculateLogicA = &ViaSync::calculateLogicAGate;
		break;
	case sync_delta:
		calculateLogicA = &ViaSync::calculateLogicADelta;
		break;
	default:
		break;
	}

}


void ViaSync::handleAux2ModeChange(int32_t mode) {

	switch (mode) {
	case sync_phasor:
		calculateDac3 = &ViaSync::calculateDac3Phasor;
		break;
	case sync_contour:
		calculateDac3 = &ViaSync::calculateDac3Contour;
		break;
	default:
		break;
	}

}


void ViaSync::handleAux3ModeChange(int32_t mode) {

	switch (mode) {
	case noOffset:
		phaseOffset = 0;
		break;
	case quarter:
		phaseOffset = 1 << 23;
		break;
	case half:
		phaseOffset = 1 << 24;
		break;
	case threeQuarters:
		phaseOffset = (3 << 23);
		break;
	default:
		break;
	}

}

void ViaSync::handleAux4ModeChange(int32_t mode) {

	if (syncUI.TABLE_GROUP_MODE) {
		switchWavetable(wavetableArray + 4 * 4 + syncUI.TABLE_MODE);
	} else {
		switchWavetable(wavetableArray + 4 * syncUI.GROUP_MODE + syncUI.TABLE_MODE);
	}

}
