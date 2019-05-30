/*
 * ViaUI::menus.cpp
 *
 *  Created on: Sep 11, 2018
 *      Author: willmitchell
 */

#include "user_interface.hpp"

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::button1Menu(int32_t sig) {
	switch (sig) {
	case ENTRY_SIG:
		button1EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button1 == releasedState) {
			if(tapped) {
				button1TapCallback();
			} else {
				button1HoldCallback();
			}
		} else if ((*button3 == pressedState) && (*button4 == pressedState) &&
				(*button6 == pressedState)) {
			specialMenuCallback();
		}
		break;

	case TIMEOUT_SIG:
		if (tapped) {
			resetTimerBlink();
			tapped = 0;
			blink = 1;
			blinkOnCallback();
		} else if (blink) {
			resetTimerHold();
			blink = 0;
			blinkOffCallback();
		}
		break;

	default:
		break;
	}
}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::button2Menu(int32_t sig) {
	switch (sig) {
	case ENTRY_SIG:
		button2EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button2 == releasedState) {
			if(tapped) {
				button2TapCallback();
			} else {
				button2HoldCallback();
			}
		}
		break;

	case TIMEOUT_SIG:
		if (tapped) {
			resetTimerBlink();
			tapped = 0;
			blink = 1;
			blinkOnCallback();
		} else if (blink) {
			resetTimerHold();
			blink = 0;
			blinkOffCallback();
		}
		break;

	default:
		break;

	}
}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::button3Menu(int32_t sig) {
	switch (sig) {

	case ENTRY_SIG:
		button3EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button3 == releasedState) {
			if(tapped) {
				button3TapCallback();
			} else {
				button3HoldCallback();
			}
		}  else if ((*button4 == pressedState) && (*button1 == pressedState) &&
				(*button6 == pressedState)) {
			specialMenuCallback();
		}
		break;

		case INIT_SIG:
		break;

		case TIMEOUT_SIG:
			if (tapped) {
				resetTimerBlink();
				tapped = 0;
				blink = 1;
				blinkOnCallback();
			} else if (blink) {
				resetTimerHold();
				blink = 0;
				blinkOffCallback();
			}
			break;

		default:
			break;

	}
}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::button4Menu(int32_t sig) {
	switch (sig) {
	case ENTRY_SIG:
		button4EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:

		if (*button4 == releasedState) {
			if(tapped) {
				button4TapCallback();
			} else {
				button4HoldCallback();
			}
		}  else if ((*button3 == pressedState) && (*button1 == pressedState) &&
				(*button6 == pressedState)) {
			specialMenuCallback();
		}
		break;

	case TIMEOUT_SIG:
		if (tapped) {
			resetTimerBlink();
			tapped = 0;
			blink = 1;
			blinkOnCallback();
		} else if (blink) {
			resetTimerHold();
			blink = 0;
			blinkOffCallback();
		}
		break;

	default:
		break;
	}

}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
/// If a different button is pressed before release, enter that the corresponding aux menu state.
void ViaUI::button5Menu(int32_t sig) {
	switch (sig) {
	case ENTRY_SIG:
		button5EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:

		// Check for entry into an aux mode
		if (*button1 == pressedState) {
			if (aux1Enabled) {
				transition(&ViaUI::aux1Menu);
				tapped = 1;
			}
		} else if (*button3 == pressedState) {
			if (aux2Enabled) {
				transition(&ViaUI::aux2Menu);
				tapped = 1;
			}
		} else if (*button2 == pressedState) {
			if (aux2AltEnabled) {
				transition(&ViaUI::aux2MenuAlt);
				tapped = 1;
			}
		} else if (*button4 == pressedState) {
			if (aux3Enabled) {
				transition(&ViaUI::aux3Menu);
				tapped = 1;
			}
		} else if (*button6 == pressedState) {
			if (aux4Enabled) {
				transition(&ViaUI::aux4Menu);
				tapped = 1;
			}
		} else if (*button5 == releasedState) {
			if (tapped) {
				button5TapCallback();
			} else {
				button5HoldCallback();
			}
		}
		break;

	case TIMEOUT_SIG:
		if (tapped) {
			resetTimerBlink();
			tapped = 0;
			blink = 1;
			blinkOnCallback();
		} else if (blink) {
			resetTimerHold();
			blink = 0;
			blinkOffCallback();
		}
		break;

	default:
		break;
	}

}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::button6Menu(int32_t sig) {
	switch (sig) {

	case ENTRY_SIG:
		button6EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button6 == releasedState) {
			if(tapped) {
				button6TapCallback();
			} else {
				button6HoldCallback();
			}
		}  else if ((*button3 == pressedState) && (*button4 == pressedState) &&
				(*button1 == pressedState)) {
			specialMenuCallback();
		}
		break;

		case INIT_SIG:
		break;
		case TIMEOUT_SIG:
			if (tapped) {
				resetTimerBlink();
				tapped = 0;
				blink = 1;
				blinkOnCallback();
			} else if (blink) {
				resetTimerHold();
				blink = 0;
				blinkOffCallback();
			}
			break;

		default:
			break;
	}
}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::aux1Menu(int32_t sig) {
	switch (sig) {

	case ENTRY_SIG:
		aux1EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button1 == releasedState) {
			if(tapped) {
				aux1TapCallback();
			} else {
				aux1HoldCallback();
			}
		} else if ((*button3 == pressedState) && (*button4 == pressedState) &&
				(*button6 == pressedState)) {
			specialMenuCallback();
		}
		break;

		case INIT_SIG:
		break;

		case TIMEOUT_SIG:
			if (tapped) {
				resetTimerBlink();
				tapped = 0;
				blink = 1;
				blinkOnCallback();
			} else if (blink) {
				resetTimerHold();
				blink = 0;
				blinkOffCallback();
			}
			break;

		default:
			break;
	}
}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::aux2Menu(int32_t sig) {
	switch (sig) {

	case ENTRY_SIG:
		aux2EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button3 == releasedState) {
			if(tapped) {
				aux2TapCallback();
			} else {
				aux2HoldCallback();
			}
		} else if ((*button1 == pressedState) && (*button4 == pressedState) &&
				(*button6 == pressedState)) {
			specialMenuCallback();
		}
		break;

		case INIT_SIG:
		break;

		case TIMEOUT_SIG:
			if (tapped) {
				resetTimerBlink();
				tapped = 0;
				blink = 1;
				blinkOnCallback();
			} else if (blink) {
				resetTimerHold();
				blink = 0;
				blinkOffCallback();
			}
			break;

		default:
			break;
	}
}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::aux2MenuAlt(int32_t sig) {
	switch (sig) {

	case ENTRY_SIG:
		aux2AltEnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button2 == releasedState) {
			if(tapped) {
				aux2AltTapCallback();
			} else {
				aux2AltHoldCallback();
			}
		}
		break;

		case INIT_SIG:
		break;

		case TIMEOUT_SIG:
			if (tapped) {
				resetTimerBlink();
				tapped = 0;
				blink = 1;
				blinkOnCallback();
			} else if (blink) {
				resetTimerHold();
				blink = 0;
				blinkOffCallback();
			}
			break;

		default:
			break;
	}
}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::aux3Menu(int32_t sig) {
	switch (sig) {

	case ENTRY_SIG:
		aux3EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button4 == releasedState) {
			if(tapped) {
				aux3TapCallback();
			} else {
				aux3HoldCallback();
			}
		} else if ((*button3 == pressedState) && (*button1 == pressedState) &&
				(*button6 == pressedState)) {
			specialMenuCallback();
		}
		break;

		case INIT_SIG:
		break;

		case TIMEOUT_SIG:
			if (tapped) {
				resetTimerBlink();
				tapped = 0;
				blink = 1;
				blinkOnCallback();
			} else if (blink) {
				resetTimerHold();
				blink = 0;
				blinkOffCallback();
			}
			break;

		default:
			break;
	}
}

/// On entry, call handler function. If release before 4095 ticks of the UI timer (magic number alert), call tap handler function, otherwise call hold handler function.
void ViaUI::aux4Menu(int32_t sig) {
	switch (sig) {

	case ENTRY_SIG:
		aux4EnterMenuCallback();
		resetTimerMenu();
		break;

	case SENSOR_EVENT_SIG:
		if (*button6 == releasedState) {
			if(tapped) {
				aux4TapCallback();
			} else {
				aux4HoldCallback();
			}
		} else if ((*button3 == pressedState) && (*button4 == pressedState) &&
				(*button1 == pressedState)) {
			specialMenuCallback();
		}
		break;

		case INIT_SIG:
		break;

		case TIMEOUT_SIG:
			if (tapped) {
				resetTimerBlink();
				tapped = 0;
				blink = 1;
				blinkOnCallback();
			} else if (blink) {
				resetTimerHold();
				blink = 0;
				blinkOffCallback();
			}
			break;

		default:
			break;
	}
}

