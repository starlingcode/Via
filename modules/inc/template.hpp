/** \file calib.hpp
 * \brief Calibration module implementation.
 *
 *  A module implementing a simple testing/calibration helper program that serves as a template.
 */
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
class ViaTemplate : public ViaModule {

public:

	/// Calibration UI implementation.
	/**
	 * Override virtual methods in the UI drivers to implement the touch interface.
	 * I believe this should use template meta-programming instead of virtual functions because specific functionality is determined at compile time.
	 * One C++ trick at a time for now.
	 **/

	class ViaTemplateUI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		ViaTemplate& this_module;


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
		ViaTemplateUI(ViaTemplate& x): this_module(x) {
			linkUI((void *) &templateTouchLink, (void *) this);
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
	ViaTemplateUI templateUI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the touch sense library.
	/// Called in the executable code in user_interrupts.c.
	/// For some reason I have it wrapped like this?
	/// Perhaps the C code in the hardware executable mangled the namespace? Gotta check.
	void ui_dispatch(int32_t sig) {
		this->templateUI.dispatch(sig);
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

	int32_t byteBeatCounter = 689854738;
	int32_t byteBeatResult = 0;

	inline void byteBeatExecute(int32_t t) {
		byteBeatResult = (t&48>>t%13)&(((t%64==0)?t>>2:t<<2)>>((t%64)/16))&((t%6==0)?(37&t>>11):(11&t<<37))|(t&t>>5)&(t*3<<4)&((t&2048)?((t>>11)/4):(t>>7))-2;
//		byteBeatResult <<= 16;
		int32_t timeCV = (int32_t) inputs.cv3Samples[0];
		byteBeatCounter += __USAT((1 + controls.knob3) + timeCV >> 4, 24);
	}

//	inline void byteBeatExecute(int32_t t) {
//		oscillator2.freq = oscillator.freq << 1;
//		oscillator3.freq = oscillator.freq << 2;
//		int32_t modulator = oscillator3.evaluatePMFM(((t%511)^(t&t%255)|(t&t%511)|(t&t%1023)|(t|t%2047)) >> 16);
////		int32_t modulator = oscillator3.evaluatePM();
//		byteBeatResult = oscillator2.evaluatePMFM((modulator - 16383) << 1) << 16;
//		int32_t timeCV = (int32_t) inputs.cv3Samples[0];
//		byteBeatCounter += __USAT(((1 + controls.knob3) << 12) + (timeCV << 8), 24);
//	}

//	inline int32_t phaseindex4drums(unsigned long t){
//		int iDrums;
//		if (t<(2*1024*1024)){iDrums = 3;}
//		else if (t<(4*1024*1024)){iDrums = 6;}
//		else if (t<(6*1024*1024)){iDrums = 5;}
//		else if (t<(8*1024*1024)){iDrums = 9;}
//		else if (t<(10*1024*1024)){iDrums = 8;}
//		else if (t<(12*1024*1024)){iDrums = 7;}
//		else if (t<(14*1024*1024)){iDrums = 4;}
//		else {iDrums = 10;}
//
//
//
//		return t>>iDrums;
//	}
//
//	inline void byteBeatExecute(int32_t t) {
//		oscillator2.freq = oscillator.freq >> 1;
//		oscillator3.freq = oscillator.freq << 4;
//		int32_t modulator = oscillator3.evaluatePMFM(((phaseindex4drums(t)|(t>>(4-(1^7&(t>>14))))|(9015+(t%8192)?((t*9)%4096):t/2)/2)) >> 16);
////		int32_t modulator = oscillator3.evaluatePM();
//		byteBeatResult = oscillator2.evaluatePMFM((modulator - 16383) << 3) << 16;
//		int32_t timeCV = (int32_t) inputs.cv3Samples[0];
//		byteBeatCounter += __USAT(((1 + controls.knob3) << 4) + timeCV, 24);
//	}

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
		byteBeatCounter = 0;
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
				expo.convert(controls.cv1Value) >> 4);
//		oscillator.freq = fix16_mul(oscillator.freq, 65535 + (controls.knob2Value << 4));
	}
	void auxTimer1InterruptCallback(void) {

	}
	void auxTimer2InterruptCallback(void) {

	}

	/// On construction, call subclass constructors and pass each a pointer to the module class.
	ViaTemplate() : templateUI(*this) {

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
		templateUI.initialize();


	}

};

#endif

#endif /* INC_Calib_HPP_ */
