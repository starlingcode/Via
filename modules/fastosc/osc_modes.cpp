/*
 * modes.c
 *
 *  Created on: Aug 22, 2018
 *      Author: willmitchell
 */

#include "osc.hpp"

void ViaOsc::handleButton1ModeChange(int32_t mode) {


}

void ViaOsc::handleButton2ModeChange(int32_t mode) {

	if (mode == 0) {
		render = &ViaOsc::renderSaw;
	} else if (mode == 1) {
		render = &ViaOsc::renderSquare;
	} else if (mode == 2) {
		render = &ViaOsc::renderTri;
	}

}

void ViaOsc::handleButton3ModeChange(int32_t mode) {


}

void ViaOsc::handleButton4ModeChange(int32_t mode) {


}

void ViaOsc::handleButton5ModeChange(int32_t mode) {


}

void ViaOsc::handleButton6ModeChange(int32_t mode) {


}


