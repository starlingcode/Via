/*
 * sync3_modes.cpp
 *
 *  Created on: Jul 16, 2019
 *      Author: willmitchell
 */

#include "sync3.hpp"

void ViaSync3::handleButton1ModeChange(int32_t mode) {

	updateOutputs = oscCombos[sync3UI.button1Mode][sync3UI.button3Mode][sync3UI.button6Mode];

}

void ViaSync3::handleButton2ModeChange(int32_t mode) {


}

void ViaSync3::handleButton3ModeChange(int32_t mode) {

	updateOutputs = oscCombos[sync3UI.button1Mode][sync3UI.button3Mode][sync3UI.button6Mode];

}

void ViaSync3::handleButton4ModeChange(int32_t mode) {

	phaseModOn = mode;

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

void ViaSync3::handleButton6ModeChange(int32_t mode) {

	updateOutputs = oscCombos[sync3UI.button1Mode][sync3UI.button3Mode][sync3UI.button6Mode];

}
