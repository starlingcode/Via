
#ifndef INC_EMPTYVIA_HPP_
#define INC_EMPTYVIA_HPP_

#include "user_interface.hpp"
#include <via_platform_binding.hpp>
#include "dsp.hpp"

/// By default, the DAC buffer update rate is exactly synchronous with the SDADC conversions
/// This macro allows you to specify 2, 3, 4, 6, 8, 12, 16, 24, or 32x oversampling
#define VIA_EMPTYVIA_OVERSAMPLE 1

/// Updating the dac slower than 50k is not yet supported

/// Callback to link to the C code in the STM32 Touch Sense Library.
void emptyviaTouchLink (void * uiVoid);

class ViaEmptyvia : public TARGET_VIA {

public:

	typedef void (ViaEmptyvia::*renderPointer)(int32_t writePosition);

	class ViaEmptyviaUI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * Note how this is used to access module data in emptyvia_ui_implementation
		 */
		ViaEmptyvia& this_module;


//		//@{
//		/// Functions to handle a tap event on a emptyvia sensor or shift combo.
		void button1TapCallback(void) override;
		void button2TapCallback(void) override;
		void button3TapCallback(void) override;
		void button4TapCallback(void) override;
		void button5TapCallback(void) override;
		void button6TapCallback(void) override;

		void aux1TapCallback(void) override;
		void aux2TapCallback(void) override;
//		void aux2AltTapCallback(void) override;
		void aux3TapCallback(void) override;
		void aux4TapCallback(void) override;
		//@}

		//@{
		/// Functions to handle a hold event on a emptyvia sensor or shift combo.
		void button1HoldCallback(void) override;
		void button2HoldCallback(void) override;
		void button3HoldCallback(void) override;
		void button4HoldCallback(void) override;
		void button5HoldCallback(void) override;
		void button6HoldCallback(void) override;

		void aux1HoldCallback(void) override;
		void aux2HoldCallback(void) override;
//		void aux2AltHoldCallback(void) override;
		void aux3HoldCallback(void) override;
		void aux4HoldCallback(void) override;
		//@}

		//@{
		/// Methods to handle entry transitions into the main module states.
		void defaultEnterMenuCallback(void) override;
//		void newModeEnterMenuCallback(void) override;
//		void newAuxModeEnterMenuCallback(void) override;
//		void presetEnterMenuCallback(void) override;
		//@}

		//@{
		/// Methods to handle entry transitions into emptyvia sensor menu states.
		void button1EnterMenuCallback(void) override;
		void button2EnterMenuCallback(void) override;
		void button3EnterMenuCallback(void) override;
		void button4EnterMenuCallback(void) override;
		void button5EnterMenuCallback(void) override;
		void button6EnterMenuCallback(void) override;

		void aux1EnterMenuCallback(void) override;
		void aux2EnterMenuCallback(void) override;
//		void aux2AltEnterMenuCallback(void) override;
		void aux3EnterMenuCallback(void) override;
		void aux4EnterMenuCallback(void) override;
		//@}

		void initialize(void) override {

			// Link to the button status addresses


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

			// Load the state save block and call the state change handlers
			loadFromEEPROM(0);
			recallModuleState();

			// Initialize the touch menu state machine
			state = &ViaUI::defaultMenu;
			transition(&ViaUI::defaultMenu);

		}

		// Ui helpers

		void recallModuleState(void) override;

		void uiSetLEDs(int mode) override {

			this_module.setLEDs(mode);

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

		/// On construction, link the calibEmptyviaLink callback to the STM32 emptyvia sense library.
		ViaEmptyviaUI(ViaEmptyvia& x): this_module(x) {
			linkUI((void *) &emptyviaTouchLink, (void *) this);
		}


	};

	// UI-related module members

	int32_t numButton1Modes = 2;
	int32_t numButton2Modes = 2;
	int32_t numButton3Modes = 2;
	int32_t numButton4Modes = 2;
	int32_t numButton5Modes = 2;
	int32_t numButton6Modes = 2;
	int32_t numAux1Modes = 2;
	int32_t numAux2Modes = 2;
	int32_t numAux3Modes = 2;
	int32_t numAux4Modes = 2;

	void handleButton1ModeChange(int32_t);
	void handleButton2ModeChange(int32_t);
	void handleButton3ModeChange(int32_t);
	void handleButton4ModeChange(int32_t);
	void handleButton5ModeChange(int32_t);
	void handleButton6ModeChange(int32_t);
	void handleAux1ModeChange(int32_t);
	void handleAux2ModeChange(int32_t);
	void handleAux3ModeChange(int32_t);
	void handleAux4ModeChange(int32_t);

	/// An instance of the UI implementation.
	ViaEmptyviaUI emptyviaUI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the ui menu state machine.
	void ui_dispatch(int32_t sig) {
		this->emptyviaUI.dispatch(sig);
	};


	/// Go ahead and put all of your code in here!!

	//@{
	/// Event handlers calling the corresponding methods from the state machine.

	void mainRisingEdgeCallback(void) override {

	}
	void mainFallingEdgeCallback(void) override {

	}
	void auxRisingEdgeCallback(void) override {

	}
	void auxFallingEdgeCallback(void) override {

	}
	void buttonPressedCallback(void) override {

	}
	void buttonReleasedCallback(void) override {

	}
	// void ioProcessCallback(void) override {

	// }
	void halfTransferCallback(void) override {

	}
	void transferCompleteCallback(void) override {

	}
	void slowConversionCallback(void) override {

	}
	void auxTimer1InterruptCallback(void) override {

	}
	void auxTimer2InterruptCallback(void) override {

	}

	/// Initialization stuff

	void readCalibrationPacket(void) {

	}

	/// On construction, call subclass constructors and pass each a pointer to the module class.
	ViaEmptyvia() : emptyviaUI(*this) {

		/// Link the module GPIO registers.
		initializeAuxOutputs();

		// Old, decrepit io initialization
		/// Initialize the input stream buffers.
		inputs.init(VIA_EMPTYVIA_OVERSAMPLE);
		/// Initialize the output stream buffers.
		outputs.init(VIA_EMPTYVIA_OVERSAMPLE);
		/// Set the data members that will be used to determine DMA stream initialization in the hardware executable.
		outputBufferSize = VIA_EMPTYVIA_OVERSAMPLE;
		inputBufferSize = 1;

		/// Call the UI initialization that needs to happen after outer class construction.
		emptyviaUI.initialize();

		// Implemeting calibration and Viaflash firmware recognition would go like this:
		// uint32_t optionBytes = readOptionBytes();
		// uint32_t ob1Data = optionBytes & 0xFFFF;
		// uint32_t ob2Data = optionBytes >> 16;

		// if (ob1Data == 254 && ob2Data == 255) {
		// 	readCalibrationPacket();
		// 	emptyviaUI.writeStockPresets();
		// 	writeOptionBytes(FIRMWAREID, FIRMWAREVERSION);
		// } else if (ob1Data == FIRMWAREID) {
		// 	readCalibrationPacket();
		// } else if (ob1Data != 0) {
		// 	writeOptionBytes(0, 0);
		// }

	}

};

#endif /* INC_EMPTYVIA_HPP_ */
