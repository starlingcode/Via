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

	pllController.scale = (Scale *) scaleArray[syncUI.GROUP_MODE][mode];
	scaleHue =  mode + syncUI.GROUP_MODE * 4;
	scaleColor.r = hueSpace[scaleHue].r;
	scaleColor.g = hueSpace[scaleHue].g;
	scaleColor.b = hueSpace[scaleHue].b;
	updateRGBDisplay(scaleColor.r, scaleColor.g, scaleColor.b, 1);


}

void ViaSync::handleButton3ModeChange(int32_t mode) {

	switch (mode) {
	case root:
		pllController.rootMod = inputs.cv2Samples;
		syncWavetable.fm = inputs.cv2VirtualGround;
		syncWavetable.pm = inputs.cv2VirtualGround;
		syncWavetable.pwm = inputs.cv2VirtualGround;
		showYChange = 1;
		break;
	case pm:
		pllController.rootMod = inputs.cv2VirtualGround;
		syncWavetable.fm = inputs.cv2VirtualGround;
		syncWavetable.pm = inputs.cv2Samples;
		syncWavetable.pwm = inputs.cv2VirtualGround;
		showYChange = 0;
		break;
	case pwm:
		syncWavetable.pm = 0;
		pllController.rootMod = inputs.cv2VirtualGround;
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

	pllController.nudgeSum = 0;

	for (int i = 0; i < 32; i ++) {
		writeBuffer(&pllController.nudgeBuffer, 0);
	}

	pllController.syncMode = mode + 1;

}

void ViaSync::handleButton5ModeChange(int32_t mode) {

	syncUI.SCALE_MODE = 0;
	pllController.scale = (Scale *) scaleArray[mode][syncUI.SCALE_MODE];
	scaleHue = mode * 4 + syncUI.SCALE_MODE;
	scaleColor.r = hueSpace[scaleHue].r;
	scaleColor.g = hueSpace[scaleHue].g;
	scaleColor.b = hueSpace[scaleHue].b;
	updateRGBDisplay(scaleColor.r, scaleColor.g, scaleColor.b, 1);
	if (syncUI.TABLE_GROUP_MODE) {
		switchWavetableGlobal(wavetableArrayGlobal[syncUI.TABLE_MODE]);
	} else {
		syncUI.TABLE_MODE = 0;
		switchWavetable(wavetableArray[mode][syncUI.TABLE_MODE]);
	}

}

void ViaSync::handleButton5ModeInit(int32_t mode) {

	pllController.scale = (Scale *) scaleArray[mode][syncUI.SCALE_MODE];
	scaleHue = mode * 4 + syncUI.SCALE_MODE;
	scaleColor.r = hueSpace[scaleHue].r;
	scaleColor.g = hueSpace[scaleHue].g;
	scaleColor.b = hueSpace[scaleHue].b;
	updateRGBDisplay(scaleColor.r, scaleColor.g, scaleColor.b, 1);
	if (syncUI.TABLE_GROUP_MODE) {
		switchWavetableGlobal(wavetableArrayGlobal[syncUI.TABLE_MODE]);
	} else {
		syncUI.TABLE_MODE = 0;
		switchWavetable(wavetableArray[mode][syncUI.TABLE_MODE]);
	}

}

void ViaSync::handleButton6ModeChange(int32_t mode) {

	if (syncUI.TABLE_GROUP_MODE) {
		switchWavetableGlobal(wavetableArrayGlobal[mode]);
	} else {
		switchWavetable(wavetableArray[syncUI.GROUP_MODE][mode]);
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
		pllController.phaseOffset = 0;
		break;
	case quarter:
		pllController.phaseOffset = 1 << 23;
		break;
	case half:
		pllController.phaseOffset = 1 << 24;
		break;
	case threeQuarters:
		pllController.phaseOffset = (3 << 23);
		break;
	default:
		break;
	}

}

void ViaSync::handleAux4ModeChange(int32_t mode) {

	if (mode) {
		switchWavetableGlobal(wavetableArrayGlobal[syncUI.TABLE_MODE]);
	} else {
		switchWavetable(wavetableArray[syncUI.GROUP_MODE][syncUI.TABLE_MODE]);
	}

}
