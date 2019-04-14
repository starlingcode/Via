/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include "gateseq.hpp"
#include "user_interface.hpp"

void ViaGateseq::ViaGateseqUI::initialize(void) {

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

void gateseqTouchLink (void * uiVoid) {
	ViaGateseq::ViaGateseqUI * ui = (ViaGateseq::ViaGateseqUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaGateseq::ViaGateseqUI::recallModuleState(void) {

	// protect against bogus values

	this_module.gateseqUI.button1Mode %= numButton1Modes;
	this_module.gateseqUI.button2Mode %= numButton2Modes;
	this_module.gateseqUI.button3Mode %= numButton3Modes;
	this_module.gateseqUI.button4Mode %= numButton4Modes;
	this_module.gateseqUI.button5Mode %= numButton5Modes;
	this_module.gateseqUI.button6Mode %= numButton6Modes;
	this_module.gateseqUI.aux1Mode = 0;
	this_module.gateseqUI.aux2Mode %= numAux2Modes;
	this_module.gateseqUI.aux3Mode = 0;
	this_module.gateseqUI.aux4Mode = 0;

	this_module.handleButton1ModeChange(this_module.gateseqUI.button1Mode);
	this_module.handleButton2ModeChange(this_module.gateseqUI.button2Mode);
	this_module.handleButton3ModeChange(this_module.gateseqUI.button3Mode);
	this_module.handleButton4ModeChange(this_module.gateseqUI.button4Mode);
	this_module.handleButton5ModeChange(this_module.gateseqUI.button5Mode);
	this_module.handleButton6ModeChange(this_module.gateseqUI.button6Mode);
	this_module.handleAux2ModeChange(this_module.gateseqUI.aux2Mode);

}

void ViaGateseq::ViaGateseqUI::uiSetLEDs(int mode) {
	this_module.setLEDs(mode);
}

void ViaGateseq::ViaGateseqUI::defaultEnterMenuCallback(void) {
	this_module.runtimeDisplay = 1;
}
void ViaGateseq::ViaGateseqUI::newModeEnterMenuCallback(void) {
	;
}
void ViaGateseq::ViaGateseqUI::newAuxModeEnterMenuCallback(void) {
	;
}
void ViaGateseq::ViaGateseqUI::presetEnterMenuCallback(void) {;

	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();

}

void ViaGateseq::ViaGateseqUI::button1EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(SH_A_MODE);
	resetTimerMenu();
}
void ViaGateseq::ViaGateseqUI::button2EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(AND_A_MODE);
	resetTimerMenu();
}
void ViaGateseq::ViaGateseqUI::button3EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(SEQ1_MODE);
	this_module.setRedLED((button3Mode >> 1) * 4095);
	this_module.setGreenLED(!(button3Mode >> 1) * 4095);
	if (button3Mode == 1 || button3Mode == 3) {
#ifdef BUILD_F373	
		SET_BLUE_LED_ONOFF(1);
#endif
#ifdef BUILD_VIRTUAL
		this_module.SET_BLUE_LED_ONOFF(1);
#endif
	} else {
#ifdef BUILD_F373	
	SET_BLUE_LED_ONOFF(0);
#endif
#ifdef BUILD_VIRTUAL
		this_module.SET_BLUE_LED_ONOFF(0);
#endif
	}
	resetTimerMenu();
}
void ViaGateseq::ViaGateseqUI::button4EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(SH_B_MODE);
	resetTimerMenu();
}
void ViaGateseq::ViaGateseqUI::button5EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(AND_B_MODE);
	resetTimerMenu();
}
void ViaGateseq::ViaGateseqUI::button6EnterMenuCallback(void) {
	this_module.runtimeDisplay = 0;
	this_module.clearLEDs();
	this_module.clearRGB();
	this_module.setLEDs(SEQ2_MODE);
	resetTimerMenu();
}
void ViaGateseq::ViaGateseqUI::aux1EnterMenuCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}
void ViaGateseq::ViaGateseqUI::aux2EnterMenuCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}
void ViaGateseq::ViaGateseqUI::aux2AltEnterMenuCallback(void) {
	this_module.clearLEDs();
	this_module.setLEDs(LOGIC_MODE);
	resetTimerMenu();
}
void ViaGateseq::ViaGateseqUI::aux3EnterMenuCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}
void ViaGateseq::ViaGateseqUI::aux4EnterMenuCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}

void ViaGateseq::ViaGateseqUI::button1TapCallback(void) {
	SH_A_MODE = incrementModeAndStore(SH_A_MODE, BUTTON1_MASK, numButton1Modes, BUTTON1_SHIFT);
	this_module.handleButton1ModeChange(SH_A_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(SH_A_MODE);
	transition(&ViaGateseq::ViaGateseqUI::newModeMenu);
}
void ViaGateseq::ViaGateseqUI::button2TapCallback(void) {
	AND_A_MODE = incrementModeAndStore(AND_A_MODE, BUTTON2_MASK, numButton2Modes, BUTTON2_SHIFT);
	this_module.handleButton2ModeChange(AND_A_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(AND_A_MODE);
	transition(&ViaGateseq::ViaGateseqUI::newModeMenu);
}
void ViaGateseq::ViaGateseqUI::button3TapCallback(void) {
	SEQ1_MODE = incrementModeAndStore(SEQ1_MODE, BUTTON3_MASK, numButton3Modes, BUTTON3_SHIFT);
	this_module.handleButton3ModeChange(SEQ1_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(SEQ1_MODE);
	transition(&ViaGateseq::ViaGateseqUI::newModeMenu);
}
void ViaGateseq::ViaGateseqUI::button4TapCallback(void) {
	SH_B_MODE = incrementModeAndStore(SH_B_MODE, BUTTON4_MASK, numButton4Modes, BUTTON4_SHIFT);
	this_module.handleButton4ModeChange(SH_B_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(SH_B_MODE);
	transition(&ViaGateseq::ViaGateseqUI::newModeMenu);
}
void ViaGateseq::ViaGateseqUI::button5TapCallback(void) {
	AND_B_MODE = incrementModeAndStore(AND_B_MODE, BUTTON5_MASK, numButton5Modes, BUTTON5_SHIFT);
	this_module.handleButton5ModeChange(AND_B_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(AND_B_MODE);
	transition(&ViaGateseq::ViaGateseqUI::newModeMenu);
}
void ViaGateseq::ViaGateseqUI::button6TapCallback(void) {
	SEQ2_MODE = incrementModeAndStore(SEQ2_MODE, BUTTON6_MASK, numButton6Modes, BUTTON6_SHIFT);
	this_module.handleButton6ModeChange(SEQ2_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(SEQ2_MODE);
	transition(&ViaGateseq::ViaGateseqUI::newModeMenu);
}

void ViaGateseq::ViaGateseqUI::aux1TapCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}

void ViaGateseq::ViaGateseqUI::aux2TapCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}

void ViaGateseq::ViaGateseqUI::aux2AltTapCallback(void) {
	LOGIC_MODE = incrementModeAndStore(LOGIC_MODE, AUX_MODE2_MASK, numAux2Modes, AUX_MODE2_SHIFT);
	this_module.handleAux2ModeChange(LOGIC_MODE);
	this_module.clearLEDs();
	this_module.setLEDs(LOGIC_MODE);
	transition(&ViaGateseq::ViaGateseqUI::newAuxModeMenu);
}

void ViaGateseq::ViaGateseqUI::aux3TapCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}

void ViaGateseq::ViaGateseqUI::aux4TapCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}

void ViaGateseq::ViaGateseqUI::button1HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}
void ViaGateseq::ViaGateseqUI::button2HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}
void ViaGateseq::ViaGateseqUI::button3HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}
void ViaGateseq::ViaGateseqUI::button4HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}
void ViaGateseq::ViaGateseqUI::button5HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}
void ViaGateseq::ViaGateseqUI::button6HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}

void ViaGateseq::ViaGateseqUI::aux1HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::button5Menu);
}

void ViaGateseq::ViaGateseqUI::aux2AltHoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}

void ViaGateseq::ViaGateseqUI::aux2HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}

void ViaGateseq::ViaGateseqUI::aux3HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}

void ViaGateseq::ViaGateseqUI::aux4HoldCallback(void) {
	transition(&ViaGateseq::ViaGateseqUI::defaultMenu);
}

void ViaGateseq::ViaGateseqUI::writeStockPresets(void) {

	storeToEEPROM(1, stockPreset1);
	storeToEEPROM(2, stockPreset2);
	storeToEEPROM(3, stockPreset3);
	storeToEEPROM(4, stockPreset4);
	storeToEEPROM(5, stockPreset5);
	storeToEEPROM(6, stockPreset6);

	transition(&ViaGateseq::ViaGateseqUI::overwritePresets);

}
