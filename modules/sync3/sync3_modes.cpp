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

	numerators = scales[mode]->numerators;
	denominators = scales[mode]->denominators;
	dividedPhases = scales[mode]->dividedPhases;

	phaseLockOn = scales[mode]->pllStyle;

	phaseModTracker2 = 0;

	numerator1Select = 1;
	numerator2Select = 1;
	numerator3Select = 1;

	numerator1Alt = 1;
	numerator1Alt = 1;
	numerator1Alt = 1;

}

void ViaSync3::handleButton3ModeChange(int32_t mode) {

	updateOutputs = oscCombos[sync3UI.button1Mode][sync3UI.button3Mode][sync3UI.button6Mode];

}

void ViaSync3::handleButton4ModeChange(int32_t mode) {

	phaseModOn = mode;
	phaseModTracker2 = 0;

}

void ViaSync3::handleButton5ModeChange(int32_t mode) {

}

void ViaSync3::handleButton6ModeChange(int32_t mode) {

	updateOutputs = oscCombos[sync3UI.button1Mode][sync3UI.button3Mode][sync3UI.button6Mode];

}
