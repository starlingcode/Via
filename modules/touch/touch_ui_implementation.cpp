/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include <touch.hpp>
#include "user_interface.hpp"

/// Callback to link to the C code in the STM32 Touch Sense Library.
void touchTouchLink (void * uiVoid) {
	ViaTouch::ViaTouchUI * ui = (ViaTouch::ViaTouchUI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);

}


