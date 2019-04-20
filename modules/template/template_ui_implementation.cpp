/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include "template.hpp"
#include "user_interface.hpp"

void ViaTemplate::ViaTemplateUI::initialize(void) {

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

void templateTouchLink (void * uiVoid) {
	ViaTemplate::ViaTemplateUI * ui = (ViaTemplate::ViaTemplateUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaTemplate::ViaTemplateUI::recallModuleState(void) {

	/// Put your code to initialze the module to a given state here

}

void ViaTemplate::ViaTemplateUI::uiSetLEDs(int mode) {

	/// This connects the UI library's LED setting calls to the enumerated LED handler of the module.
	/// This could probably go away with some refactoring.

	this_module.setLEDs(mode);

}

void ViaTemplate::ViaTemplateUI::defaultEnterMenuCallback(void) {

	/// Executed when the module returns to an idle state, waiting for a touch event
	this_module.runtimeDisplay = 1;

}
void ViaTemplate::ViaTemplateUI::newModeEnterMenuCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}
void ViaTemplate::ViaTemplateUI::newAuxModeEnterMenuCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}
void ViaTemplate::ViaTemplateUI::presetEnterMenuCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}

void ViaTemplate::ViaTemplateUI::button1EnterMenuCallback(void) {

	/// Executed after a press is detected at button 1 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaTemplate::ViaTemplateUI::button2EnterMenuCallback(void) {

	/// Executed after a press is detected at button 2 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaTemplate::ViaTemplateUI::button3EnterMenuCallback(void) {

	/// Executed after a press is detected at button 3 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaTemplate::ViaTemplateUI::button4EnterMenuCallback(void) {

	/// Executed after a press is detected at button 4 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaTemplate::ViaTemplateUI::button5EnterMenuCallback(void) {

	/// Executed after a press is detected at button 5 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaTemplate::ViaTemplateUI::button6EnterMenuCallback(void) {

	/// Executed after a press is detected at button 6 from the idle state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaTemplate::ViaTemplateUI::aux1EnterMenuCallback(void) {

	/// Executed after a press is detected at button 1 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaTemplate::ViaTemplateUI::aux2EnterMenuCallback(void) {

	/// Executed after a press is detected at button 3 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaTemplate::ViaTemplateUI::aux2AltEnterMenuCallback(void) {

	/// Executed after a press is detected at button 2 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaTemplate::ViaTemplateUI::aux3EnterMenuCallback(void) {

	/// Executed after a press is detected at button 4 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaTemplate::ViaTemplateUI::aux4EnterMenuCallback(void) {

	/// Executed after a press is detected at button 6 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}


/// Executed after exiting a button menu with a release event in less than half a second (roughly)

void ViaTemplate::ViaTemplateUI::button1TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}
void ViaTemplate::ViaTemplateUI::button2TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}
void ViaTemplate::ViaTemplateUI::button3TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}
void ViaTemplate::ViaTemplateUI::button4TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}
void ViaTemplate::ViaTemplateUI::button5TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}
void ViaTemplate::ViaTemplateUI::button6TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}

void ViaTemplate::ViaTemplateUI::aux1TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}

void ViaTemplate::ViaTemplateUI::aux2TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}

void ViaTemplate::ViaTemplateUI::aux2AltTapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}

void ViaTemplate::ViaTemplateUI::aux3TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}

void ViaTemplate::ViaTemplateUI::aux4TapCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::newModeMenu);

}


/// Executed after exiting a button menu with a release event in more than half a second (roughly)

void ViaTemplate::ViaTemplateUI::button1HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}
void ViaTemplate::ViaTemplateUI::button2HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}
void ViaTemplate::ViaTemplateUI::button3HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}
void ViaTemplate::ViaTemplateUI::button4HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}
void ViaTemplate::ViaTemplateUI::button5HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}
void ViaTemplate::ViaTemplateUI::button6HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::defaultMenu);

}

void ViaTemplate::ViaTemplateUI::aux1HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::button5Menu);

}

void ViaTemplate::ViaTemplateUI::aux2HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::button5Menu);

}

void ViaTemplate::ViaTemplateUI::aux2AltHoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::button5Menu);

}

void ViaTemplate::ViaTemplateUI::aux3HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::button5Menu);

}

void ViaTemplate::ViaTemplateUI::aux4HoldCallback(void) {

	transition(&ViaTemplate::ViaTemplateUI::button5Menu);

}


