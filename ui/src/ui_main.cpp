/*
 * main.cpp
 *
 *  Created on: Sep 11, 2018
 *      Author: willmitchell
 */

#include "user_interface.hpp"


// Call the current state with a signal

void ViaUI::dispatch(int32_t sig) {
	(this->*state)(sig);
}

/**
 *
 * Transition to new UI state
 * Send the exit signal to the current state
 * Change the state pointer to the new state
 * Send the entry signal to the new state
 *
 */

void ViaUI::transition(void (ViaUI::*func)(int32_t)) {
	dispatch(EXIT_SIG);
	state = func;
	dispatch(ENTRY_SIG);
}

/**
 *
 * Default runtime state.
 * Looks for sensor or trigger button event to launch menu.
 *
 */

void ViaUI::defaultMenu(int32_t sig) {

	if (sig == ENTRY_SIG) {
		defaultEnterMenuCallback();
		timerReset();
		timerDisable();
	} else if (sig == SENSOR_EVENT_SIG) {
		if (*button3 == pressedState) {
			transition(&ViaUI::button3Menu);

		} else if (*button1 == pressedState) {
			transition(&ViaUI::button1Menu);

		} else if (*button4 == pressedState) {
			transition(&ViaUI::button4Menu);

		} else if (*button6 == pressedState) {
			transition(&ViaUI::button6Menu);

		} else if (*button2 == pressedState) {
			transition(&ViaUI::button2Menu);

		} else if (*button5 == pressedState) {
			transition(&ViaUI::button5Menu);
		}
	} else if (sig == EXPAND_SW_ON_SIG) {
		transition(&ViaUI::presetMenu);
	}
}

/**
 *
 * Write current state to EEPROM on entry for state recall after power down.
 * Stay in state til timeout.
 * Launch appropriate menu on sensor event or pushbutton signal.
 *
 */

void ViaUI::newModeMenu(int32_t sig) {
	switch (sig) {
	case ENTRY_SIG:
		newModeEnterMenuCallback();
#ifdef BUILD_F373
		storeStateToEEPROM(0);
#endif
		timerReset();
		timerSetOverflow(5000);
		timerEnable();
		break;


		// once timerRead() times out, clear display and return to default state
	case TIMEOUT_SIG:
		transition(&ViaUI::defaultMenu);
		break;

	case EXPAND_SW_ON_SIG:
		transition(&ViaUI::presetMenu);
		break;

		// in case of new events immediately jump to relevant menu
	case SENSOR_EVENT_SIG:

		if (*button3== pressedState) {
			transition(&ViaUI::button3Menu);

		} else if (*button1 == pressedState) {
			transition(&ViaUI::button1Menu);

		} else if (*button4 == pressedState) {
			transition(&ViaUI::button4Menu);

		} else if (*button6 == pressedState) {
			transition(&ViaUI::button6Menu);

		} else if (*button2 == pressedState) {
			transition(&ViaUI::button2Menu);

		} else if (*button5 == pressedState) {
			transition(&ViaUI::button5Menu);
		}
		break;

		case EXIT_SIG:

		break;
	}

}

/**
 *
 * Transition to this state after the aux mode has changed.
 * Look for a new aux mode sensor event and exit on shift button release.
 *
 */

void ViaUI::newAuxModeMenu(int32_t sig) {
	switch (sig) {
	case ENTRY_SIG:
		newAuxModeEnterMenuCallback();
#ifdef BUILD_F373
		storeStateToEEPROM(0);
#endif
		timerReset();
		timerDisable();
		break;

	case SENSOR_EVENT_SIG:
		if (*button1 == pressedState) {
			transition(&ViaUI::aux1Menu);
		} else if (*button3 == pressedState) {
			transition(&ViaUI::aux2Menu);
		} else if (*button2 == pressedState) {
			transition(&ViaUI::aux2MenuAlt);
		} else if (*button4 == pressedState) {
			transition(&ViaUI::aux3Menu);
		} else if (*button6 == pressedState) {
			transition(&ViaUI::aux4Menu);
		} else if (*button5 == releasedState) {
			transition(&ViaUI::defaultMenu);
		}
		break;
	}
}


