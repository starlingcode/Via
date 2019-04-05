/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include "calib.hpp"
#include "user_interface.hpp"

void ViaCalib::ViaCalibUI::initialize(void) {

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

#ifdef BUILD_F373

	recallModuleState();

#endif

	state = &ViaUI::defaultMenu;
	transition(&ViaUI::defaultMenu);
}

void calibTouchLink (void * uiVoid) {
	ViaCalib::ViaCalibUI * ui = (ViaCalib::ViaCalibUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);
}

void ViaCalib::ViaCalibUI::recallModuleState(void) {


}

void ViaCalib::ViaCalibUI::uiSetLEDs(int mode) {

}

void ViaCalib::ViaCalibUI::defaultEnterMenuCallback(void) {

	if (this_module.currentState == &(this_module.checkTouch) &&
			(touch1OK & touch2OK & touch3OK & touch4OK & touch5OK & touch6OK)) {

		this_module.currentState->advanceState();

	}

}
void ViaCalib::ViaCalibUI::newModeEnterMenuCallback(void) {

}
void ViaCalib::ViaCalibUI::newAuxModeEnterMenuCallback(void) {

}
void ViaCalib::ViaCalibUI::presetEnterMenuCallback(void) {

}

void ViaCalib::ViaCalibUI::button1EnterMenuCallback(void) {

	if (this_module.currentState == &(this_module.checkTouch)) {
		this_module.setLEDA(1);
		touch1OK = 1;
	}
	resetTimerMenu();

}
void ViaCalib::ViaCalibUI::button2EnterMenuCallback(void) {

	if (this_module.currentState == &(this_module.checkTouch)) {
		this_module.setLEDB(1);
		touch2OK = 1;
	}
	resetTimerMenu();

}
void ViaCalib::ViaCalibUI::button3EnterMenuCallback(void) {

	if (this_module.currentState == &(this_module.checkTouch)) {
		this_module.setLEDC(1);
		touch3OK = 1;
	}
	resetTimerMenu();

}
void ViaCalib::ViaCalibUI::button4EnterMenuCallback(void) {

	if (this_module.currentState == &(this_module.checkTouch)) {
		this_module.setLEDD(1);
		touch4OK = 1;
	}
	resetTimerMenu();

}
void ViaCalib::ViaCalibUI::button5EnterMenuCallback(void) {

	if (this_module.currentState == &(this_module.checkTouch)) {
		this_module.setRedLED(2048);
		touch5OK = 1;
	}
	resetTimerMenu();

}
void ViaCalib::ViaCalibUI::button6EnterMenuCallback(void) {

	if (this_module.currentState == &(this_module.checkTouch)) {
		this_module.setBlueLED(2048);
		touch6OK = 1;
	}
	resetTimerMenu();

}
void ViaCalib::ViaCalibUI::aux1EnterMenuCallback(void) {

}
void ViaCalib::ViaCalibUI::aux2EnterMenuCallback(void) {

}

void ViaCalib::ViaCalibUI::aux2AltEnterMenuCallback(void) {

}

void ViaCalib::ViaCalibUI::aux3EnterMenuCallback(void) {

}
void ViaCalib::ViaCalibUI::aux4EnterMenuCallback(void) {

}

void ViaCalib::ViaCalibUI::button1TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}
void ViaCalib::ViaCalibUI::button2TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}
void ViaCalib::ViaCalibUI::button3TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}
void ViaCalib::ViaCalibUI::button4TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}
void ViaCalib::ViaCalibUI::button5TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}
void ViaCalib::ViaCalibUI::button6TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}

void ViaCalib::ViaCalibUI::aux1TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}

void ViaCalib::ViaCalibUI::aux2TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}

void ViaCalib::ViaCalibUI::aux2AltTapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}

void ViaCalib::ViaCalibUI::aux3TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}

void ViaCalib::ViaCalibUI::aux4TapCallback(void) {

	transition(&ViaCalib::ViaCalibUI::defaultMenu);

}

void ViaCalib::ViaCalibUI::button1HoldCallback(void) {
	if (this_module.currentState != &(this_module.checkTouch)) {
		this_module.currentState->advanceState();
	}
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}
void ViaCalib::ViaCalibUI::button2HoldCallback(void) {
	if (this_module.currentState != &(this_module.checkTouch)) {
		this_module.currentState->advanceState();
	}
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}
void ViaCalib::ViaCalibUI::button3HoldCallback(void) {
	if (this_module.currentState != &(this_module.checkTouch)) {
		this_module.currentState->advanceState();
	}
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}
void ViaCalib::ViaCalibUI::button4HoldCallback(void) {
	if (this_module.currentState != &(this_module.checkTouch)) {
		this_module.currentState->advanceState();
	}
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}
void ViaCalib::ViaCalibUI::button5HoldCallback(void) {
	if (this_module.currentState != &(this_module.checkTouch)) {
		this_module.currentState->advanceState();
	}
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}
void ViaCalib::ViaCalibUI::button6HoldCallback(void) {
	if (this_module.currentState != &(this_module.checkTouch)) {
		this_module.currentState->advanceState();
	}
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}

void ViaCalib::ViaCalibUI::aux1HoldCallback(void) {
	transition(&ViaCalib::ViaCalibUI::button5Menu);
}

void ViaCalib::ViaCalibUI::aux2HoldCallback(void) {
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}

void ViaCalib::ViaCalibUI::aux2AltHoldCallback(void) {
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}

void ViaCalib::ViaCalibUI::aux3HoldCallback(void) {
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}

void ViaCalib::ViaCalibUI::aux4HoldCallback(void) {
	transition(&ViaCalib::ViaCalibUI::defaultMenu);
}


