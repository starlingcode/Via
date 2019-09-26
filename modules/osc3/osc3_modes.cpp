/*
 * modes.c
 *
 *  Created on: Aug 22, 2018
 *      Author: willmitchell
 */

#include "osc3.hpp"

void ViaOsc3::handleButton1ModeChange(int32_t mode) {

	octaveRange = mode + 1;

}

void ViaOsc3::handleButton2ModeChange(int32_t mode) {

	if (mode == 0) {
		render = &ViaOsc3::renderSaw;
	} else if (mode == 1) {
		render = &ViaOsc3::renderSquare;
	} else if (mode == 2) {
		render = &ViaOsc3::renderTrap;
	}  else if (mode == 3) {
		render = &ViaOsc3::renderTri;
	}

}

void ViaOsc3::handleButton3ModeChange(int32_t mode) {

	shAOn = mode;
	shBOn = mode;

}

void ViaOsc3::handleButton4ModeChange(int32_t mode) {

	octaveRange = mode + 1;

}

void ViaOsc3::handleButton5ModeChange(int32_t mode) {

	scaleMode = mode;

	if (mode == 0) {
		scale = chromatic;
		updateBaseFreqs = &ViaOsc3::updateBaseFreqsSmooth;
	} else if (mode == 1) {
		scale = chromatic;
		intervals = majorIntervals;
		updateBaseFreqs = &ViaOsc3::updateBaseFreqsSemi;
	} else if (mode == 2) {
		scale = major;
		intervals = majorIntervals;
		updateBaseFreqs = &ViaOsc3::updateBaseFreqsScale;
	} else if (mode == 3) {
		scale = minor;
		intervals = minorIntervals;
		updateBaseFreqs = &ViaOsc3::updateBaseFreqsScale;
	}

}

void ViaOsc3::handleButton6ModeChange(int32_t mode) {

	if (mode == 0) {
		doDetune = &ViaOsc3::linearDetune;
		chordMode = 0;
		clockedBeat = 0;
	} else if (mode == 1) {
		doDetune = &ViaOsc3::scaledDetune;
		chordMode = 0;
		clockedBeat = 0;
	} else if (mode == 2) {
		chordMode = 1;
		clockedBeat = 0;
		doDetune = &ViaOsc3::chordalDetune;
	} else {
		chordMode = 0;
		clockedBeat = 1;
		doDetune = &ViaOsc3::clockedDetune;
	}

}
