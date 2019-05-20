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

	void (ViaOsc::*render)(int32_t writePosition);

	void renderSaw(int32_t writePosition);
	void renderSquare(int32_t writePosition);
	void renderTri(int32_t writePosition);
	void renderTrap(int32_t writePosition);

	int32_t detuneBase = 0;
	int32_t detune = 0;
	int32_t pm = 0;
	int32_t pmTracker = 0;
	int32_t aFreq = 0;
	int32_t bFreq = 0;
	int32_t cFreq = 0;
	uint32_t aPhase = 0;
	uint32_t bPhase = 0;
	uint32_t cPhase = 0;
	int32_t aBasePitch = 0;
	int32_t bBasePitch = 0;
	int32_t cBasePitch = 0;

	int32_t octave = 0;
	int32_t octaveRange = 0;
	int32_t unity = 0;

	int32_t lastLogicA = 0;
	int32_t lastLogicB = 0;
	int32_t lastPM = 0;

	int32_t shAOn = 1;
	int32_t shBOn = 1;

	int32_t chromatic[128] = {0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122};
	int32_t major[128] = {0, 0, 0, 0, 0, 0, 2, 2, 4, 5, 5, 7, 7, 9, 9, 11, 12, 12, 14, 14, 16, 17, 17, 19, 19, 21, 21, 23, 24, 24, 26, 26, 28, 29, 29, 31, 31, 33, 33, 35, 36, 36, 38, 38, 40, 41, 41, 43, 43, 45, 45, 47, 48, 48, 50, 50, 52, 53, 53, 55, 55, 57, 57, 59, 60, 60, 62, 62, 64, 65, 65, 67, 67, 69, 69, 71, 72, 72, 74, 74, 76, 77, 77, 79, 79, 81, 81, 83, 84, 84, 86, 86, 88, 89, 89, 91, 91, 93, 93, 95, 96, 96, 98, 98, 100, 101, 101, 103, 103, 105, 105, 107, 108, 108, 110, 110, 112, 113, 113, 115, 115, 117, 117, 119, 120, 120, 122};
	int32_t minor[128] = {0, 0, 0, 0, 0, 0, 2, 2, 3, 5, 5, 7, 7, 8, 8, 10, 12, 12, 14, 14, 15, 17, 17, 19, 19, 20, 20, 22, 24, 24, 26, 26, 27, 29, 29, 31, 31, 32, 32, 34, 36, 36, 38, 38, 39, 41, 41, 43, 43, 44, 44, 46, 48, 48, 50, 50, 51, 53, 53, 55, 55, 56, 56, 58, 60, 60, 62, 62, 63, 65, 65, 67, 67, 68, 68, 70, 72, 72, 74, 74, 75, 77, 77, 79, 79, 80, 80, 82, 84, 84, 86, 86, 87, 89, 89, 91, 91, 92, 92, 94, 96, 96, 98, 98, 99, 101, 101, 103, 103, 104, 104, 106, 108, 108, 110, 110, 111, 113, 113, 115, 115, 116, 116, 118, 120, 120, 122};

	int32_t * scale = chromatic;
	int32_t scaleMode = 0;

	int32_t chordMode = 0;

	int32_t beat = 1;

	inline void beatDetect(uint32_t aPhase, uint32_t bPhase) {
		int32_t aQuadrant = aPhase >> 30;
		int32_t bQuadrant = bPhase >> 30;
		if (beat) {
			beat = (abs(aQuadrant - bQuadrant) != 2);
		} else {
			beat = (aQuadrant == bQuadrant);
		}
	}

	void linearDetune(int32_t detuneMod) {

		detune = (detuneBase + detuneMod) * (detuneBase != 0);

		aFreq = (cBasePitch << (octave * octaveRange)) + (detune >> 2) * !unity;
		bFreq = (cBasePitch << (octave * octaveRange)) - (detune >> 2) * !unity;

	}

	void scaledDetune(int32_t detuneMod) {

		detune = (detuneBase + detuneMod) * (detuneBase != 0);

		aFreq = fix16_mul(cBasePitch << (octave * octaveRange), 65536 - (detune >> 5) * !unity);
		bFreq = fix16_mul(cBasePitch << (octave * octaveRange), 65536 + (detune >> 5) * !unity);

	}

	void chordalDetune(int32_t detuneMod) {

		aFreq = aBasePitch << (octave * octaveRange);
		bFreq = bBasePitch << (octave * octaveRange);

	}

	void (ViaOsc::*doDetune)(int32_t detuneMod);

	inline void updateFrequencies(void) {

		(this->*doDetune)((int32_t) -inputs.cv3Samples[0]);

		cFreq = cBasePitch << (octave * octaveRange);

		int32_t pmInput = inputs.cv2Samples[0];

		// quadrature
		int32_t aError = aPhase - (cPhase + pmTracker + (1 << 30));
		int32_t bError = bPhase - (cPhase + pmTracker + (1 << 31));

		aFreq += __SSAT(aError * ((detuneBase == 0)), 12);
		bFreq += __SSAT(bError * ((detuneBase == 0)), 12);

		aFreq += __SSAT(aError * (unity), 14);
		bFreq += __SSAT(bError * (unity), 14);

		pm = (pmInput - lastPM) << 11;

		pmTracker += pm;

		lastPM = pmInput;

	}

	inline void parsePhase(int32_t aPhaseWorker, int32_t bPhaseWorker, int32_t cPhaseWorker) {

		int32_t resample = (cPhase > (0xFFFFFFFF - (1 << 30))) & (cPhaseWorker < (1 << 30));

		beatDetect(aPhaseWorker, bPhaseWorker);

		aPhase = aPhaseWorker;
		bPhase = bPhaseWorker;
		cPhase = cPhaseWorker;

		outputs.logicA[0] = GET_ALOGIC_MASK(beat * (detuneBase != 0) * (unity == 0));
		outputs.auxLogic[0] = 0;
		outputs.shA[0] = GET_SH_A_MASK(resample * shAOn);
		outputs.shB[0] = GET_SH_B_MASK(resample * shBOn);

	}

//	int32_t tableRead[4095];

	/// Instance of the exponential converter class.
	ExpoConverter expo;

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {

		octave = 1;

	}
	void mainFallingEdgeCallback(void) {

		octave = 0;

	}
	void auxRisingEdgeCallback(void) {

		unity = 1;

	}
	void auxFallingEdgeCallback(void) {
		unity = 0;
	}
	void buttonPressedCallback(void) {
	}
	void buttonReleasedCallback(void) {}
	void ioProcessCallback(void) {}
	void halfTransferCallback(void) {
		setLogicOut(0, runtimeDisplay);
		(this->*render)(0);
	}
	void transferCompleteCallback(void) {
		setLogicOut(0, runtimeDisplay);
		(this->*render)(TEMPLATE_BUFFER_SIZE);
	}
	void slowConversionCallback(void) {
		controls.updateExtra();
		int32_t cv1Index;
		int32_t knob1Index;
		if (scaleMode) {
			cv1Index = scale[controls.cv1Value >> 5] << 5;
			knob1Index = ((controls.knob1Value * 3) >> 2) & 0b111111100000;
		} else {
			cv1Index = __USAT((controls.cv1Value - (4 << 5)), 12);
			knob1Index = ((controls.knob1Value * 3) >> 2);
		}
		if (chordMode) {

			int32_t fineTune = 65535 + (controls.knob2Value << 3);
			int32_t coarseTune = expo.convert(knob1Index) >> 3;
			int32_t chord = __USAT((controls.knob3Value << 4) + (int32_t) -inputs.cv3Samples[0], 16) >> 12;

			int32_t chordTranspose = 0;

			if ((knob1Index >> 5) < 52) {
				chordTranspose = (52 - (knob1Index >> 5)) / 12;
			}

			cBasePitch = fix16_mul(coarseTune,
					expo.convert(cv1Index) >> 2);
			cBasePitch = fix16_mul(cBasePitch, 65762);
			cBasePitch = fix16_mul(cBasePitch, fineTune);

			int32_t chordMultiplier = scale[__USAT((controls.cv1Value >> 5) + chord, 8)] << 5;

			aBasePitch = fix16_mul(coarseTune, expo.convert(chordMultiplier) >> 2);
			aBasePitch = fix16_mul(aBasePitch, 65762);
			aBasePitch = fix16_mul(aBasePitch, fineTune) << chordTranspose;

			chordMultiplier = scale[__USAT((controls.cv1Value >> 5) - chord, 8)] << 5;

			bBasePitch = fix16_mul(coarseTune, expo.convert(chordMultiplier) >> 2);
			bBasePitch = fix16_mul(bBasePitch, 65762);
			bBasePitch = fix16_mul(bBasePitch, fineTune) << chordTranspose;

			detuneBase = 2000;

		} else {
			cBasePitch = fix16_mul(expo.convert(knob1Index) >> 3,
					expo.convert(cv1Index) >> 2);
			cBasePitch = fix16_mul(cBasePitch, 65762);
			cBasePitch = fix16_mul(cBasePitch, 65535 + (controls.knob2Value << 3));
			detuneBase = controls.knob3Value << 4;
		}

		if (runtimeDisplay) {
			setRedLED(hueSpace[scaleMode * 4].r);
			setGreenLED(hueSpace[scaleMode * 4].g);
			setBlueLED(hueSpace[scaleMode * 4].b);
		}

	}
	void auxTimer1InterruptCallback(void) {

	}
	void auxTimer2InterruptCallback(void) {

	}

	int32_t numButton1Modes = 2;
	int32_t numButton2Modes = 4;
	int32_t numButton3Modes = 2;
	int32_t numButton4Modes = 4;
	int32_t numButton5Modes = 4;
	int32_t numButton6Modes = 2;

	void handleButton1ModeChange(int32_t);
	void handleButton2ModeChange(int32_t);
	void handleButton3ModeChange(int32_t);
	void handleButton4ModeChange(int32_t);
	void handleButton5ModeChange(int32_t);
	void handleButton6ModeChange(int32_t);

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

		render = &ViaOsc::renderTri;
		doDetune = &ViaOsc::linearDetune;

		/// Call the UI initialization that needs to happen after outer class construction.
		oscUI.initialize();


	}

};

#endif

#endif

#endif /* INC_Calib_HPP_ */
