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
		updateBaseFreqs = &ViaOsc3::updateBaseFreqsSmooth;
        scale = scales[0].notes;
        intervals = scales[0].intervals;
        chords = scales[0].chords;
        scaleDegrees = scales[0].degrees;
	} else {
		updateBaseFreqs = &ViaOsc3::updateBaseFreqsScale;
        scale = scales[mode - 1].notes;
        intervals = scales[mode - 1].intervals;
        chords = scales[mode - 1].chords;
        scaleDegrees = scales[mode - 1].degrees;
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
