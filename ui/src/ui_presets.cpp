/*
 * ViaUI::presets.cpp
 *
 *  Created on: Sep 11, 2018
 *      Author: willmitchell
 */


#include "user_interface.hpp"

void ViaUI::presetMenu(int32_t sig) {

	switch (sig) {

	case ENTRY_SIG:
		presetEnterMenuCallback();
		break;

	case SENSOR_EVENT_SIG:

		if (*button1 == pressedState) {
			transition(&ViaUI::presetPressedMenu);
			presetNumber = 1;
		} else if (*button2 == pressedState) {
			transition(&ViaUI::presetPressedMenu);
			presetNumber = 2;
		} else if (*button3 == pressedState) {
			transition(&ViaUI::presetPressedMenu);
			presetNumber = 3;
		} else if (*button4 == pressedState) {
			transition(&ViaUI::presetPressedMenu);
			presetNumber = 4;
		} else if (*button5 == pressedState) {
			transition(&ViaUI::presetPressedMenu);
			presetNumber = 5;
		} else if (*button6 == pressedState) {
			transition(&ViaUI::presetPressedMenu);
			presetNumber = 6;
		}
		break;

		case EXPAND_SW_OFF_SIG:
		transition(&ViaUI::defaultMenu);
		break;

		case EXIT_SIG:

		break;

		default:
		break;
	}
}

void ViaUI::presetPressedMenu(int32_t sig) {
	switch (sig) {
	case ENTRY_SIG:
		timerReset();
		timerSetOverflow(5000);
		timerEnable();
		break;

	case SENSOR_EVENT_SIG:
		switch (presetNumber) {
		case 1:
			if (*button1 == releasedState) {
				loadFromEEPROM(presetNumber);
				recallModuleState();
				transition(&ViaUI::switchPreset);
			}
			break;
			case 2:
			if (*button2 == releasedState) {
				loadFromEEPROM(presetNumber);
				recallModuleState();
				transition(&ViaUI::switchPreset);
			}
			break;
			case 3:
			if (*button3 == releasedState) {
				loadFromEEPROM(presetNumber);
				recallModuleState();
				transition(&ViaUI::switchPreset);
			}
			break;
			case 4:
			if (*button4 == releasedState) {
				loadFromEEPROM(presetNumber);
				recallModuleState();
				transition(&ViaUI::switchPreset);
			}
			break;
			case 5:
			if (*button5 == releasedState) {
				loadFromEEPROM(presetNumber);
				recallModuleState();
				transition(&ViaUI::switchPreset);
			}
			break;
			case 6:
			if (*button6 == releasedState) {
				loadFromEEPROM(presetNumber);
				recallModuleState();
				transition(&ViaUI::switchPreset);
			}
			break;
		}
		break;

		case TIMEOUT_SIG:
		storeStateToEEPROM(presetNumber);
		transition(&ViaUI::newPreset);
		break;

		// if trig button is released, exit menu
		case EXPAND_SW_OFF_SIG:
		transition(&ViaUI::defaultMenu);
		break;

		case EXIT_SIG:
		break;
	}
}

/**
 *
 * Flashes around the full led enumeration 2 times
 *
 */

void ViaUI::newPreset(int32_t sig) {
	static int32_t flashCounter = 0;
	switch (sig) {
	case ENTRY_SIG:
		timerReset();
		timerSetOverflow(500);
		timerEnable();
		break;

	case TIMEOUT_SIG:
		if (flashCounter < 16) {
			flashCounter++;
			uiSetLEDs(flashCounter % 8);
		} else {
			flashCounter = 0;
			transition(&ViaUI::defaultMenu);
		}
	}
}

/**
 *
 * Flashes around the white leds once.
 *
 */

void ViaUI::switchPreset(int32_t sig) {
	static int32_t flashCounter = 0;
	switch (sig) {
	case ENTRY_SIG:
		timerReset();
		timerSetOverflow(500);
		timerEnable();
		break;

	case TIMEOUT_SIG:
		if (flashCounter < 4) {
			flashCounter++;
			uiSetLEDs(flashCounter % 4);
		} else {
			flashCounter = 0;
			if (EXPANDER_BUTTON_PRESSED) {
				transition(&ViaUI::presetMenu);
			} else {
				transition(&ViaUI::defaultMenu);
			}
		}
	}
}

/**
 *
 * Flashes around the full led enumeration 4 times.
 *
 */

void ViaUI::overwritePresets(int32_t sig) {
	static int32_t flashCounter = 0;
	switch (sig) {
	case ENTRY_SIG:
		timerReset();
		timerSetOverflow(500);
		timerEnable();
		break;

	case TIMEOUT_SIG:
		if (flashCounter < 32) {
			flashCounter++;
			uiSetLEDs(flashCounter % 8);
		} else {
			flashCounter = 0;
			transition(&ViaUI::defaultMenu);
		}
	}
}
