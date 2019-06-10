/*
 * meta_ui_implementation.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */


#include <sync3.hpp>
#include "user_interface.hpp"

/// Callback to link to the C code in the STM32 Sync3 Sense Library.
void sync3TouchLink (void * uiVoid) {
	ViaSync3::ViaSync3UI * ui = (ViaSync3::ViaSync3UI *) uiVoid;
	ui->dispatch(SENSOR_EVENT_SIG);

}


