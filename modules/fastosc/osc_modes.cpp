/*
 * modes.c
 *
 *  Created on: Aug 22, 2018
 *      Author: willmitchell
 */

#include "osc.hpp"

void ViaOsc::handleButton1ModeChange(int32_t mode) {

	shAOn = mode;
	shBOn = mode;

}

void ViaOsc::handleButton2ModeChange(int32_t mode) {

	scaleMode = mode;

	if (mode == 0) {
		scale = chromatic;
	} else if (mode == 1) {
		scale = chromatic;
	} else if (mode == 2) {
		scale = major;
	} else if (mode == 3) {
		scale = minor;
	}

}

void ViaOsc::handleButton3ModeChange(int32_t mode) {

	if (mode) {
		doDetune = &ViaOsc::linearDetune;
	} else {
		doDetune = &ViaOsc::scaledDetune;
	}

}

void ViaOsc::handleButton4ModeChange(int32_t mode) {

	octaveRange = mode + 1;

}

void ViaOsc::handleButton5ModeChange(int32_t mode) {

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

void ViaOsc::handleButton6ModeChange(int32_t mode) {

	chordMode = mode;
	if (mode) {
		doDetune = &ViaOsc::chordalDetune;
	} else {
		doDetune = &ViaOsc::scaledDetune;
	}

}


