/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include <sinebeat.hpp>
#include "user_interface.hpp"

void ViaSinebeat::ViaSinebeatUI::initialize(void) {

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

void sinebeatTouchLink (void * uiVoid) {
	ViaSinebeat::ViaSinebeatUI * ui = (ViaSinebeat::ViaSinebeatUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaSinebeat::ViaSinebeatUI::recallModuleState(void) {

	/// Put your code to initialze the module to a given state here
	this_module.handleSineBeatChange(button2Mode);

}

void ViaSinebeat::ViaSinebeatUI::uiSetLEDs(int mode) {

	/// This connects the UI library's LED setting calls to the enumerated LED handler of the module.
	/// This could probably go away with some refactoring.

	this_module.setLEDs(mode);

}

void ViaSinebeat::ViaSinebeatUI::defaultEnterMenuCallback(void) {

	/// Executed when the module returns to an idle state, waiting for a touch event
	this_module.clearLEDs();
	this_module.runtimeDisplay = 1;

}
void ViaSinebeat::ViaSinebeatUI::newModeEnterMenuCallback(void) {

	;

}
void ViaSinebeat::ViaSinebeatUI::newAuxModeEnterMenuCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::defaultMenu);

}
void ViaSinebeat::ViaSinebeatUI::presetEnterMenuCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::defaultMenu);

}

void ViaSinebeat::ViaSinebeatUI::button1EnterMenuCallback(void) {

	/// Executed after a press is detected at button 1 from the idle state
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button1Mode);
	resetTimerMenu();

}
void ViaSinebeat::ViaSinebeatUI::button2EnterMenuCallback(void) {

	/// Executed after a press is detected at button 2 from the idle state
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button2Mode);
	resetTimerMenu();

}
void ViaSinebeat::ViaSinebeatUI::button3EnterMenuCallback(void) {

	/// Executed after a press is detected at button 3 from the idle state
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button3Mode);
	resetTimerMenu();

}
void ViaSinebeat::ViaSinebeatUI::button4EnterMenuCallback(void) {

	/// Executed after a press is detected at button 4 from the idle state
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button4Mode);
	resetTimerMenu();

}
void ViaSinebeat::ViaSinebeatUI::button5EnterMenuCallback(void) {

	/// Executed after a press is detected at button 5 from the idle state
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button2Mode);
	resetTimerMenu();

}
void ViaSinebeat::ViaSinebeatUI::button6EnterMenuCallback(void) {

	/// Executed after a press is detected at button 6 from the idle state
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button6Mode);
	resetTimerMenu();

}
void ViaSinebeat::ViaSinebeatUI::aux1EnterMenuCallback(void) {

	/// Executed after a press is detected at button 1 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaSinebeat::ViaSinebeatUI::aux2EnterMenuCallback(void) {

	/// Executed after a press is detected at button 3 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaSinebeat::ViaSinebeatUI::aux2AltEnterMenuCallback(void) {

	/// Executed after a press is detected at button 2 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaSinebeat::ViaSinebeatUI::aux3EnterMenuCallback(void) {

	/// Executed after a press is detected at button 4 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaSinebeat::ViaSinebeatUI::aux4EnterMenuCallback(void) {

	/// Executed after a press is detected at button 6 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}


/// Executed after exiting a button menu with a release event in less than half a second (roughly)

void ViaSinebeat::ViaSinebeatUI::button1TapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}
void ViaSinebeat::ViaSinebeatUI::button2TapCallback(void) {

	button2Mode = incrementModeAndStore(button2Mode, BUTTON2_MASK, 4, BUTTON2_SHIFT);
	this_module.handleSineBeatChange(button2Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button2Mode);
	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}
void ViaSinebeat::ViaSinebeatUI::button3TapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}
void ViaSinebeat::ViaSinebeatUI::button4TapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}
void ViaSinebeat::ViaSinebeatUI::button5TapCallback(void) {

	button2Mode = incrementModeAndStore(button2Mode, BUTTON2_MASK, 4, BUTTON2_SHIFT);
	this_module.handleSineBeatChange(button2Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button2Mode);
	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}
void ViaSinebeat::ViaSinebeatUI::button6TapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}

void ViaSinebeat::ViaSinebeatUI::aux1TapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}

void ViaSinebeat::ViaSinebeatUI::aux2TapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}

void ViaSinebeat::ViaSinebeatUI::aux2AltTapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}

void ViaSinebeat::ViaSinebeatUI::aux3TapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}

void ViaSinebeat::ViaSinebeatUI::aux4TapCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::newModeMenu);

}


/// Executed after exiting a button menu with a release event in more than half a second (roughly)

void ViaSinebeat::ViaSinebeatUI::button1HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::defaultMenu);

}
void ViaSinebeat::ViaSinebeatUI::button2HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::defaultMenu);

}
void ViaSinebeat::ViaSinebeatUI::button3HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::defaultMenu);

}
void ViaSinebeat::ViaSinebeatUI::button4HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::defaultMenu);

}
void ViaSinebeat::ViaSinebeatUI::button5HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::defaultMenu);

}
void ViaSinebeat::ViaSinebeatUI::button6HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::defaultMenu);

}

void ViaSinebeat::ViaSinebeatUI::aux1HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::button5Menu);

}

void ViaSinebeat::ViaSinebeatUI::aux2HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::button5Menu);

}

void ViaSinebeat::ViaSinebeatUI::aux2AltHoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::button5Menu);

}

void ViaSinebeat::ViaSinebeatUI::aux3HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::button5Menu);

}

void ViaSinebeat::ViaSinebeatUI::aux4HoldCallback(void) {

	transition(&ViaSinebeat::ViaSinebeatUI::button5Menu);

}


