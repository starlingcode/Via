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
#define CALIB_BUFFER_SIZE 8

/// Callback to link to the C code in the STM32 Touch Sense Library.
void calibTouchLink (void *);

/// Calibration/template module class.
/** A simple self calibration tool that doubles as an introductory template.*/
class ViaCalib : public ViaModule {

public:

	/// Calibration UI implementation.
	/**
	 * Override virtual methods in the UI drivers to implement the touch interface.
	 * I believe this should use template meta-programming instead of virtual functions because specific functionality is determined at compile time.
	 * One C++ trick at a time for now.
	 **/

	class ViaCalibUI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		ViaCalib& this_module;


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
		ViaCalibUI(ViaCalib& x): this_module(x) {
			linkUI((void *) &calibTouchLink, (void *) this);
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
	ViaCalibUI calibUI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the touch sense library.
	/// Called in the executable code in user_interrupts.c.
	/// For some reason I have it wrapped like this?
	/// Perhaps the C code in the hardware executable mangled the namespace? Gotta check.
	void ui_dispatch(int32_t sig) {
		this->calibUI.dispatch(sig);
	};

	/// Base class for the implementation of different calibration stages.
	/**
	 * Each calibration stage has a different way of handling the module's runtime events.
	 * This class lays out the different handlers that are used and defaults to a NOP if not overridden.
	 */
	class CalibrationState {
	public:

		virtual void slowConversionHandler(void) {}
		virtual void mainRisingEdgeHandler(void) {}
		virtual void auxRisingEdgeHandler(void) {}
		virtual void mainFallingEdgeHandler(void) {}
		virtual void auxFallingEdgeHandler(void) {}
		virtual void buttonPressedHandler(void) {}
		virtual void dacHalfTransferHandler(void) {}
		virtual void dacTransferHandler(void) {}
		virtual void advanceState(void) {}

	};

	/// Module state at power on, advance state when all sensors have detected a press event.
	class CheckTouchState: public CalibrationState {
	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for implementation.
		 */
		ViaCalib& this_module;

		//@{
		/// Set A and B level to full scale and signal out to virtual ground.
		void dacHalfTransferHandler(void) override {
			this_module.renderFixedOutputs(0);
		}
		void dacTransferHandler(void) override {
			this_module.renderFixedOutputs(CALIB_BUFFER_SIZE);
		}
		//@}

		/// Light up the green LED with a button press.
		void buttonPressedHandler(void) override {
			this_module.setGreenLED(1024);
		}

		/// Clear the LEDs and switch the module state to the test and trim stage.
		void advanceState(void) override {
			this_module.clearLEDs();
			this_module.clearRGB();
			this_module.currentState = &(this_module.testAndTrim);
		}

		/// Define the pointer to the outer class.
		CheckTouchState(ViaCalib& x): this_module(x) {}

	};

	/// Generate test signals at the outputs and display LED indication of the inputs.
	class TestAndTrimState: public CalibrationState {
	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for implementation.
		 */
		ViaCalib& this_module;

		/// Update the knob averages, execute the CV1 self tune routine, update the test sine frequency, advance the LFOs, and check if CV2 and CV3 are operational.
		void slowConversionHandler(void) override {
			this_module.controls.update();
			this_module.cv1TunerExecute();
			this_module.sineFreq = this_module.expo.convert(this_module.controls.cv1Value) >> 1;
			this_module.advanceLFO();
			this_module.verifyCV2CV3();
		}
		//@{
		/// Mirror the logic level at the main logic input on the left LEDs.
		void mainRisingEdgeHandler(void) override {
			this_module.setLEDA(1);
			this_module.setLEDB(1);
		}
		void mainFallingEdgeHandler(void) override {
			this_module.setLEDA(0);
			this_module.setLEDB(0);
		}
		//@}
		//@{
		/// Mirror the logic level at the aux logic input on the right LEDs.
		void auxRisingEdgeHandler(void) override {
			this_module.setLEDC(1);
			this_module.setLEDD(1);
		}
		void auxFallingEdgeHandler(void) override {
			this_module.setLEDC(0);
			this_module.setLEDD(0);
		}
		//@}
		/// The touch button is used to advance state.
		void buttonPressedHandler(void) override {
			advanceState();
		}
		//@{
		/// Render test oscillator outputs.
		void dacHalfTransferHandler(void) override {
			this_module.renderTestOutputs(0);
		}
		void dacTransferHandler(void) override {
			this_module.renderTestOutputs(CALIB_BUFFER_SIZE);
		}
		//@}
		///
		void advanceState(void) override {
			this_module.clearLEDs();
			this_module.clearRGB();
			this_module.currentState = &(this_module.calibrateCV);
		}

		/// Define the pointer to the outer class.
		TestAndTrimState(ViaCalib& x): this_module(x) {}

	};

	/// Measure the CV offsets with no signals connected.
	class CalibrateCVState: public CalibrationState {
	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for implementation.
		 */
		ViaCalib& this_module;

		//@{
		/// Render fixed outputs and measure the inputs.
		void dacHalfTransferHandler(void) override {
			this_module.renderFixedOutputs(0);
			this_module.measureCVOffsets();
		}
		void dacTransferHandler(void) override {
			this_module.renderFixedOutputs(CALIB_BUFFER_SIZE);
		}
		//@}
		/// The touch button is used to advance state.
		void buttonPressedHandler(void) override {
			advanceState();
		}
		//@{
		/// Advance to the stage where the dac output is calibrated.
		void advanceState(void) override {
			this_module.currentState = &(this_module.calibrateDACStore);
			this_module.cvCalibCounter = 0;
		}
		/// Update the controls.
		void slowConversionHandler(void) override {
			this_module.controls.update();
		}

		/// Define the pointer to the outer class.
		CalibrateCVState(ViaCalib& x): this_module(x) {}

	};

	class CalibrateDACStoreState: public CalibrationState {
	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for implementation.
		 */
		ViaCalib& this_module;

		//@{
		/// Render fixed outputs.
		void dacHalfTransferHandler(void) override {
			this_module.renderFixedOutputs(0);
		}
		void dacTransferHandler(void) override {
			this_module.renderFixedOutputs(CALIB_BUFFER_SIZE);
		}
		//@}
		/// The touch button is used to advance state.
		void buttonPressedHandler(void) override {
			advanceState();
		}
		/// Store the calibration data and write the option bytes, resetting the MCU.
		void advanceState(void) override {
			this_module.calibUI.storeToEEPROM(7, this_module.calibrationPacket);
			this_module.writeOptionBytes(254, 255);
		}
		/// Update the controls.
		void slowConversionHandler(void) override {
			this_module.controls.update();
			this_module.measureDAC3Offset();
		}

		/// Define the pointer to the outer class.
		CalibrateDACStoreState(ViaCalib& x): this_module(x) {}

	};

	/// Pointer to the base state class used to implement the polymorphism.
	CalibrationState * currentState;

	//@{
	/// Instances of the specific state subclasses.
	CheckTouchState checkTouch;
	TestAndTrimState testAndTrim;
	CalibrateCVState calibrateCV;
	CalibrateDACStoreState calibrateDACStore;
	//@}

	/*
	 *
	 * State functions
	 *
	 */

	/// Fill the dac buffers with fixed outputs.
	void renderTestOutputs(int32_t writePosition);
	/// Render and write the test audio outputs, write the test LFO to the signal output.
	void renderFixedOutputs(int32_t writePosition);

	/// Instance of a sine oscillator.
	Sine oscillator;
	//@{
	/// Data members that should be encapsulated in Sine.
	uint32_t sineFreq = 0;
	uint32_t sinePhase = 0;
	//@}

	/// Instance of the exponential converter class.
	ExpoConverter expo;

	/// Method to advance the LFO.
	void advanceLFO(void);

	//@{
	/// Data members that should be encapsulated in a class with the LFO.
	uint32_t lfoPhasor = 0;
	uint32_t lfoIncrement = 0;
	//@}

	//@{
	/// Parameters for scaling and biasing the test LFO signal output.
	int32_t dac3Base = 0;
	int32_t dac3Range = 0;
	uint32_t dac3Output = 0;
	//@}

	/// Method to check a jump at the CV1 against a perfect octave span.
	void cv1TunerExecute(void);

	//@{
	/// Data members for the tuner.
	enum tunerStates {resting, changeDetected, averaging, measuring};
	int32_t tunerState = 0;
	int32_t lastAverage = 0;
	int32_t baseCV1 = 0;
	int32_t extraCV1Counter = 0;
	int32_t extraCV1Sum = 0;
	//@}

	/// Simple method to check the status of the sigma delta ADCs.
	void verifyCV2CV3(void);

	//@{
	/// Data member for the CV check.
	int32_t cv2High = 0;
	int32_t cv3High = 0;
	//@}

	//@{
	/// Simple methods to average and store the CV offsets.
	void measureCVOffsets(void);
	void measureDAC3Offset(void);
	//@}

	//@{
	/// Data members to average and store CV offsets.
	uint32_t calibTest = 0;
	uint32_t calibrateDAC3 = 0;
	uint32_t cvCalibCounter = 0;
	int32_t cv1Sum = 0;
	int32_t cv2Sum = 0;
	int32_t cv3Sum = 0;
	//@}

	//@{
	/// Calibration data.
	int32_t cv1Read = 0;
	int32_t cv2Read = 0;
	int32_t cv3Read = 0;
	int32_t dac3Offset = 0;
	//@}

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {
		currentState->mainRisingEdgeHandler();
	}
	void mainFallingEdgeCallback(void) {
		currentState->mainFallingEdgeHandler();
	}
	void auxRisingEdgeCallback(void) {
		currentState->auxRisingEdgeHandler();
	}
	void auxFallingEdgeCallback(void) {
		currentState->auxFallingEdgeHandler();
	}
	void buttonPressedCallback(void) {
		currentState->buttonPressedHandler();
	}
	void buttonReleasedCallback(void) {}
	void ioProcessCallback(void) {}
	void halfTransferCallback(void) {
		currentState->dacHalfTransferHandler();
	}
	void transferCompleteCallback(void) {
		currentState->dacTransferHandler();
	}
	void slowConversionCallback(void) {
		currentState->slowConversionHandler();
	}
	void auxTimer1InterruptCallback(void) {}
	void auxTimer2InterruptCallback(void) {}
	//@}

	/// On construction, call subclass constructors and pass each a pointer to the module class.
	ViaCalib() : calibUI(*this), checkTouch(*this), testAndTrim(*this),
			calibrateCV(*this), calibrateDACStore(*this) {

		/// Link the module GPIO registers.
		initializeAuxOutputs();

		/// Initialize the input stream buffers.
		inputs.init(CALIB_BUFFER_SIZE);
		/// Initialize the output stream buffers.
		outputs.init(CALIB_BUFFER_SIZE);
		/// Set the data members that will be used to determine DMA stream initialization in the hardware executable.
		outputBufferSize = CALIB_BUFFER_SIZE;
		inputBufferSize = 1;

//		/// Initialize the buffers for the averages.
//		for (int i = 0; i < 256; i++) {
//			extraCV1Buffer.buff[i] = 0;
//		}

		/// Call the UI initialization that needs to happen after outer class construction.
		calibUI.initialize();

		/// Initialize the state machine.
		currentState = &checkTouch;

	}

};

#endif

#endif /* INC_Calib_HPP_ */
