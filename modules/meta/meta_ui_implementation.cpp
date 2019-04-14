/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include "meta.hpp"
#include "user_interface.hpp"

void ViaMeta::ViaMetaUI::initialize(void) {

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

#ifdef BUILD_F373

	

#endif

	state = &ViaUI::defaultMenu;
	transition(&ViaUI::defaultMenu);
}

void metaTouchLink (void * uiVoid) {
	ViaMeta::ViaMetaUI * ui = (ViaMeta::ViaMetaUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaMeta::ViaMetaUI::recallModuleState(void) {

	// protect against bogus values

	this_module.metaUI.button1Mode %= numButton1Modes;
	this_module.metaUI.button2Mode %= numButton2Modes;
	this_module.metaUI.button3Mode %= numButton3Modes;
	this_module.metaUI.button4Mode %= numButton4Modes;
	this_module.metaUI.button5Mode %= numButton5Modes;
	this_module.metaUI.button6Mode %= numButton6Modes;
	this_module.metaUI.aux1Mode %= numAux1Modes;
	this_module.metaUI.aux2Mode %= numAux2Modes;
	this_module.metaUI.aux3Mode %= numAux3Modes;
	this_module.metaUI.aux4Mode %= numAux4Modes;


	this_module.handleAux2ModeChange(this_module.metaUI.aux2Mode);
	this_module.handleButton1ModeChange(this_module.metaUI.button1Mode);
	this_module.handleButton2ModeChange(this_module.metaUI.button2Mode);
	this_module.handleButton3ModeChange(this_module.metaUI.button3Mode);
	this_module.handleButton4ModeChange(this_module.metaUI.button4Mode);
	this_module.handleButton5ModeChange(this_module.metaUI.button5Mode);
	this_module.handleButton6ModeChange(this_module.metaUI.button6Mode);

}

void ViaMeta::ViaMetaUI::uiSetLEDs(int mode) {
	this_module.setLEDs(mode);
}

void ViaMeta::ViaMetaUI::defaultEnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.runtimeDisplay = 1;
	this_module.setLEDD(aux4Mode);
}
void ViaMeta::ViaMetaUI::newModeEnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
}
void ViaMeta::ViaMetaUI::newAuxModeEnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
}
void ViaMeta::ViaMetaUI::presetEnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
}

void ViaMeta::ViaMetaUI::button1EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(SH_MODE);
	resetTimerMenu();
}
void ViaMeta::ViaMetaUI::button2EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(TABLE);
	resetTimerMenu();
}
void ViaMeta::ViaMetaUI::button3EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(FREQ_MODE);
	switch (FREQ_MODE) {
	case audio:
		if (LOOP_MODE == noloop) {
			this_module.updateRGBDisplay(0, 4095, 4095, 1);
		} else {
			this_module.updateRGBDisplay(0, 0, 4095, 1);
		}
		break;
	case env:
		this_module.updateRGBDisplay(0, 4095, 0, 1);
		break;
	case seq:
		this_module.updateRGBDisplay(4095, 0, 0, 1);
		break;
	}
	resetTimerMenu();
}
void ViaMeta::ViaMetaUI::button4EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	if (!LOOP_MODE && !FREQ_MODE) {
		this->transition(&ViaMeta::ViaMetaUI::aux3Menu);
	} else {
		this_module.setLEDs(TRIG_MODE);
		resetTimerMenu();
	}
}
void ViaMeta::ViaMetaUI::button5EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(TABLE);
	resetTimerMenu();
}
void ViaMeta::ViaMetaUI::button6EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(LOOP_MODE);
	resetTimerMenu();
}
void ViaMeta::ViaMetaUI::aux1EnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.setLEDs(DRUM_AUX_MODE);
	resetTimerMenu();
}
void ViaMeta::ViaMetaUI::aux2EnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.setLEDs(LOGIC_A_MODE);
	resetTimerMenu();
}

void ViaMeta::ViaMetaUI::aux2AltEnterMenuCallback(void) {
	transition(&ViaMeta::ViaMetaUI::button5Menu);
}

void ViaMeta::ViaMetaUI::aux3EnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.setLEDs(DRUM_MODE);
	resetTimerMenu();
}
void ViaMeta::ViaMetaUI::aux4EnterMenuCallback(void) {
	this_module.clearLEDs();
	if (!LOOP_MODE && !FREQ_MODE) {
		this->transition(&ViaMeta::ViaMetaUI::aux1Menu);
	} else {
		this_module.setLEDs(DAC_3_MODE);
		resetTimerMenu();
	}
}

void ViaMeta::ViaMetaUI::button1TapCallback(void) {
	SH_MODE = incrementModeAndStore(SH_MODE, BUTTON1_MASK, numButton1Modes, BUTTON1_SHIFT);
	this_module.handleButton1ModeChange(SH_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(SH_MODE);
	transition(&ViaMeta::ViaMetaUI::newModeMenu);
}
void ViaMeta::ViaMetaUI::button2TapCallback(void) {
	TABLE = incrementModeAndStore(TABLE, BUTTON2_MASK, numButton2Modes, BUTTON2_SHIFT);
	this_module.handleButton2ModeChange(TABLE);
	this_module.clearLEDs();
	this_module.setLEDs(TABLE);
	transition(&ViaMeta::ViaMetaUI::newModeMenu);
}
void ViaMeta::ViaMetaUI::button3TapCallback(void) {
	FREQ_MODE = incrementModeAndStore(FREQ_MODE, BUTTON3_MASK, numButton3Modes, BUTTON3_SHIFT);
	this_module.handleButton3ModeChange(FREQ_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(FREQ_MODE);
	transition(&ViaMeta::ViaMetaUI::newModeMenu);
}
void ViaMeta::ViaMetaUI::button4TapCallback(void) {
	TRIG_MODE = incrementModeAndStore(TRIG_MODE, BUTTON4_MASK, numButton4Modes, BUTTON4_SHIFT);
	this_module.handleButton4ModeChange(TRIG_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(TRIG_MODE);
	transition(&ViaMeta::ViaMetaUI::newModeMenu);
}
void ViaMeta::ViaMetaUI::button5TapCallback(void) {
	TABLE = decrementModeAndStore(TABLE, BUTTON5_MASK, numButton5Modes, BUTTON5_SHIFT);
	this_module.handleButton5ModeChange(TABLE);
	this_module.clearLEDs();
	this_module.setLEDs(TABLE);
	transition(&ViaMeta::ViaMetaUI::newModeMenu);
}
void ViaMeta::ViaMetaUI::button6TapCallback(void) {
	LOOP_MODE = incrementModeAndStore(LOOP_MODE, BUTTON6_MASK, numButton6Modes, BUTTON6_SHIFT);
	this_module.handleButton6ModeChange(LOOP_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(LOOP_MODE);
	transition(&ViaMeta::ViaMetaUI::newModeMenu);
}

void ViaMeta::ViaMetaUI::aux1TapCallback(void) {
	DRUM_AUX_MODE = incrementModeAndStore(DRUM_AUX_MODE, AUX_MODE1_MASK, numAux1Modes, AUX_MODE1_SHIFT);
	this_module.handleAux1ModeChange(DRUM_AUX_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(DRUM_AUX_MODE);
	transition(&ViaMeta::ViaMetaUI::newAuxModeMenu);
}

void ViaMeta::ViaMetaUI::aux2TapCallback(void) {
	LOGIC_A_MODE = incrementModeAndStore(LOGIC_A_MODE, AUX_MODE2_MASK, numAux2Modes, AUX_MODE2_SHIFT);
	this_module.handleAux2ModeChange(LOGIC_A_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(LOGIC_A_MODE);
	transition(&ViaMeta::ViaMetaUI::newAuxModeMenu);
}

void ViaMeta::ViaMetaUI::aux2AltTapCallback(void) {
	transition(&ViaMeta::ViaMetaUI::button5Menu);
}

void ViaMeta::ViaMetaUI::aux3TapCallback(void) {
	DRUM_MODE = incrementModeAndStore(DRUM_MODE, AUX_MODE3_MASK, numAux3Modes, AUX_MODE3_SHIFT);
	this_module.handleAux3ModeChange(DRUM_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(DRUM_MODE);
	transition(&ViaMeta::ViaMetaUI::newModeMenu);
}

void ViaMeta::ViaMetaUI::aux4TapCallback(void) {
	DAC_3_MODE = incrementModeAndStore(DAC_3_MODE, AUX_MODE4_MASK, numAux4Modes, AUX_MODE4_SHIFT);
	this_module.handleAux4ModeChange(DAC_3_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(DAC_3_MODE);
	transition(&ViaMeta::ViaMetaUI::newAuxModeMenu);
}

void ViaMeta::ViaMetaUI::button1HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}
void ViaMeta::ViaMetaUI::button2HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}
void ViaMeta::ViaMetaUI::button3HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}
void ViaMeta::ViaMetaUI::button4HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}
void ViaMeta::ViaMetaUI::button5HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}
void ViaMeta::ViaMetaUI::button6HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}

void ViaMeta::ViaMetaUI::aux1HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::button5Menu);
}

void ViaMeta::ViaMetaUI::aux2HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}

void ViaMeta::ViaMetaUI::aux2AltHoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}

void ViaMeta::ViaMetaUI::aux3HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}

void ViaMeta::ViaMetaUI::aux4HoldCallback(void) {
	transition(&ViaMeta::ViaMetaUI::defaultMenu);
}

void ViaMeta::ViaMetaUI::writeStockPresets(void) {

	storeToEEPROM(1, stockPreset1);
	storeToEEPROM(2, stockPreset2);
	storeToEEPROM(3, stockPreset3);
	storeToEEPROM(4, stockPreset4);
	storeToEEPROM(5, stockPreset5);
	storeToEEPROM(6, stockPreset6);

	transition(&ViaMeta::ViaMetaUI::overwritePresets);

}
