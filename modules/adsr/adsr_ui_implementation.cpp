/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include <adsr.hpp>
#include "user_interface.hpp"

void ViaAdsr::ViaAdsrUI::initialize(void) {

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

void adsrTouchLink (void * uiVoid) {
	ViaAdsr::ViaAdsrUI * ui = (ViaAdsr::ViaAdsrUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaAdsr::ViaAdsrUI::recallModuleState(void) {

	/// Put your code to initialze the module to a given state here

}

void ViaAdsr::ViaAdsrUI::uiSetLEDs(int mode) {

	/// This connects the UI library's LED setting calls to the enumerated LED handler of the this_module.
	/// This could probably go away with some refactoring.

	this_module.setLEDs(mode);

}

void ViaAdsr::ViaAdsrUI::defaultEnterMenuCallback(void) {

	/// Executed when the module returns to an idle state, waiting for a touch event
	this_module.runtimeDisplay = 1;

}
void ViaAdsr::ViaAdsrUI::newModeEnterMenuCallback(void) {

//	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}
void ViaAdsr::ViaAdsrUI::newAuxModeEnterMenuCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}
void ViaAdsr::ViaAdsrUI::presetEnterMenuCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}

void ViaAdsr::ViaAdsrUI::button1EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button1Mode);
	resetTimerMenu();

}
void ViaAdsr::ViaAdsrUI::button2EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button2Mode);
	resetTimerMenu();

}
void ViaAdsr::ViaAdsrUI::button3EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button3Mode);
	resetTimerMenu();

}
void ViaAdsr::ViaAdsrUI::button4EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button4Mode);
	resetTimerMenu();

}
void ViaAdsr::ViaAdsrUI::button5EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button5Mode);
	resetTimerMenu();

}
void ViaAdsr::ViaAdsrUI::button6EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button6Mode);
	resetTimerMenu();
}
void ViaAdsr::ViaAdsrUI::aux1EnterMenuCallback(void) {

	/// Executed after a press is detected at button 1 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaAdsr::ViaAdsrUI::aux2EnterMenuCallback(void) {

	/// Executed after a press is detected at button 3 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaAdsr::ViaAdsrUI::aux2AltEnterMenuCallback(void) {

	/// Executed after a press is detected at button 2 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaAdsr::ViaAdsrUI::aux3EnterMenuCallback(void) {

	/// Executed after a press is detected at button 4 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaAdsr::ViaAdsrUI::aux4EnterMenuCallback(void) {

	/// Executed after a press is detected at button 6 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}


/// Executed after exiting a button menu with a release event in less than half a second (roughly)

void ViaAdsr::ViaAdsrUI::button1TapCallback(void) {

	button1Mode = incrementModeAndStore(button1Mode, BUTTON1_MASK, this_module.numButton1Modes, BUTTON1_SHIFT);
	this_module.handleButton1ModeChange(button1Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button1Mode);
	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}
void ViaAdsr::ViaAdsrUI::button2TapCallback(void) {

	button2Mode = incrementModeAndStore(button2Mode, BUTTON2_MASK, this_module.numButton2Modes, BUTTON2_SHIFT);
	this_module.handleButton2ModeChange(button2Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button2Mode);
	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}
void ViaAdsr::ViaAdsrUI::button3TapCallback(void) {

	button3Mode = incrementModeAndStore(button3Mode, BUTTON3_MASK, this_module.numButton3Modes, BUTTON3_SHIFT);
	this_module.handleButton3ModeChange(button3Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button3Mode);
	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}
void ViaAdsr::ViaAdsrUI::button4TapCallback(void) {

	button4Mode = incrementModeAndStore(button4Mode, BUTTON4_MASK, this_module.numButton4Modes, BUTTON4_SHIFT);
	this_module.handleButton4ModeChange(button4Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button4Mode);
	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}
void ViaAdsr::ViaAdsrUI::button5TapCallback(void) {

	button5Mode = incrementModeAndStore(button5Mode, BUTTON5_MASK, this_module.numButton5Modes, BUTTON5_SHIFT);
	this_module.handleButton5ModeChange(button5Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button5Mode);
	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}
void ViaAdsr::ViaAdsrUI::button6TapCallback(void) {

	button6Mode = incrementModeAndStore(button6Mode, BUTTON6_MASK, this_module.numButton6Modes, BUTTON6_SHIFT);
	this_module.handleButton6ModeChange(button6Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button6Mode);
	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}

void ViaAdsr::ViaAdsrUI::aux1TapCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}

void ViaAdsr::ViaAdsrUI::aux2TapCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}

void ViaAdsr::ViaAdsrUI::aux2AltTapCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}

void ViaAdsr::ViaAdsrUI::aux3TapCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}

void ViaAdsr::ViaAdsrUI::aux4TapCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::newModeMenu);

}


/// Executed after exiting a button menu with a release event in more than half a second (roughly)

void ViaAdsr::ViaAdsrUI::button1HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}
void ViaAdsr::ViaAdsrUI::button2HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}
void ViaAdsr::ViaAdsrUI::button3HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}
void ViaAdsr::ViaAdsrUI::button4HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}
void ViaAdsr::ViaAdsrUI::button5HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}
void ViaAdsr::ViaAdsrUI::button6HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::defaultMenu);

}

void ViaAdsr::ViaAdsrUI::aux1HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::button5Menu);

}

void ViaAdsr::ViaAdsrUI::aux2HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::button5Menu);

}

void ViaAdsr::ViaAdsrUI::aux2AltHoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::button5Menu);

}

void ViaAdsr::ViaAdsrUI::aux3HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::button5Menu);

}

void ViaAdsr::ViaAdsrUI::aux4HoldCallback(void) {

	transition(&ViaAdsr::ViaAdsrUI::button5Menu);

}


