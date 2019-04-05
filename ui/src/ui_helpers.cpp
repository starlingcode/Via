/*
 * ui_helpers.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: willmitchell
 */

#include "user_interface.hpp"
#include "touch_lib_link.hpp"

/// Called when subclasses are constructed.
void linkUI(void * callback, void * ui) {

	thisUI = ui;
	touchStateChangeCallback = (void (*)(void *)) callback;

}


int32_t ViaUI::incrementModeAndStore(int32_t mode, int32_t mask, int32_t numModes, int32_t shift) {

	mode = (mode + 1) % numModes;
	modeStateBuffer = (modeStateBuffer & ~(mask)) | (mode << shift);
	return mode;

}

int32_t ViaUI::decrementModeAndStore(int32_t mode, int32_t mask, int32_t numModes, int32_t shift) {

	mode = (mode - 1);
	if (mode < 0) {
		mode += numModes;
	}
	modeStateBuffer = (modeStateBuffer & ~(mask)) | (mode << shift);
	return mode;

}

void ViaUI::storeMode(int32_t mode, int32_t mask, int32_t shift) {

	modeStateBuffer = (modeStateBuffer & ~(mask)) | (mode << shift);

}
