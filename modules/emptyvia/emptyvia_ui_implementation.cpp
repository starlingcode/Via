/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include <emptyvia.hpp>
#include "user_interface.hpp"

/// Link to the STM32 Touch Sense Library.
void emptyviaTouchLink (void * uiVoid) {
	ViaEmptyvia::ViaEmptyviaUI * ui = (ViaEmptyvia::ViaEmptyviaUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

// Used to restore the module's state after power cycle or during preset recall
void ViaEmptyvia::ViaEmptyviaUI::recallModuleState(void) {

	button1Mode %= this_module.numButton1Modes;
	button2Mode %= this_module.numButton2Modes;
	button3Mode %= this_module.numButton3Modes;
	button4Mode %= this_module.numButton4Modes;
	button5Mode %= this_module.numButton5Modes;
	button6Mode %= this_module.numButton6Modes;
	aux1Mode %= this_module.numAux1Modes;
	aux2Mode %= this_module.numAux2Modes;
	aux3Mode %= this_module.numAux3Modes;
	aux4Mode %= this_module.numAux4Modes;

	this_module.handleButton1ModeChange(button1Mode);
	this_module.handleButton2ModeChange(button2Mode);
	this_module.handleButton3ModeChange(button3Mode);
	this_module.handleButton4ModeChange(button4Mode);
	this_module.handleButton5ModeChange(button5Mode);
	this_module.handleButton6ModeChange(button6Mode);
	this_module.handleAux1ModeChange(aux1Mode);
	this_module.handleAux2ModeChange(aux2Mode);
	this_module.handleAux3ModeChange(aux3Mode);
	this_module.handleAux4ModeChange(aux4Mode);

}

/// Executed when the UI returns to the "default mode"

void ViaEmptyvia::ViaEmptyviaUI::defaultEnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.runtimeDisplay = 1;
}

/// Button handlers

/// Enter Menu called when touch detected from default menu
/// Tap called when sensor released after < 1 second
/// Hold called when sensor released after > 1 second


void ViaEmptyvia::ViaEmptyviaUI::button1EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button1Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::button1TapCallback(void) {
	button1Mode = incrementModeAndStore(button1Mode, BUTTON1_MASK, this_module.numButton1Modes, BUTTON1_SHIFT);
	this_module.handleButton1ModeChange(button1Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button1Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::button1HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

void ViaEmptyvia::ViaEmptyviaUI::button2EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button2Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::button2TapCallback(void) {
	button2Mode = incrementModeAndStore(button2Mode, BUTTON2_MASK, this_module.numButton2Modes, BUTTON2_SHIFT);
	this_module.handleButton2ModeChange(button2Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button2Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::button2HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

void ViaEmptyvia::ViaEmptyviaUI::button3EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button3Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::button3TapCallback(void) {
	button3Mode = incrementModeAndStore(button3Mode, BUTTON3_MASK, this_module.numButton3Modes, BUTTON3_SHIFT);
	this_module.handleButton3ModeChange(button3Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button3Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::button3HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

void ViaEmptyvia::ViaEmptyviaUI::button4EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button4Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::button4TapCallback(void) {
	button4Mode = incrementModeAndStore(button4Mode, BUTTON4_MASK, this_module.numButton4Modes, BUTTON4_SHIFT);
	this_module.handleButton4ModeChange(button4Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button4Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::button4HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

void ViaEmptyvia::ViaEmptyviaUI::button5EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button5Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::button5TapCallback(void) {
	button5Mode = incrementModeAndStore(button5Mode, BUTTON5_MASK, this_module.numButton5Modes, BUTTON5_SHIFT);
	this_module.handleButton5ModeChange(button5Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button5Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::button5HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

void ViaEmptyvia::ViaEmptyviaUI::button6EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(button6Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::button6TapCallback(void) {
	button6Mode = incrementModeAndStore(button6Mode, BUTTON6_MASK, this_module.numButton6Modes, BUTTON6_SHIFT);
	this_module.handleButton6ModeChange(button6Mode);
	this_module.clearLEDs();
	this_module.setLEDs(button6Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::button6HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

// Aux modes if you neeeeeeed them

void ViaEmptyvia::ViaEmptyviaUI::aux1EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(aux1Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::aux1TapCallback(void) {
	aux1Mode = incrementModeAndStore(aux1Mode, AUX_MODE1_MASK, this_module.numAux1Modes, AUX_MODE1_SHIFT);
	this_module.handleButton6ModeChange(aux1Mode);
	this_module.clearLEDs();
	this_module.setLEDs(aux1Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::aux1HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

void ViaEmptyvia::ViaEmptyviaUI::aux2EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(aux2Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::aux2TapCallback(void) {
	aux2Mode = incrementModeAndStore(aux2Mode, AUX_MODE2_MASK, this_module.numAux2Modes, AUX_MODE2_SHIFT);
	this_module.handleButton6ModeChange(aux2Mode);
	this_module.clearLEDs();
	this_module.setLEDs(aux2Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::aux2HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

void ViaEmptyvia::ViaEmptyviaUI::aux3EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(aux3Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::aux3TapCallback(void) {
	aux3Mode = incrementModeAndStore(aux3Mode, AUX_MODE3_MASK, this_module.numAux3Modes, AUX_MODE3_SHIFT);
	this_module.handleButton6ModeChange(aux3Mode);
	this_module.clearLEDs();
	this_module.setLEDs(aux3Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::aux3HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}

void ViaEmptyvia::ViaEmptyviaUI::aux4EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(aux4Mode);
	resetTimerMenu();
}
void ViaEmptyvia::ViaEmptyviaUI::aux4TapCallback(void) {
	aux4Mode = incrementModeAndStore(aux4Mode, AUX_MODE4_MASK, this_module.numAux4Modes, AUX_MODE4_SHIFT);
	this_module.handleButton6ModeChange(aux4Mode);
	this_module.clearLEDs();
	this_module.setLEDs(aux4Mode);
	transition(&ViaEmptyvia::ViaEmptyviaUI::newModeMenu);
}
void ViaEmptyvia::ViaEmptyviaUI::aux4HoldCallback(void) {
	transition(&ViaEmptyvia::ViaEmptyviaUI::defaultMenu);
}
