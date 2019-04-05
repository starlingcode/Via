/*
 * ViaUI::save_recall.cpp
 *
 *  Created on: Sep 11, 2018
 *      Author: willmitchell
 */

#include "user_interface.hpp"

/*
 *
 * Template routine for the UI state machine.
 * Assign the button pointers to the address of the sensor state variables.
 * Initialize the eeprom and read the last saved mode set.
 * Initialize those modes
 * Set the UI state default
 *
 */
void ViaUI::initialize(void) {


#ifdef BUILD_F373

	button1 = (int32_t *) &BUTTON1SENSOR;
	button2 = (int32_t *) &BUTTON2SENSOR;
	button3 = (int32_t *) &BUTTON3SENSOR;
	button4 = (int32_t *) &BUTTON4SENSOR;
	button5 = (int32_t *) &BUTTON5SENSOR;
	button6 = (int32_t *) &BUTTON6SENSOR;

#endif

	state = &ViaUI::defaultMenu;
	transition(&ViaUI::defaultMenu);

}

void ViaUI::loadFromEEPROM(int32_t position) {

	loadStateFromMemory(position);

	button1Mode = modeStateBuffer & BUTTON1_MASK;
	button2Mode = (modeStateBuffer & BUTTON2_MASK) >> BUTTON2_SHIFT;
	button3Mode = (modeStateBuffer & BUTTON3_MASK) >> BUTTON3_SHIFT;
	button4Mode = (modeStateBuffer & BUTTON4_MASK) >> BUTTON4_SHIFT;
	button5Mode = (modeStateBuffer & BUTTON5_MASK) >> BUTTON5_SHIFT;
	button6Mode = (modeStateBuffer & BUTTON6_MASK) >> BUTTON6_SHIFT;
	aux1Mode = (modeStateBuffer & AUX_MODE1_MASK) >> AUX_MODE1_SHIFT;
	aux2Mode = (modeStateBuffer & AUX_MODE2_MASK) >> AUX_MODE2_SHIFT;
	aux3Mode = (modeStateBuffer & AUX_MODE3_MASK) >> AUX_MODE3_SHIFT;
	aux4Mode = (modeStateBuffer & AUX_MODE4_MASK) >> AUX_MODE4_SHIFT;

}

// writes 2 16-bit values representing the modeStateBuffer word to EEPROM at the specified position,  1 runtime + 6 presets + calibration word
void ViaUI::storeStateToEEPROM(int32_t position) {

#ifdef BUILD_F373
	eepromStatus = EE_WriteVariable(VirtAddVarTab[position * 2],
			(uint16_t) modeStateBuffer);
	eepromStatus |= EE_WriteVariable(VirtAddVarTab[(position * 2) + 1],
			(uint16_t) (modeStateBuffer >> 16));
#endif

}

// writes 2 16-bit values representing the data word to EEPROM per position,  1 runtime + 6 presets + calibration word
void ViaUI::storeToEEPROM(int32_t position, uint32_t data) {

#ifdef BUILD_F373
	eepromStatus = EE_WriteVariable(VirtAddVarTab[position * 2],
			(uint16_t) data);
	eepromStatus |= EE_WriteVariable(VirtAddVarTab[(position * 2) + 1],
			(uint16_t) (data >> 16));
#endif

}
