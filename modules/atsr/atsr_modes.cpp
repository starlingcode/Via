/*
 * modes.c
 *
 *  Created on: Aug 22, 2018
 *      Author: willmitchell
 */

#include "atsr.hpp"

void ViaAtsr::handleButton1ModeChange(int32_t mode) {

	if (mode == 0) {
		aSlope = (int32_t *) expoSlope;
	} else if (mode == 1) {
		aSlope = (int32_t *) linSlope;
	} else if (mode ==  2) {
		aSlope = (int32_t *) sigmoidSlope;
	} else if (mode ==  3) {
		aSlope = (int32_t *) logSlope;
	}


}

void ViaAtsr::handleButton2ModeChange(int32_t mode) {

	if (mode == 0) {
		dSlope = (int32_t *) expoSlope;
	} else if (mode == 1) {
		dSlope = (int32_t *) linSlope;
	} else if (mode ==  2) {
		dSlope = (int32_t *) sigmoidSlope;
	} else if (mode ==  3) {
		dSlope = (int32_t *) logSlope;
	}


}

void ViaAtsr::handleButton3ModeChange(int32_t mode) {

	if (mode == 0) {
		assignableLogic = &attacking;
	} else if (mode == 1) {
		assignableLogic = &ting;
	} else if (mode == 2) {
		assignableLogic = &sustaining;
	} else if (mode == 3) {
		assignableLogic = &releasing;
	}

}

void ViaAtsr::handleButton4ModeChange(int32_t mode) {

	cycleTime = mode;

}

void ViaAtsr::handleButton5ModeChange(int32_t mode) {

	shOn = mode;

}

void ViaAtsr::handleButton6ModeChange(int32_t mode) {

	if (mode == 0) {
		rSlope = (int32_t *) expoSlope;
	} else if (mode == 1) {
		rSlope = (int32_t *) linSlope;
	} else if (mode ==  2) {
		rSlope = (int32_t *) sigmoidSlope;
	} else if (mode ==  3) {
		rSlope = (int32_t *) logSlope;
	}


}


