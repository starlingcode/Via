/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include "sync.hpp"
#include "user_interface.hpp"

void ViaSync::ViaSyncUI::initialize(void) {

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

void syncTouchLink (void * uiVoid) {
	ViaSync::ViaSyncUI * ui = (ViaSync::ViaSyncUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaSync::ViaSyncUI::recallModuleState(void) {

	this_module.syncUI.button1Mode %= numButton1Modes;
	this_module.syncUI.button2Mode %= numButton2Modes;
	this_module.syncUI.button3Mode %= numButton3Modes;
	this_module.syncUI.button4Mode %= numButton4Modes;
	this_module.syncUI.button5Mode %= numButton5Modes;
	this_module.syncUI.button6Mode %= numButton6Modes;
	this_module.syncUI.aux1Mode %= numAux1Modes;
	this_module.syncUI.aux2Mode %= numAux2Modes;
	this_module.syncUI.aux3Mode %= numAux3Modes;
	this_module.syncUI.aux4Mode %= numAux4Modes;

	this_module.handleAux3ModeChange(this_module.syncUI.aux3Mode);
	this_module.handleButton1ModeChange(this_module.syncUI.button1Mode);
	this_module.handleButton2ModeChange(this_module.syncUI.button2Mode);
	this_module.handleButton3ModeChange(this_module.syncUI.button3Mode);
	this_module.handleButton4ModeChange(this_module.syncUI.button4Mode);
	//this_module.handleButton5ModeInit(this_module.syncUI.button5Mode);
	this_module.handleButton6ModeChange(this_module.syncUI.button6Mode);
	this_module.handleAux1ModeChange(this_module.syncUI.aux1Mode);
	this_module.handleAux2ModeChange(this_module.syncUI.aux2Mode);
	this_module.handleAux4ModeChange(this_module.syncUI.aux4Mode);

}

void ViaSync::ViaSyncUI::uiSetLEDs(int mode) {
	this_module.setLEDs(mode);
}

void ViaSync::ViaSyncUI::defaultEnterMenuCallback(void) {
	this_module.clearLEDs();
	if (button3Mode == 1) {
		this_module.setLEDD(0);
	} else {
		this_module.setLEDD(1);
	}
	this_module.runtimeDisplay = 1;
}
void ViaSync::ViaSyncUI::newModeEnterMenuCallback(void) {
	;
}
void ViaSync::ViaSyncUI::newAuxModeEnterMenuCallback(void) {
	;
}
void ViaSync::ViaSyncUI::presetEnterMenuCallback(void) {;
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
}

void ViaSync::ViaSyncUI::button1EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(SH_MODE);
	resetTimerMenu();
}
void ViaSync::ViaSyncUI::button2EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(SCALE_MODE);
	this_module.updateRGBDisplay(this_module.scaleColor.r, this_module.scaleColor.g, this_module.scaleColor.b, 1);
	resetTimerMenu();
}
void ViaSync::ViaSyncUI::button3EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(X_MODE);
	resetTimerMenu();
}
void ViaSync::ViaSyncUI::button4EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(SYNC_MODE);
	resetTimerMenu();
}
void ViaSync::ViaSyncUI::button5EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.updateRGBDisplay(this_module.scaleColor.r, this_module.scaleColor.g, this_module.scaleColor.b, 1);
	this_module.setLEDs(GROUP_MODE);
	resetTimerMenu();
}
void ViaSync::ViaSyncUI::button6EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(TABLE_MODE);
	resetTimerMenu();
}
void ViaSync::ViaSyncUI::aux1EnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.setLEDs(LOGIC_A_MODE);
	resetTimerMenu();
}
void ViaSync::ViaSyncUI::aux2EnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.setLEDs(QUADRATURE_MODE);
	resetTimerMenu();
}

void ViaSync::ViaSyncUI::aux2AltEnterMenuCallback(void) {
	transition(&ViaSync::ViaSyncUI::button5Menu);
}
void ViaSync::ViaSyncUI::aux3EnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.setLEDs(AUX_OUT_MODE);
	resetTimerMenu();
}
void ViaSync::ViaSyncUI::aux4EnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.setLEDs(TABLE_GROUP_MODE);
	resetTimerMenu();
}

void ViaSync::ViaSyncUI::button1TapCallback(void) {
	SH_MODE = incrementModeAndStore(SH_MODE, BUTTON1_MASK, numButton1Modes, BUTTON1_SHIFT);
	this_module.handleButton1ModeChange(SH_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(SH_MODE);
	transition(&ViaSync::ViaSyncUI::newModeMenu);
}
void ViaSync::ViaSyncUI::button2TapCallback(void) {
	SCALE_MODE = incrementModeAndStore(SCALE_MODE, BUTTON2_MASK, numButton2Modes, BUTTON2_SHIFT);
	this_module.handleButton2ModeChange(SCALE_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(SCALE_MODE);
	transition(&ViaSync::ViaSyncUI::newModeMenu);
}
void ViaSync::ViaSyncUI::button3TapCallback(void) {
	X_MODE = incrementModeAndStore(X_MODE, BUTTON3_MASK, numButton3Modes, BUTTON3_SHIFT);
	this_module.handleButton3ModeChange(X_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(X_MODE);
	transition(&ViaSync::ViaSyncUI::newModeMenu);
}
void ViaSync::ViaSyncUI::button4TapCallback(void) {
	SYNC_MODE = incrementModeAndStore(SYNC_MODE, BUTTON4_MASK, numButton4Modes, BUTTON4_SHIFT);
	this_module.handleButton4ModeChange(SYNC_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(SYNC_MODE);
	transition(&ViaSync::ViaSyncUI::newModeMenu);
}
void ViaSync::ViaSyncUI::button5TapCallback(void) {
	GROUP_MODE = incrementModeAndStore(GROUP_MODE, BUTTON5_MASK, numButton5Modes, BUTTON5_SHIFT);
	this_module.handleButton5ModeChange(GROUP_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(GROUP_MODE);
	transition(&ViaSync::ViaSyncUI::newModeMenu);
}
void ViaSync::ViaSyncUI::button6TapCallback(void) {
	TABLE_MODE = incrementModeAndStore(TABLE_MODE, BUTTON6_MASK, numButton6Modes, BUTTON6_SHIFT);
	this_module.handleButton6ModeChange(TABLE_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(TABLE_MODE);
	transition(&ViaSync::ViaSyncUI::newModeMenu);
}

void ViaSync::ViaSyncUI::aux1TapCallback(void) {
	LOGIC_A_MODE = incrementModeAndStore(LOGIC_A_MODE, AUX_MODE1_MASK, numAux1Modes, AUX_MODE1_SHIFT);
	this_module.handleAux1ModeChange(LOGIC_A_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(LOGIC_A_MODE);
	transition(&ViaSync::ViaSyncUI::newAuxModeMenu);
}

void ViaSync::ViaSyncUI::aux2TapCallback(void) {
	QUADRATURE_MODE = incrementModeAndStore(QUADRATURE_MODE, AUX_MODE2_MASK, numAux2Modes, AUX_MODE2_SHIFT);
	this_module.handleAux2ModeChange(QUADRATURE_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(QUADRATURE_MODE);
	transition(&ViaSync::ViaSyncUI::newAuxModeMenu);
}

void ViaSync::ViaSyncUI::aux2AltTapCallback(void) {
	transition(&ViaSync::ViaSyncUI::button5Menu);
}

void ViaSync::ViaSyncUI::aux3TapCallback(void) {
	AUX_OUT_MODE = incrementModeAndStore(AUX_OUT_MODE, AUX_MODE3_MASK, numAux3Modes, AUX_MODE3_SHIFT);
	this_module.handleAux3ModeChange(AUX_OUT_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(AUX_OUT_MODE);
	transition(&ViaSync::ViaSyncUI::newModeMenu);
}

void ViaSync::ViaSyncUI::aux4TapCallback(void) {
	TABLE_GROUP_MODE = incrementModeAndStore(TABLE_GROUP_MODE, AUX_MODE4_MASK, numAux4Modes, AUX_MODE4_SHIFT);
	this_module.handleAux4ModeChange(TABLE_GROUP_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(TABLE_GROUP_MODE);
	transition(&ViaSync::ViaSyncUI::newAuxModeMenu);
}

void ViaSync::ViaSyncUI::button1HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}
void ViaSync::ViaSyncUI::button2HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}
void ViaSync::ViaSyncUI::button3HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}
void ViaSync::ViaSyncUI::button4HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}
void ViaSync::ViaSyncUI::button5HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}
void ViaSync::ViaSyncUI::button6HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}

void ViaSync::ViaSyncUI::aux1HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}

void ViaSync::ViaSyncUI::aux2HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}

void ViaSync::ViaSyncUI::aux2AltHoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}

void ViaSync::ViaSyncUI::aux3HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}

void ViaSync::ViaSyncUI::aux4HoldCallback(void) {
	transition(&ViaSync::ViaSyncUI::defaultMenu);
}

void ViaSync::ViaSyncUI::writeStockPresets(void) {

	storeToEEPROM(1, stockPreset1);
	storeToEEPROM(2, stockPreset2);
	storeToEEPROM(3, stockPreset3);
	storeToEEPROM(4, stockPreset4);
	storeToEEPROM(5, stockPreset5);
	storeToEEPROM(6, stockPreset6);

	transition(&ViaSync::ViaSyncUI::overwritePresets);

}


