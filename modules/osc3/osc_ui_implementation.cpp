/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include <osc.hpp>
#include "user_interface.hpp"

void ViaOsc::ViaOscUI::initialize(void) {

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

void oscTouchLink (void * uiVoid) {
	ViaOsc::ViaOscUI * ui = (ViaOsc::ViaOscUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaOsc::ViaOscUI::recallModuleState(void) {

	button1Mode %= this_module.numButton1Modes;
	button2Mode %= this_module.numButton2Modes;
	button3Mode %= this_module.numButton3Modes;
	button4Mode %= this_module.numButton4Modes;
	button5Mode %= this_module.numButton5Modes;
	button6Mode %= this_module.numButton6Modes;

	this_module.handleButton1ModeChange(button1Mode);
	this_module.handleButton2ModeChange(button2Mode);
	this_module.handleButton3ModeChange(button3Mode);
//	this_module.handleButton4ModeChange(button4Mode);
	this_module.handleButton5ModeChange(button5Mode);
	this_module.handleButton6ModeChange(button6Mode);

}

void ViaOsc::ViaOscUI::uiSetLEDs(int mode) {

	/// This connects the UI library's LED setting calls to the enumerated LED handler of the this_module.
	/// This could probably go away with some refactoring.

	this_module.setLEDs(mode);

}

void ViaOsc::ViaOscUI::defaultEnterMenuCallback(void) {

	/// Executed when the module returns to an idle state, waiting for a touch event
	this_module.runtimeDisplay = 1;
	this_module.clearLEDs();
	this_module.setLEDD(this_module.chordMode);

}
void ViaOsc::ViaOscUI::newModeEnterMenuCallback(void) {

//	transition(&ViaOsc::ViaOscUI::defaultMenu);

}
void ViaOsc::ViaOscUI::newAuxModeEnterMenuCallback(void) {

	transition(&ViaOsc::ViaOscUI::defaultMenu);

}
void ViaOsc::ViaOscUI::presetEnterMenuCallback(void) {

	transition(&ViaOsc::ViaOscUI::defaultMenu);

}

void ViaOsc::ViaOscUI::button1EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button1Mode);
	resetTimerMenu();

}
void ViaOsc::ViaOscUI::button2EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button2Mode);
	resetTimerMenu();

}
void ViaOsc::ViaOscUI::button3EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button3Mode);
	resetTimerMenu();

}
void ViaOsc::ViaOscUI::button4EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button1Mode);
	resetTimerMenu();

}
void ViaOsc::ViaOscUI::button5EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.showQuant();
	this_module.setLEDs(button5Mode);
	resetTimerMenu();

}
void ViaOsc::ViaOscUI::button6EnterMenuCallback(void) {

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button6Mode);
	resetTimerMenu();
}
void ViaOsc::ViaOscUI::aux1EnterMenuCallback(void) {

	/// Executed after a press is detected at button 1 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaOsc::ViaOscUI::aux2EnterMenuCallback(void) {

	/// Executed after a press is detected at button 3 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaOsc::ViaOscUI::aux2AltEnterMenuCallback(void) {

	/// Executed after a press is detected at button 2 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}

void ViaOsc::ViaOscUI::aux3EnterMenuCallback(void) {

	/// Executed after a press is detected at button 4 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}
void ViaOsc::ViaOscUI::aux4EnterMenuCallback(void) {

	/// Executed after a press is detected at button 6 from the shift state
	this_module.runtimeDisplay = 0;
	resetTimerMenu();

}


/// Executed after exiting a button menu with a release event in less than half a second (roughly)

void ViaOsc::ViaOscUI::button1TapCallback(void) {

	button1Mode = button1Mode > 4 ? 5 : button1Mode + 1;
	storeMode(button1Mode, BUTTON1_MASK, BUTTON1_SHIFT);
	this_module.handleButton1ModeChange(button1Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button1Mode);
	transition(&ViaOsc::ViaOscUI::newModeMenu);

}
void ViaOsc::ViaOscUI::button2TapCallback(void) {

	button2Mode = incrementModeAndStore(button2Mode, BUTTON2_MASK, this_module.numButton2Modes, BUTTON2_SHIFT);
	this_module.handleButton2ModeChange(button2Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button2Mode);
	transition(&ViaOsc::ViaOscUI::newModeMenu);

}
void ViaOsc::ViaOscUI::button3TapCallback(void) {

	button3Mode = incrementModeAndStore(button3Mode, BUTTON3_MASK, this_module.numButton3Modes, BUTTON3_SHIFT);
	this_module.handleButton3ModeChange(button3Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button3Mode);
	transition(&ViaOsc::ViaOscUI::newModeMenu);

}
void ViaOsc::ViaOscUI::button4TapCallback(void) {

	button1Mode = button1Mode <= 0 ? 0 : button1Mode - 1;
	storeMode(button1Mode, BUTTON1_MASK, BUTTON1_SHIFT);
	this_module.handleButton1ModeChange(button1Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button1Mode);
	transition(&ViaOsc::ViaOscUI::newModeMenu);

}
void ViaOsc::ViaOscUI::button5TapCallback(void) {

	button5Mode = incrementModeAndStore(button5Mode, BUTTON5_MASK, this_module.numButton5Modes, BUTTON5_SHIFT);
	this_module.handleButton5ModeChange(button5Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button5Mode);
	this_module.showQuant();
	transition(&ViaOsc::ViaOscUI::newModeMenu);

}
void ViaOsc::ViaOscUI::button6TapCallback(void) {

	button6Mode = incrementModeAndStore(button6Mode, BUTTON6_MASK, this_module.numButton6Modes, BUTTON6_SHIFT);
	this_module.handleButton6ModeChange(button6Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button6Mode);
	transition(&ViaOsc::ViaOscUI::newModeMenu);

}

void ViaOsc::ViaOscUI::aux1TapCallback(void) {

	transition(&ViaOsc::ViaOscUI::newModeMenu);

}

void ViaOsc::ViaOscUI::aux2TapCallback(void) {

	transition(&ViaOsc::ViaOscUI::newModeMenu);

}

void ViaOsc::ViaOscUI::aux2AltTapCallback(void) {

	transition(&ViaOsc::ViaOscUI::newModeMenu);

}

void ViaOsc::ViaOscUI::aux3TapCallback(void) {

	transition(&ViaOsc::ViaOscUI::newModeMenu);

}

void ViaOsc::ViaOscUI::aux4TapCallback(void) {

	transition(&ViaOsc::ViaOscUI::newModeMenu);

}


/// Executed after exiting a button menu with a release event in more than half a second (roughly)

void ViaOsc::ViaOscUI::button1HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::defaultMenu);

}
void ViaOsc::ViaOscUI::button2HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::defaultMenu);

}
void ViaOsc::ViaOscUI::button3HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::defaultMenu);

}
void ViaOsc::ViaOscUI::button4HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::defaultMenu);

}
void ViaOsc::ViaOscUI::button5HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::defaultMenu);

}
void ViaOsc::ViaOscUI::button6HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::defaultMenu);

}

void ViaOsc::ViaOscUI::aux1HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::button5Menu);

}

void ViaOsc::ViaOscUI::aux2HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::button5Menu);

}

void ViaOsc::ViaOscUI::aux2AltHoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::button5Menu);

}

void ViaOsc::ViaOscUI::aux3HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::button5Menu);

}

void ViaOsc::ViaOscUI::aux4HoldCallback(void) {

	transition(&ViaOsc::ViaOscUI::button5Menu);

}


