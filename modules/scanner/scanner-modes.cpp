/*
 * modes.c
 *
 *  Created on: Aug 22, 2018
 *      Author: willmitchell
 */

#include "scanner.hpp"

void ViaScanner::handleButton4ModeChange(int32_t mode) {

	switchWavetableX(wavetableArray[0][mode]);

	if (mode >= 6) {
		scanner.xInterpolateOff = 1;
	} else {
		scanner.xInterpolateOff = 0;
	}

}

void ViaScanner::handleButton2ModeChange(int32_t mode) {

	switchWavetableY(wavetableArray[1][mode]);

	if (mode >= 6) {
		scanner.yInterpolateOff = 1;
	} else {
		scanner.yInterpolateOff = 0;
	}

}

void ViaScanner::handleButton6ModeChange(int32_t mode) {

	switchWavetableX(wavetableArray[0][mode]);

	if (mode >= 6) {
		scanner.xInterpolateOff = 1;
	} else {
		scanner.xInterpolateOff = 0;
	}
}

void ViaScanner::handleButton1ModeChange(int32_t mode) {

	scanner.syncMode = mode;

}

void ViaScanner::handleButton5ModeChange(int32_t mode) {

	switchWavetableY(wavetableArray[1][mode]);

	if (mode >= 6) {
		scanner.yInterpolateOff = 1;
	} else {
		scanner.yInterpolateOff = 0;
	}

}

void ViaScanner::handleButton3ModeChange(int32_t mode) {

	switch (mode) {
	case sum:
		scanner.fillBuffer = &ThreeAxisScanner::fillBufferSum;
		break;
	case multiply:
		scanner.fillBuffer = &ThreeAxisScanner::fillBufferMultiply;
		break;
	case difference:
		scanner.fillBuffer = &ThreeAxisScanner::fillBufferDifference;
		break;
	case lighten:
		scanner.fillBuffer = &ThreeAxisScanner::fillBufferLighten;
		break;

	}

}


