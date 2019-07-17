/*
 * sync3_modes.cpp
 *
 *  Created on: Jul 16, 2019
 *      Author: willmitchell
 */

#include "sync3.hpp"

void ViaSync3::handleButton2ModeChange(int32_t mode) {

	if (mode == 0) {
		updateOutputs = &ViaSync3::updateOutputsSaw;
	} else if (mode == 1) {
		updateOutputs = &ViaSync3::updateOutputsSquare;
	} else if (mode == 2) {
		updateOutputs = &ViaSync3::updateOutputsTriangle;
	}

}


void ViaSync3::handleButton5ModeChange(int32_t mode) {

	if (mode == 0) {
		multipliers = multipliersInt;
	} else if (mode == 1) {
		multipliers = multipliersMinor;
	} else if (mode == 2) {
		multipliers = multipliersRhythms;
	} else if (mode == 3) {
		multipliers = multipliersMinorPent;
	}

}
