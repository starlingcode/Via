/** \file .hpp
 * \brief Calibration module implementation.
 *
 *  An empty Via  for lots of hacking fun
 */


#ifndef INC__HPP_
#define INC__HPP_

#include "user_interface.hpp"
#include <via_platform_binding.hpp>

/// Macro used to specify the number of samples to per DAC transfer.
#define VIA__BUFFER_SIZE 32

/// Callback to link to the C code in the STM32  Sense Library.
void TouchLink (void * uiVoid);

class Via : public ViaModule {

public:

	class ViaUI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		Via& this_module;


//		//@{
//		/// Functions to handle a tap event on a  sensor or shift combo.
//		void button1TapCallback(void) override;
//		void button2TapCallback(void) override;
//		void button3TapCallback(void) override;
//		void button4TapCallback(void) override;
//		void button5TapCallback(void) override;
//		void button6TapCallback(void) override;
//
//		void aux1TapCallback(void) override;
//		void aux2TapCallback(void) override;
//		void aux2AltTapCallback(void) override;
//		void aux3TapCallback(void) override;
//		void aux4TapCallback(void) override;
//		//@}
//
//		//@{
//		/// Functions to handle a hold event on a  sensor or shift combo.
//		void button1HoldCallback(void) override;
//		void button2HoldCallback(void) override;
//		void button3HoldCallback(void) override;
//		void button4HoldCallback(void) override;
//		void button5HoldCallback(void) override;
//		void button6HoldCallback(void) override;
//
//		void aux1HoldCallback(void) override;
//		void aux2HoldCallback(void) override;
//		void aux2AltHoldCallback(void) override;
//		void aux3HoldCallback(void) override;
//		void aux4HoldCallback(void) override;
//		//@}

//		//@{
//		/// Methods to handle entry transitions into the main module states.
//		void defaultEnterMenuCallback(void) override;
//		void newModeEnterMenuCallback(void) override;
//		void newAuxModeEnterMenuCallback(void) override;
//		void presetEnterMenuCallback(void) override;
//		//@}
//
//		//@{
//		/// Methods to handle entry transitions into  sensor menu states.
//		void button1EnterMenuCallback(void) override;
//		void button2EnterMenuCallback(void) override;
//		void button3EnterMenuCallback(void) override;
//		void button4EnterMenuCallback(void) override;
//		void button5EnterMenuCallback(void) override;
//		void button6EnterMenuCallback(void) override;
//
//		void aux1EnterMenuCallback(void) override;
//		void aux2EnterMenuCallback(void) override;
//		void aux2AltEnterMenuCallback(void) override;
//		void aux3EnterMenuCallback(void) override;
//		void aux4EnterMenuCallback(void) override;
//		//@}

		void initialize(void) override {

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

		void uiSetLEDs(int mode) override {

			/// This connects the UI library's LED setting calls to the enumerated LED handler of the module.
			/// This could probably go away with some refactoring.

			this_module.setLEDs(mode);

		}

		void recallModuleState(void) override {

		}

		void blinkOnCallback(void) override {
			restoreRed = *(this_module.redLevel);
			restoreGreen = *(this_module.greenLevel);
			restoreBlue = *(this_module.blueLevel);
			this_module.updateRGBDisplay(4095, 4095, 4095, 1);
		}

		void blinkOffCallback(void) override {
			this_module.updateRGBDisplay(restoreRed, restoreGreen,
					restoreBlue, 1);
		}

		/// On construction, link the calibLink callback to the STM32  sense library.
		ViaUI(Via& x): this_module(x) {
			linkUI((void *) &Link, (void *) this);
		}


	};

	/// An instance of the UI implementation.
	ViaUI UI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the  sense library.
	/// Called in the executable code in user_interrupts.c.
	/// For some reason I have it wrapped like this?
	/// Perhaps the C code in the hardware executable mangled the namespace? Gotta check.
	void ui_dispatch(int32_t sig) {
		this->UI.dispatch(sig);
	};


	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {

	}
	void mainFallingEdgeCallback(void) {

	}
	void auxRisingEdgeCallback(void) {

	}
	void auxFallingEdgeCallback(void) {

	}
	void buttonPressedCallback(void) {

	}
	void buttonReleasedCallback(void) {

	}
	void ioProcessCallback(void) {}
	void halfTransferCallback(void) {

	}
	void transferCompleteCallback(void) {

	}
	void slowConversionCallback(void) {


	}
	void auxTimer1InterruptCallback(void) {

	}
	void auxTimer2InterruptCallback(void) {

	}

	int32_t numButton1Modes = 4;
	int32_t numButton2Modes = 4;
	int32_t numButton3Modes = 4;
	int32_t numButton4Modes = 2;
	int32_t numButton5Modes = 2;
	int32_t numButton6Modes = 4;

	void handleButton1ModeChange(int32_t);
	void handleButton2ModeChange(int32_t);
	void handleButton3ModeChange(int32_t);
	void handleButton4ModeChange(int32_t);
	void handleButton5ModeChange(int32_t);
	void handleButton6ModeChange(int32_t);

	/// On construction, call subclass constructors and pass each a pointer to the module class.
	Via() : UI(*this) {

		/// Link the module GPIO registers.
		initializeAuxOutputs();

		/// Initialize the input stream buffers.
		inputs.init(VIA__BUFFER_SIZE);
		/// Initialize the output stream buffers.
		outputs.init(VIA__BUFFER_SIZE);
		/// Set the data members that will be used to determine DMA stream initialization in the hardware executable.
		outputBufferSize = VIA__BUFFER_SIZE;
		inputBufferSize = 1;

		/// Call the UI initialization that needs to happen after outer class construction.
		UI.initialize();

	}

};

#endif /* INC__HPP_ */
