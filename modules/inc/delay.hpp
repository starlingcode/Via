/** \file delay.hpp
 * \brief delay implementation.
 *
 *  A module implementing a simple testing/calibration helper program that serves as a template.
 */
#ifndef INC_Delay_HPP_
#define INC_Delay_HPP_

#include "user_interface.hpp"
#include <via_platform_binding.hpp>

class Delay {

private:
	int32_t * delayLine;
	int32_t delayWrite = 0;
	// int32_t lastReadPosition = 0;
	// int32_t lastSample = 0;

public:

	int32_t delayLength = 4096;

	void init(void) {
		delayLine = (int32_t *) malloc(sizeof(int32_t) * delayLength);
	}

	// DC blocker from https://github.com/pichenettes/stmlib/blob/master/dsp/filter.h
	// and https://www.dsprelated.com/freebooks/filters/DC_Blocker.html

	int32_t dcBlockLastIn;
	int32_t dcBlockLastOut;
#define DC_BLOCK_COEFFICIENT 65208

	int32_t processDCBlocker(int32_t input) {
		int32_t output = input - dcBlockLastIn + fix16_mul(DC_BLOCK_COEFFICIENT, dcBlockLastOut);
		dcBlockLastIn = input;
		dcBlockLastOut = output;
		return output;
	}


	int32_t delayTime = 65536;
	int32_t feedback = 0;

	inline int32_t read() {
		int32_t readPosition = ((delayWrite) << 16) - delayTime;
		readPosition &= (delayLength << 16) - 1;

		int32_t readIndex = readPosition >> 16;
		int32_t readFractional = readPosition & 0xFFFF;

		return fast_15_16_lerp(delayLine[readIndex], delayLine[(readIndex + 1) & (delayLength - 1)], readFractional);

	}

	inline void write(int32_t input, int32_t feedbackSample) {
		delayWrite += 1;
		delayWrite &= delayLength - 1;

		delayLine[delayWrite] = __SSAT(input +
									fix16_mul(processDCBlocker(feedbackSample),
											feedback),
												15);
	}

};

/// Macro used to specify the number of samples to per DAC transfer.
#define DELAY_BUFFER_SIZE 1

/// Callback to link to the C code in the STM32 Touch Sense Library.
void delayTouchLink (void *);

/// Calibration/template module class.
/** A simple self calibration tool that doubles as an introductory template.*/
class ViaDelay : public TARGET_VIA {

public:

	/// Calibration UI implementation.
	/**
	 * Override virtual methods in the UI drivers to implement the touch interface.
	 * I believe this should use template meta-programming instead of virtual functions because specific functionality is determined at compile time.
	 * One C++ trick at a time for now.
	 **/

	class ViaDelayUI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		ViaDelay& this_module;


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

		/// A method to handle any initialization that needs to be done after some or all of the outer class constructor.
		void initialize(void) override;

		/// A utility method to write the factory presets from memory.
		void writeStockPresets(void) override {}

		/// On construction, link the calibTouchLink callback to the STM32 touch sense library.
		ViaDelayUI(ViaDelay& x): this_module(x) {
			linkUI((void *) &delayTouchLink, (void *) this);
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
	ViaDelayUI delayUI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the touch sense library.
	/// Called in the executable code in user_interrupts.c.
	/// For some reason I have it wrapped like this?
	/// Perhaps the C code in the hardware executable mangled the namespace? Gotta check.
	void ui_dispatch(int32_t sig) {
		this->delayUI.dispatch(sig);
	};


	/*
	 *
	 * Processing functions
	 *
	 */

	void render(int32_t writePosition);

	Delay delay;

	int32_t burstCounter = 0;
	int32_t lfsrState = 1;
	void advanceLFSR(void) {
		lfsrState ^= lfsrState << 13;
		lfsrState ^= lfsrState << 17;
		lfsrState ^= lfsrState << 5;
		lfsrState &= 65535;
	}

	/// Instance of the exponential converter class.
	ExpoConverter revExpo;

	int32_t feedbackModOn = 0;
	int32_t delayModOn = 1;

	int32_t delayTimeTarget = 0;
	int32_t slewFactor = 0;
	int32_t lastTarget = 0;
	int32_t upsamplesLeft = 0;

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {

		burstCounter = 128;

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
		render(0);
	}
	void transferCompleteCallback(void) {
		render(DELAY_BUFFER_SIZE);
	}
	void slowConversionCallback(void) {
		upsamplesLeft = 8;
		controls.updateExtra();
		int32_t rawControls = 4095 - __USAT(controls.cv1Value - 2048 + controls.knob1Value, 12);
		lastTarget = delayTimeTarget;
		delayTimeTarget = revExpo.convert(rawControls);
		delayTimeTarget = fix16_mul(delayTimeTarget, 160000);
		slewFactor = (delayTimeTarget - lastTarget) >> 3;

	}
	void auxTimer1InterruptCallback(void) {

	}
	void auxTimer2InterruptCallback(void) {

	}

	/// On construction, call subclass constructors and pass each a pointer to the module class.
	ViaDelay() : delayUI(*this) {

		/// Link the module GPIO registers.
		initializeAuxOutputs();

		/// Initialize the input stream buffers.
		inputs.init(DELAY_BUFFER_SIZE);
		/// Initialize the output stream buffers.
		outputs.init(DELAY_BUFFER_SIZE);
		/// Set the data members that will be used to determine DMA stream initialization in the hardware executable.
		outputBufferSize = DELAY_BUFFER_SIZE;
		inputBufferSize = 1;

		delay.init();

		/// Call the UI initialization that needs to happen after outer class construction.
		delayUI.initialize();


	}

};

#endif /* INC_Calib_HPP_ */
