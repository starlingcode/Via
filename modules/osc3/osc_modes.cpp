/*
 * modes.c
 *
 *  Created on: Aug 22, 2018
 *      Author: willmitchell
 */

#include "osc.hpp"

void ViaOsc::handleButton1ModeChange(int32_t mode) {

	octaveRange = mode + 1;

}

void ViaOsc::handleButton2ModeChange(int32_t mode) {

	if (mode == 0) {
		render = &ViaOsc::renderSaw;
	} else if (mode == 1) {
		render = &ViaOsc::renderSquare;
	} else if (mode == 2) {
		render = &ViaOsc::renderTrap;
	}  else if (mode == 3) {
		render = &ViaOsc::renderTri;
	}

}

void ViaOsc::handleButton3ModeChange(int32_t mode) {

	shAOn = mode;
	shBOn = mode;

}

void ViaOsc::handleButton4ModeChange(int32_t mode) {

	octaveRange = mode + 1;

}

void ViaOsc::handleButton5ModeChange(int32_t mode) {

	scaleMode = mode;

	if (mode == 0) {
		scale = chromatic;
	} else if (mode == 1) {
		scale = chromatic;
	} else if (mode == 2) {
		scale = major;
		intervals = majorIntervals;
	} else if (mode == 3) {
		scale = minor;
		intervals = minorIntervals;
	}

}

void ViaOsc::handleButton6ModeChange(int32_t mode) {

	if (mode == 0) {
		doDetune = &ViaOsc::linearDetune;
		chordMode = 0;
	} else if (mode == 1) {
		doDetune = &ViaOsc::scaledDetune;
		chordMode = 0;
	} else {
		chordMode = 1;
		doDetune = &ViaOsc::chordalDetune;

	}

}
