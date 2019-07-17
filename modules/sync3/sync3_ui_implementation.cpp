/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include <sync3.hpp>
#include "user_interface.hpp"

/// Callback to link to the C code in the STM32 Sync3 Sense Library.
void sync3TouchLink (void * uiVoid) {
	ViaSync3::ViaSync3UI * ui = (ViaSync3::ViaSync3UI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);

}

void ViaSync3::ViaSync3UI::recallModuleState(void) {

	button1Mode %= this_module.numButton1Modes;
	button2Mode %= this_module.numButton2Modes;
	button3Mode %= this_module.numButton3Modes;
	button4Mode %= this_module.numButton4Modes;
	button5Mode %= this_module.numButton5Modes;
	button6Mode %= this_module.numButton6Modes;

	this_module.handleButton1ModeChange(button1Mode);
	this_module.handleButton2ModeChange(button2Mode);
	this_module.handleButton3ModeChange(button3Mode);
	this_module.handleButton4ModeChange(button4Mode);
	this_module.handleButton5ModeChange(button5Mode);
	this_module.handleButton6ModeChange(button6Mode);

}

void ViaSync3::ViaSync3UI::defaultEnterMenuCallback(void) {

	/// Executed when the module returns to an idle state, waiting for a touch event
	this_module.runtimeDisplay = 1;

}

void ViaSync3::ViaSync3UI::button2EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button2Mode);
	resetTimerMenu();

}

void ViaSync3::ViaSync3UI::button2TapCallback(void) {

	button2Mode = incrementModeAndStore(button2Mode, BUTTON2_MASK, this_module.numButton2Modes, BUTTON2_SHIFT);
	this_module.handleButton2ModeChange(button2Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button2Mode);
	transition(&ViaSync3::ViaSync3UI::newModeMenu);

}

void ViaSync3::ViaSync3UI::button2HoldCallback(void) {

	transition(&ViaSync3::ViaSync3UI::defaultMenu);

}

void ViaSync3::ViaSync3UI::button5EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button5Mode);
	resetTimerMenu();

}

void ViaSync3::ViaSync3UI::button5TapCallback(void) {

	button5Mode = incrementModeAndStore(button5Mode, BUTTON5_MASK, this_module.numButton5Modes, BUTTON5_SHIFT);
	this_module.handleButton5ModeChange(button5Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button5Mode);
	transition(&ViaSync3::ViaSync3UI::newModeMenu);

}

void ViaSync3::ViaSync3UI::button5HoldCallback(void) {

	transition(&ViaSync3::ViaSync3UI::defaultMenu);

}

