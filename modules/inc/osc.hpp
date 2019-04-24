/** \file calib.hpp
 * \brief Calibration module implementation.
 *
 *  A module implementing a simple testing/calibration helper program that serves as a template.
 */

#ifdef BUILD_F373

/// Macro used to specify the number of samples to per DAC transfer.
#define OSC_BUFFER_SIZE 32

/// Callback to link to the C code in the STM32 Touch Sense Library.
void oscTouchLink (void *);

#ifndef INC_Calib_HPP_
#define INC_Calib_HPP_

#include "user_interface.hpp"
#include <via_platform_binding.hpp>
#include <oscillators.hpp>

#ifdef BUILD_F373

/// Macro used to specify the number of samples to per DAC transfer.
#define TEMPLATE_BUFFER_SIZE 32

/// Callback to link to the C code in the STM32 Touch Sense Library.
void templateTouchLink (void *);

/// Calibration/template module class.

/** A simple self calibration tool that doubles as an introductory template.*/
class ViaOsc : public ViaModule {

public:

	/// Calibration UI implementation.
	/**
	 * Override virtual methods in the UI drivers to implement the touch interface.
	 * I believe this should use template meta-programming instead of virtual functions because specific functionality is determined at compile time.
	 * One C++ trick at a time for now.
	 **/

	class ViaOscUI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		ViaOsc& this_module;


		//@{
		/// Functions to handle a tap event on a touch sensor or shift combo.
		void button1TapCallback(void) override;
		void button2TapCallback(void) override;
		void button3TapCallback(void) override;
		void button4TapCallback(void) override;
		void button5TapCallback(void) override;
		void button6TapCallback(void) override;

		void aux1TapCallback(void) override;
		void aux2TapCallback(void) override;
		void aux2AltTapCallback(void) override;
		void aux3TapCallback(void) override;
		void aux4TapCallback(void) override;
		//@}

		//@{
		/// Functions to handle a hold event on a touch sensor or shift combo.
		void button1HoldCallback(void) override;
		void button2HoldCallback(void) override;
		void button3HoldCallback(void) override;
		void button4HoldCallback(void) override;
		void button5HoldCallback(void) override;
		void button6HoldCallback(void) override;

		void aux1HoldCallback(void) override;
		void aux2HoldCallback(void) override;
		void aux2AltHoldCallback(void) override;
		void aux3HoldCallback(void) override;
		void aux4HoldCallback(void) override;
		//@}

		//@{
		/// A method to access the LED control functions of this_module in the UI driver.
		void uiSetLEDs(int) override;
		//@}

		//@{
		/// A method to recall the module's last state at power on or restore presets.
		void recallModuleState(void) override;
		//@}

		//@{
		/// Methods to handle entry transitions into the main module states.
		void defaultEnterMenuCallback(void) override;
		void newModeEnterMenuCallback(void) override;
		void newAuxModeEnterMenuCallback(void) override;
		void presetEnterMenuCallback(void) override;
		//@}

		//@{
		/// Methods to handle entry transitions into touch sensor menu states.
		void button1EnterMenuCallback(void) override;
		void button2EnterMenuCallback(void) override;
		void button3EnterMenuCallback(void) override;
		void button4EnterMenuCallback(void) override;
		void button5EnterMenuCallback(void) override;
		void button6EnterMenuCallback(void) override;

		void aux1EnterMenuCallback(void) override;
		void aux2EnterMenuCallback(void) override;
		void aux2AltEnterMenuCallback(void) override;
		void aux3EnterMenuCallback(void) override;
		void aux4EnterMenuCallback(void) override;
		//@}

		/// A method to handle any initialization that needs to be done after some or all of the outer class constructor.
		void initialize(void) override;

		/// A utility method to write the factory presets from memory.
		void writeStockPresets(void) override {}

		/// On construction, link the calibTouchLink callback to the STM32 touch sense library.
		ViaOscUI(ViaOsc& x): this_module(x) {
			linkUI((void *) &oscTouchLink, (void *) this);
		}

		//@{
		/// Some firmware-specific data members used to register touch events on each sensor for testing.
		int32_t touch1OK = 0;
		int32_t touch2OK = 0;
		int32_t touch3OK = 0;
		int32_t touch4OK = 0;
		int32_t touch5OK = 0;
		int32_t touch6OK = 0;
		///

	};

	/// An instance of the UI implementation.
	ViaOscUI oscUI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the touch sense library.
	/// Called in the executable code in user_interrupts.c.
	/// For some reason I have it wrapped like this?
	/// Perhaps the C code in the hardware executable mangled the namespace? Gotta check.
	void ui_dispatch(int32_t sig) {
		this->oscUI.dispatch(sig);
	};


	/*
	 *
	 * State functions
	 *
	 */

	/// Fill the dac buffers with fixed outputs.
	void renderTestOutputs(int32_t writePosition);

	/// Instance of a sine oscillator.
	Sine oscillator;
	Sine oscillator2;
	Sine oscillator3;

	int32_t * sharedTable;

	void loadTable(void) {
		sharedTable = (int32_t *) malloc(4095*sizeof(int32_t));
		for (int i; i < 4096; i++) {
			sharedTable[i] = oscillator.big_sine[i] | ((oscillator.big_sine[i + 1] - oscillator.big_sine[i]) << 16);
		}
		oscillator.tableRead = sharedTable;
		oscillator2.tableRead = sharedTable;
		oscillator3.tableRead = sharedTable;
	}

	/// Instance of the exponential converter class.
	ExpoConverter expo;

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {
		oscillator.phase = 0;
		oscillator2.phase = 0;
		oscillator3.phase = 0;
	}
	void mainFallingEdgeCallback(void) {
	}
	void auxRisingEdgeCallback(void) {

	}
	void auxFallingEdgeCallback(void) {
	}
	void buttonPressedCallback(void) {
	}
	void buttonReleasedCallback(void) {}
	void ioProcessCallback(void) {}
	void halfTransferCallback(void) {
		renderTestOutputs(0);
	}
	void transferCompleteCallback(void) {
		renderTestOutputs(TEMPLATE_BUFFER_SIZE);
	}
	void slowConversionCallback(void) {
		controls.update();
		oscillator.freq = fix16_mul(expo.convert((controls.knob1Value * 3) >> 2) >> 3,
				expo.convert(controls.cv1Value) >> 2);
		oscillator2.freq = (oscillator.freq * 8) / (16 - (controls.knob2 >> 8));
	}
	void auxTimer1InterruptCallback(void) {

	}
	void auxTimer2InterruptCallback(void) {

	}

	/// On construction, call subclass constructors and pass each a pointer to the module class.
	ViaOsc() : oscUI(*this) {

		/// Link the module GPIO registers.
		initializeAuxOutputs();

		/// Initialize the input stream buffers.
		inputs.init(TEMPLATE_BUFFER_SIZE);
		/// Initialize the output stream buffers.
		outputs.init(TEMPLATE_BUFFER_SIZE);
		/// Set the data members that will be used to determine DMA stream initialization in the hardware executable.
		outputBufferSize = TEMPLATE_BUFFER_SIZE;
		inputBufferSize = 1;

		loadTable();

		/// Call the UI initialization that needs to happen after outer class construction.
		oscUI.initialize();


	}

};

#endif

#endif

#endif /* INC_Calib_HPP_ */
