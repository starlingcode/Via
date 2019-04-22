/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include "delay.hpp"
#include "user_interface.hpp"

void ViaDelay::ViaDelayUI::initialize(void) {

#ifdef BUILD_VIRTUAL

	button1 = (int32_t *) &this_module.button1Input;
	button2 = (int32_t *) &this_module.button2Input;
	button3 = (int32_t *) &this_module.button3Input;
	button4 = (int32_t *) &this_module.button4Input;
	button5 = (int32_t *) &this_module.button5Input;
	button6 = (int32_t *) &this_module.button6Input;

#endif

#ifdef BUILD_F373

	tsl_user_Init();

	button1 = (int32_t *) &BUTTON1SENSOR;
	button2 = (int32_t *) &BUTTON2SENSOR;
	button3 = (int32_t *) &BUTTON3SENSOR;
	button4 = (int32_t *) &BUTTON4SENSOR;
	button5 = (int32_t *) &BUTTON5SENSOR;
	button6 = (int32_t *) &BUTTON6SENSOR;

	initializeMemory();

#endif

	loadFromEEPROM(0);
	recallModuleState();

	state = &ViaUI::defaultMenu;
	transition(&ViaUI::defaultMenu);
}

void delayTouchLink (void * uiVoid) {
	ViaDelay::ViaDelayUI * ui = (ViaDelay::ViaDelayUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaDelay::ViaDelayUI::recallModuleState(void) {

	/// Put your code to initialze the module to a given state here

}

void ViaDelay::ViaDelayUI::uiSetLEDs(int mode) {

	/// This connects the UI library's LED setting calls to the enumerated LED handler of the module.
	/// This could probably go away with some refactoring.

	this_module.setLEDs(mode);

}

void ViaDelay::ViaDelayUI::defaultEnterMenuCallback(void) {

	/// Executed when the module returns to an idle state, waiting for a touch event
	this_module.runtimeDisplay = 1;

}
void ViaDelay::ViaDelayUI::newModeEnterMenuCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}
void ViaDelay::ViaDelayUI::newAuxModeEnterMenuCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}
void ViaDelay::ViaDelayUI::presetEnterMenuCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}

void ViaDelay::ViaDelayUI::button1EnterMenuCallback(void) {

	/// Executed after a press is detected at button 1 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaDelay::ViaDelayUI::button2EnterMenuCallback(void) {

	/// Executed after a press is detected at button 2 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaDelay::ViaDelayUI::button3EnterMenuCallback(void) {

	/// Executed after a press is detected at button 3 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaDelay::ViaDelayUI::button4EnterMenuCallback(void) {

	/// Executed after a press is detected at button 4 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaDelay::ViaDelayUI::button5EnterMenuCallback(void) {

	/// Executed after a press is detected at button 5 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaDelay::ViaDelayUI::button6EnterMenuCallback(void) {

	/// Executed after a press is detected at button 6 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaDelay::ViaDelayUI::aux1EnterMenuCallback(void) {

	/// Executed after a press is detected at button 1 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaDelay::ViaDelayUI::aux2EnterMenuCallback(void) {

	/// Executed after a press is detected at button 3 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaDelay::ViaDelayUI::aux2AltEnterMenuCallback(void) {

	/// Executed after a press is detected at button 2 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaDelay::ViaDelayUI::aux3EnterMenuCallback(void) {

	/// Executed after a press is detected at button 4 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaDelay::ViaDelayUI::aux4EnterMenuCallback(void) {

	/// Executed after a press is detected at button 6 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}


/// Executed after exiting a button menu with a release event in less than half a second (roughly)

void ViaDelay::ViaDelayUI::button1TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}
void ViaDelay::ViaDelayUI::button2TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}
void ViaDelay::ViaDelayUI::button3TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}
void ViaDelay::ViaDelayUI::button4TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}
void ViaDelay::ViaDelayUI::button5TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}
void ViaDelay::ViaDelayUI::button6TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}

void ViaDelay::ViaDelayUI::aux1TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}

void ViaDelay::ViaDelayUI::aux2TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}

void ViaDelay::ViaDelayUI::aux2AltTapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}

void ViaDelay::ViaDelayUI::aux3TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}

void ViaDelay::ViaDelayUI::aux4TapCallback(void) {

	transition(&ViaDelay::ViaDelayUI::newModeMenu);

}


/// Executed after exiting a button menu with a release event in more than half a second (roughly)

void ViaDelay::ViaDelayUI::button1HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}
void ViaDelay::ViaDelayUI::button2HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}
void ViaDelay::ViaDelayUI::button3HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}
void ViaDelay::ViaDelayUI::button4HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}
void ViaDelay::ViaDelayUI::button5HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}
void ViaDelay::ViaDelayUI::button6HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::defaultMenu);

}

void ViaDelay::ViaDelayUI::aux1HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::button5Menu);

}

void ViaDelay::ViaDelayUI::aux2HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::button5Menu);

}

void ViaDelay::ViaDelayUI::aux2AltHoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::button5Menu);

}

void ViaDelay::ViaDelayUI::aux3HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::button5Menu);

}

void ViaDelay::ViaDelayUI::aux4HoldCallback(void) {

	transition(&ViaDelay::ViaDelayUI::button5Menu);

}


