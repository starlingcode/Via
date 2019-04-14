/** \file user_interface.hpp
 * \brief UI abstraction.
 *
 *  A module implementing a simple testing/calibration helper program that serves as a template.
 */

#ifndef INC_USER_INTERFACE_HPP_
#define INC_USER_INTERFACE_HPP_

#include <via_platform_binding.hpp>


#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILD_F373
#include "tsl_user.h"
#include "eeprom.h"
#endif

/// An anonymous callback to pass tsc state change events from the 3rd party C touch sense library.
void linkUI(void *, void *);

#ifdef __cplusplus
}
#endif

/// A class to represent the touch button states, accessible from other code (like the VCVRack implementation).
class ViaVirtualButtons {
public:
	int32_t button1 = 0;
	int32_t button2 = 0;
	int32_t button3 = 0;
	int32_t button4 = 0;
	int32_t button5 = 0;
	int32_t button6 = 0;
};

/// Signals for the UI state machine.
enum uiSignals {
	NULL_SIG, /// Null signal, all state functions should ignore this signal and return their parent state or NONE if it's the top level state
	ENTRY_SIG, /// Entry signal, a state function should perform its entry actions (if any)
	EXIT_SIG,/// Exit signal, a state function should pEntry signal, a state function should perform its entry actions (if any)erform its exit actions (if any)
	INIT_SIG, /// Just look to global value and initialize, return to default state.  For recalling (presets, memory)
	TIMEOUT_SIG, /// Timer timeout
	SENSOR_EVENT_SIG, /// Sensor state machine state changed.
	EXPAND_SW_ON_SIG,  /// Expander button depressed
	EXPAND_SW_OFF_SIG, /// Expander button released
	TSL_ERROR_SIG
};

/// Main UI abstraction. Handles parsing touch sensor events, parameter storage, and overall state storage/recall
class ViaUI {

public:

	/// UI state machine implemented with a pointer to member function, holdover from the C implementation.
	void (ViaUI::*state)(int32_t);

	/// Some virtual timer data members if no hardware timer is available for timeout handling.
	int32_t virtualTimer = 0;
	int32_t virtualTimerEnable = 0;
	int32_t virtualTimerOverflow = 0;

	//@{
	/// Pointer assigned to the state of the touch sensor
	int32_t * button1;
	int32_t * button2;
	int32_t * button3;
	int32_t * button4;
	int32_t * button5;
	int32_t * button6;
	//@}

#ifdef BUILD_F373
	/// Definition of the pressed state.
	int32_t pressedState = PRESSED;
	/// Definition of the released state.
	int32_t releasedState = RELEASED;
#endif

#ifdef BUILD_VIRTUAL
	int32_t pressedState = 1;
	int32_t releasedState = 0;

	int32_t trigButton;

	#define EXPANDER_BUTTON_PRESSED trigButton

#endif

	/// A function taking a state function as the argument used to transition to that state.
	virtual void transition(void (ViaUI::*func)(int32_t));



	// dispatch a signal to current state
	void dispatch(int32_t);  // dispatch signal to state

	/// Main default state; all other states have a path back to default when no sensors are pressed and the preset menu has not been activated.
	void defaultMenu(int32_t sig);
	/// State entered when a mode has changed, usually displays the new mode for a time before returning to runtime display.
	void newModeMenu(int32_t sig);
	/// State entered when one of the aux modes has changed. This state prevents extraneous touch events on the shift button.
	void newAuxModeMenu(int32_t sig);

	/// State entered when button 1 (top left) is pressed. By default, the state does not change if another button is pressed.
	void button1Menu(int32_t sig);
	/// State entered when button 2 (top middle) is pressed. By default, the state does not change if another button is pressed.
	void button2Menu(int32_t sig);
	/// State entered when button 3 (top right) is pressed. By default, the state does not change if another button is pressed.
	void button3Menu(int32_t sig);
	/// State entered when button 4 (bottom left) is pressed. By default, the state does not change if another button is pressed.
	void button4Menu(int32_t sig);
	/// State entered when button 5 (bottom middle) is pressed. If another button is pressed, enter that aux mode.
	void button5Menu(int32_t sig);
	/// State entered when button 6 (bottom right) is pressed. By default, the state does not change if another button is pressed.
	void button6Menu(int32_t sig);
	/// State entered when button 1 (top left) is pressed while button 5 (bottom middle) is pressed.
	void aux1Menu(int32_t sig);
	/// State entered when button 3 (top right) is pressed while button 5 (bottom middle) is pressed.
	void aux2Menu(int32_t sig);
	/// State entered when button 2 (top middle) is pressed while button 5 (bottom middle) is pressed.
	void aux2MenuAlt(int32_t sig);
	/// State entered when button 4 (bottom left) is pressed while button 5 (bottom middle) is pressed.
	void aux3Menu(int32_t sig);
	/// State entered when button 6 (bottom right) is pressed while button 5 (bottom middle) is pressed.
	void aux4Menu(int32_t sig);

	/// Preset menu default state (no touch events yet).
	void presetMenu(int32_t sig);
	/// State entered after touch event in preset menu state.
	void presetPressedMenu(int32_t sig);
	/// State to indicate storage of a new preset
	void newPreset(int32_t sig);
	/// State to indicate preset recall.
	void switchPreset(int32_t sig);
	/// State to indicate factory preset recall.
	void overwritePresets(int32_t sig);

	/// Used by the preset menu to indicate the preset slot to be recalled or overwritten.
	int32_t presetNumber = 0;

	//@{
	/// State change event handlers. TODO add default methods.
	virtual void button1TapCallback(void) {
		transition(&ViaUI::defaultMenu);
	}
	virtual void button1HoldCallback(void) = 0;
	virtual void button2TapCallback(void) = 0;
	virtual void button2HoldCallback(void) = 0;
	virtual void button3TapCallback(void) = 0;
	virtual void button3HoldCallback(void) = 0;
	virtual void button4TapCallback(void) = 0;
	virtual void button4HoldCallback(void) = 0;
	virtual void button5TapCallback(void) = 0;
	virtual void button5HoldCallback(void) = 0;
	virtual void button6TapCallback(void) = 0;
	virtual void button6HoldCallback(void) = 0;

	virtual void aux1TapCallback(void) = 0;
	virtual void aux1HoldCallback(void) = 0;
	virtual void aux2TapCallback(void) = 0;
	virtual void aux2HoldCallback(void) = 0;
	virtual void aux2AltTapCallback(void) = 0;
	virtual void aux2AltHoldCallback(void) = 0;
	virtual void aux3TapCallback(void) = 0;
	virtual void aux3HoldCallback(void) = 0;
	virtual void aux4TapCallback(void) = 0;
	virtual void aux4HoldCallback(void) = 0;

	virtual void defaultEnterMenuCallback(void) = 0;
	virtual void newModeEnterMenuCallback(void) = 0;
	virtual void newAuxModeEnterMenuCallback(void) = 0;
	virtual void presetEnterMenuCallback(void) = 0;

	virtual void button1EnterMenuCallback(void) = 0;
	virtual void button2EnterMenuCallback(void) = 0;
	virtual void button3EnterMenuCallback(void) = 0;
	virtual void button4EnterMenuCallback(void) = 0;
	virtual void button5EnterMenuCallback(void) = 0;
	virtual void button6EnterMenuCallback(void) = 0;

	virtual void aux1EnterMenuCallback(void) = 0;
	virtual void aux2EnterMenuCallback(void) = 0;
	virtual void aux2AltEnterMenuCallback(void) = 0;
	virtual void aux3EnterMenuCallback(void) = 0;
	virtual void aux4EnterMenuCallback(void) = 0;
	//@}

	/// Virtual method used in subclasses to access module display drivers.
	virtual void uiSetLEDs(int) = 0;

	/// Virtual method to represent state recall, implemented in subclasses.
	virtual void recallModuleState(void) = 0;

	/// Virtual method used to represent writing the factory presets, implemented in subclasses.
	virtual void writeStockPresets(void) = 0;

	/// Initial setup of UI
	virtual void initialize(void) = 0;

	virtual ~ViaUI(void) {};

	//@{
	/// A data member for the 10 modes capable of being stored and recalled, 3 bits for each mode.
	int32_t button1Mode = 0;
	int32_t button2Mode = 0;
	int32_t button3Mode = 0;
	int32_t button4Mode = 0;
	int32_t button5Mode = 0;
	int32_t button6Mode = 0;
	int32_t aux1Mode = 0;
	int32_t aux2Mode = 0;
	int32_t aux3Mode = 0;
	int32_t aux4Mode = 0;
	//@}

	//@{
	/// Defines to aid packing and unpacking the modes into a single word.
	/// Mask off all bits but the ones storing the relevant mode, then right shift by the amount of the shift define to align the bits.
	#define BUTTON1_MASK 		0b00000000000000000000000000000111
	#define BUTTON1_SHIFT		0

	#define BUTTON2_MASK 		0b00000000000000000000000000111000
	#define BUTTON2_SHIFT		3

	#define BUTTON3_MASK 		0b00000000000000000000000111000000
	#define BUTTON3_SHIFT 		6

	#define BUTTON4_MASK 		0b00000000000000000000111000000000
	#define BUTTON4_SHIFT 		9

	#define BUTTON5_MASK	 	0b00000000000000000111000000000000
	#define BUTTON5_SHIFT	 	12

	#define BUTTON6_MASK		0b00000000000000111000000000000000
	#define BUTTON6_SHIFT		15

	#define AUX_MODE1_MASK		0b00000000000111000000000000000000
	#define AUX_MODE1_SHIFT		18

	#define AUX_MODE2_MASK		0b00000000111000000000000000000000
	#define AUX_MODE2_SHIFT		21

	#define AUX_MODE3_MASK		0b00000111000000000000000000000000
	#define AUX_MODE3_SHIFT		24

	#define AUX_MODE4_MASK		0b00111000000000000000000000000000
	#define AUX_MODE4_SHIFT		27

	//@}

	/// A macro to encode the 10 modes as integers into the packed single word format
	#define ENCODE_PRESET(B1, B2, B3, B4, B5, B6, A1, A2, A3, A4) \
		(B1 | (B2 << BUTTON2_SHIFT) | (B3 << BUTTON3_SHIFT) | (B4 << BUTTON4_SHIFT) | \
			(B5 << BUTTON5_SHIFT) | (B6 << BUTTON6_SHIFT) | (A1 << AUX_MODE1_SHIFT) | \
				(A2 << AUX_MODE2_SHIFT) | (A3 << AUX_MODE3_SHIFT) | (A4 << AUX_MODE4_SHIFT))

	/// holds the mode state as a EEPROM-formatted value.
	int32_t modeStateBuffer = 0;

	/// Load a state page from the eeprom storage and set the mode variables accordingly. Position 0 is last state, positions 1-6 are presets.
	void loadFromEEPROM(int32_t position);
	/// Store the current modeStateBuffer to an EEPROM page, use position 0 for current state storage, positions 1-6 for presets 1-6.
	void storeStateToEEPROM(int32_t position);
	/// Store an arbitrary word to an EEPROM page, used to write calibration packet to position 7.
	void storeToEEPROM(int32_t position, uint32_t data);

	/// Increment mode modulo numModes of modes and store to eeprom using mask and shift.
	int32_t incrementModeAndStore(int32_t mode, int32_t mask, int32_t numModes, int32_t shift);
	/// Decrement mode modulo numModes of modes and store to eeprom using mask and shift.
	int32_t decrementModeAndStore(int32_t mode, int32_t mask, int32_t numModes, int32_t shift);

	void storeMode(int32_t mode, int32_t mask, int32_t shift);


#ifdef BUILD_F373

	/// Reset UI timer; does not affect enable/disable.
	inline void timerReset(void) {
		TIM7->CNT = 1;
	}
	/// Enable the UI timer.
	inline void timerEnable(void) {
		TIM7->CR1 |= TIM_CR1_CEN;
	}
	/// Disable the UI timer.
	inline void timerDisable(void) {
		TIM7->CR1 &= ~TIM_CR1_CEN;
	}
	/// Set the overflow of the timer, in other words the time from reset to timeout event.
	inline void timerSetOverflow(int32_t arr) {
		TIM7->ARR = arr;
	}
	/// Read the current count of the timer.
	inline int32_t timerRead(void) {
		return TIM7->CNT;
	}

	/// Data member to represent the status of the eeprom library after initialzation.
	int32_t eepromStatus;

	/// A copy of the virtual address table used in stm32f373/scr/eeprom.c
	uint16_t VirtAddVarTab[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	/// Cal the eeprom library initialization function
	void initializeMemory(void) {
		eepromStatus = EE_Init();
	}

	/// Read a state packet from one of the 8 words in the EEPROM and store it to the mode state data member.
	void loadStateFromMemory(int32_t position) {

		uint16_t eepromTemp;

		eepromStatus = EE_ReadVariable(VirtAddVarTab[position * 2], &eepromTemp);
		modeStateBuffer = (int32_t) eepromTemp;  // load bottom 16 bits
		eepromStatus |= EE_ReadVariable(VirtAddVarTab[(position * 2) + 1],
				&eepromTemp);
		modeStateBuffer |= ((int32_t) eepromTemp) << 16;  // load 16 upper bits

	}

	/// Read a state packet from one of the 8 words in the EEPROM and return it.
	uint32_t loadFromMemory(int32_t position) {

		uint16_t eepromTemp;
		uint32_t data;

		eepromStatus = EE_ReadVariable(VirtAddVarTab[position * 2], &eepromTemp);
		data = (int32_t) eepromTemp;  // load bottom 16 bits
		eepromStatus |= EE_ReadVariable(VirtAddVarTab[(position * 2) + 1],
				&eepromTemp);
		data |= ((int32_t) eepromTemp) << 16;  // load 16 upper bits
		return data;

	}

#endif

#ifdef BUILD_VIRTUAL

	void loadStateFromMemory(int32_t position) {
		;
	}

	uint32_t loadFromMemory(int32_t position) {
		return 0;
	}

	void timerReset(void) {
		virtualTimer = 0;
	}
	void timerEnable(void) {
		virtualTimerEnable = 1;
	}
	void timerDisable(void) {
		virtualTimerEnable = 0;
	}
	void timerSetOverflow(int32_t arr) {
		virtualTimerOverflow = arr;
	}
	int32_t timerRead(void) {
		return virtualTimer;
	}

	void incrementTimer(void) {
		virtualTimer += virtualTimerEnable;
		if (virtualTimer >= virtualTimerOverflow) {
            virtualTimer = 0;
            dispatch(TIMEOUT_SIG);
        }
	}

#endif

		/// An aggregate function to reset the timer, set the timeout to max, and enable it. Useful when entering a button menu and measuring length of press on release event with timerRead().
	void resetTimerMenu(void) {
		timerReset();
		timerSetOverflow(65535);
		timerEnable();
	}

};

#endif /* INC_USER_INTERFACE_HPP_ */
