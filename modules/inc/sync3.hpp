/** \file sync3.hpp
 * \brief Calibration module implementation.
 *
 *  An empty Via sync3 for lots of hacking fun
 */


#ifndef INC_SYNC3_HPP_
#define INC_SYNC3_HPP_

#include "user_interface.hpp"
#include <via_platform_binding.hpp>
#include "oscillators.hpp"

/// Macro used to specify the number of samples to per DAC transfer.
#define VIA_SYNC3_BUFFER_SIZE 32

/// Callback to link to the C code in the STM32 Sync3 Sense Library.
void sync3TouchLink (void * uiVoid);

class ViaSync3 : public ViaModule {

public:

	typedef void (ViaSync3::*renderPointer)(int32_t writePosition);

	class ViaSync3UI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		ViaSync3& this_module;


//		//@{
//		/// Functions to handle a tap event on a sync3 sensor or shift combo.
		void button1TapCallback(void) override;
		void button2TapCallback(void) override;
		void button3TapCallback(void) override;
		void button4TapCallback(void) override;
		void button5TapCallback(void) override;
		void button6TapCallback(void) override;
//
//		void aux1TapCallback(void) override;
//		void aux2TapCallback(void) override;
//		void aux2AltTapCallback(void) override;
//		void aux3TapCallback(void) override;
//		void aux4TapCallback(void) override;
//		//@}
//
//		//@{
//		/// Functions to handle a hold event on a sync3 sensor or shift combo.
		void button1HoldCallback(void) override;
		void button2HoldCallback(void) override;
		void button3HoldCallback(void) override;
		void button4HoldCallback(void) override;
		void button5HoldCallback(void) override;
		void button6HoldCallback(void) override;
//
//		void aux1HoldCallback(void) override;
//		void aux2HoldCallback(void) override;
//		void aux2AltHoldCallback(void) override;
//		void aux3HoldCallback(void) override;
//		void aux4HoldCallback(void) override;
//		//@}

//		//@{
//		/// Methods to handle entry transitions into the main module states.
		void defaultEnterMenuCallback(void) override;
//		void newModeEnterMenuCallback(void) override;
//		void newAuxModeEnterMenuCallback(void) override;
//		void presetEnterMenuCallback(void) override;
//		//@}
//
//		//@{
//		/// Methods to handle entry transitions into sync3 sensor menu states.
		void button1EnterMenuCallback(void) override;
		void button2EnterMenuCallback(void) override;
		void button3EnterMenuCallback(void) override;
		void button4EnterMenuCallback(void) override;
		void button5EnterMenuCallback(void) override;
		void button6EnterMenuCallback(void) override;
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

		void recallModuleState(void) override;

		void uiSetLEDs(int mode) override {

			/// This connects the UI library's LED setting calls to the enumerated LED handler of the module.
			/// This could probably go away with some refactoring.

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

		/// On construction, link the calibSync3Link callback to the STM32 sync3 sense library.
		ViaSync3UI(ViaSync3& x): this_module(x) {
			linkUI((void *) &sync3TouchLink, (void *) this);
		}


	};

	/// An instance of the UI implementation.
	ViaSync3UI sync3UI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the sync3 sense library.
	/// Called in the executable code in user_interrupts.c.
	/// For some reason I have it wrapped like this?
	/// Perhaps the C code in the hardware executable mangled the namespace? Gotta check.
	void ui_dispatch(int32_t sig) {
		this->sync3UI.dispatch(sig);
	};

	uint32_t phase1 = 0;
	uint32_t phase2 = 0;
	uint32_t phase3 = 0;
	uint32_t phase4 = 0;
	uint32_t phases[VIA_SYNC3_BUFFER_SIZE];
	int32_t increment1 = 1000;
	int32_t increment2 = 1000;
	int32_t increment3 = 1000;
	int32_t increment4 = 1000;

	uint32_t phaseModOn = 0;
	int32_t lastPhaseMod = 0;
	int32_t phaseModIncrement2 = 0;
	int32_t phaseModIncrement3 = 0;
	uint32_t phaseModTracker2 = 0;
	uint32_t phaseModTracker3 = 0;

	uint32_t periodCount = 0;
	int32_t errorPileup = 0;
	int32_t phaseLockOn = 0;
	int32_t hardSync = 0;
	uint32_t subharm = 0;

	int32_t error = 0;

	inline void advanceSubharm(void) {
		subharm = (subharm + 1) & 1;
		setAuxLogic(subharm);
	}

	struct Sync3Scale {
		uint32_t numerators[32];
		uint32_t denominators[32];
		uint32_t dividedPhases[32];
		uint32_t keys[32];
		uint32_t pllStyle;
	};

	static const struct Sync3Scale perfect;
	static const struct Sync3Scale simpleRhythms;
	static const struct Sync3Scale ints;
	static const struct Sync3Scale circleFifths;
	static const struct Sync3Scale fourthsFifths;
	static const struct Sync3Scale minorArp;
	static const struct Sync3Scale evenOdds;
	static const struct Sync3Scale bP;

	static const struct Sync3Scale * scales[8];

	const uint32_t * numerators = ints.numerators;
	const uint32_t * denominators = ints.denominators;
	const uint32_t * dividedPhases = ints.dividedPhases;
	const uint32_t * keys = ints.keys;

	inline void displayRatio(void) {
		setRGB(hueSpace[sync3UI.button2Mode * 2]);
	}

	int32_t sync1Div = 0;
	int32_t sync2Div = 0;
	int32_t sync3Div = 0;

	uint32_t numerator1 = 1;
	uint32_t numerator2 = 1;
	uint32_t numerator3 = 1;

	uint32_t numerator1Select = 1;
	uint32_t numerator2Select = 1;
	uint32_t numerator3Select = 1;

	uint32_t denominator1 = 1;
	uint32_t denominator2 = 1;
	uint32_t denominator3 = 1;

	uint32_t denominator1Select = 1;
	uint32_t denominator2Select = 1;
	uint32_t denominator3Select = 1;

	uint32_t numerator1Alt = 1;
	uint32_t numerator2Alt = 1;
	uint32_t numerator3Alt = 1;

	uint32_t key1 = 0;
	uint32_t lastKey1 = 0;
	uint32_t key2 = 0;
	uint32_t lastKey2 = 0;
	uint32_t key3 = 0;
	uint32_t lastKey3 = 0;

	int32_t index1Stable = 0;
	int32_t lastStableIndex1 = 0;
	int32_t index1Transition = 0;

	int32_t index1Hysterisis(int32_t index, int32_t control) {

		if (index1Stable) {
			index1Stable = (index - lastStableIndex1) == 0;
			index1Transition = (index > lastStableIndex1) ? control & 0b1111111110000000 : lastStableIndex1 << 7;
			lastStableIndex1 = index;
			return index;
		} else {
			index1Stable = abs(control - index1Transition) > 32;
			lastStableIndex1 = index1Stable ? index : lastStableIndex1;
			return lastStableIndex1;
		}
	}

	int32_t index2Stable = 0;
	int32_t lastStableIndex2 = 0;
	int32_t index2Transition = 0;

	int32_t index2Hysterisis(int32_t index, int32_t control) {

		if (index2Stable) {
			index2Stable = (index - lastStableIndex2) == 0;
			index2Transition = (index > lastStableIndex2) ? control & 0b1111111110000000 : lastStableIndex2 << 7;
			lastStableIndex2 = index;
			return index;
		} else {
			index2Stable = abs(control - index2Transition) > 32;
			lastStableIndex2 = index2Stable ? index : lastStableIndex2;
			return lastStableIndex2;
		}
	}

	int32_t index2CVStable = 0;
	int32_t lastStableIndex2CV = 0;
	int32_t index2CVTransition = 0;

	int32_t index2CVHysterisis(int32_t index, int32_t control) {

		if (index2CVStable) {
			index2CVStable = (index - lastStableIndex2CV) == 0;
			index2CVTransition = (index > lastStableIndex2CV) ? control & 0b1111111110000000 : lastStableIndex2CV << 7;
			lastStableIndex2CV = index;
			return index;
		} else {
			index2CVStable = abs(control - index2CVTransition) > 32;
			lastStableIndex2CV = index2CVStable ? index : lastStableIndex2CV;
			return lastStableIndex2CV;
		}
	}

	int32_t index3Stable = 0;
	int32_t lastStableIndex3 = 0;
	int32_t index3Transition = 0;

	int32_t index3Hysterisis(int32_t index, int32_t control) {

		if (index3Stable) {
			index3Stable = (index - lastStableIndex3) == 0;
			index3Transition = (index > lastStableIndex3) ? control & 0b1111111110000000 : lastStableIndex3 << 7;
			lastStableIndex3 = index;
			return index;
		} else {
			index3Stable = abs(control - index3Transition) > 32;
			lastStableIndex3 = index3Stable ? index : lastStableIndex3;
			return lastStableIndex3;
		}
	}

	int32_t index3CVStable = 0;
	int32_t lastStableIndex3CV = 0;
	int32_t index3CVTransition = 0;

	int32_t index3CVHysterisis(int32_t index, int32_t control) {

		if (index3CVStable) {
			index3CVStable = (index - lastStableIndex3CV) == 0;
			index3CVTransition = (index > lastStableIndex3CV) ? control & 0b1111111110000000 : lastStableIndex3CV << 7;
			lastStableIndex3CV = index;
			return index;
		} else {
			index3CVStable = abs(control - index3CVTransition) > 32;
			lastStableIndex3CV = index3CVStable ? index : lastStableIndex3CV;
			return lastStableIndex3CV;
		}
	}

	inline uint32_t fix32Mul(int32_t in, uint32_t frac) {
		return ((int64_t) frac * (int64_t) in) >> 32;
	}

	inline void phaseMod() {

		int32_t phaseMod = -inputs.cv3Samples[0];
		int32_t phaseModIncrement = (phaseMod - lastPhaseMod) << 11;
		lastPhaseMod = phaseMod;
		phaseModIncrement *= phaseModOn;
		phaseModIncrement2 = phaseModIncrement;
		phaseModTracker2 += (phaseModIncrement2 << 5);
	}

	void (ViaSync3::*updateOutputs)(int32_t writePosition);

	void updateOutputsSawSawSaw(int32_t writePosition);
	void updateOutputsSawSawSq(int32_t writePosition);
	void updateOutputsSawSawTri(int32_t writePosition);

	void updateOutputsSawSqSaw(int32_t writePosition);
	void updateOutputsSawSqSq(int32_t writePosition);
	void updateOutputsSawSqTri(int32_t writePosition);

	void updateOutputsSawTriSaw(int32_t writePosition);
	void updateOutputsSawTriSq(int32_t writePosition);
	void updateOutputsSawTriTri(int32_t writePosition);

	void updateOutputsSqSawSaw(int32_t writePosition);
	void updateOutputsSqSawSq(int32_t writePosition);
	void updateOutputsSqSawTri(int32_t writePosition);

	void updateOutputsSqSqSaw(int32_t writePosition);
	void updateOutputsSqSqSq(int32_t writePosition);
	void updateOutputsSqSqTri(int32_t writePosition);

	void updateOutputsSqTriSaw(int32_t writePosition);
	void updateOutputsSqTriSq(int32_t writePosition);
	void updateOutputsSqTriTri(int32_t writePosition);

	void updateOutputsTriSawSaw(int32_t writePosition);
	void updateOutputsTriSawSq(int32_t writePosition);
	void updateOutputsTriSawTri(int32_t writePosition);

	void updateOutputsTriSqSaw(int32_t writePosition);
	void updateOutputsTriSqSq(int32_t writePosition);
	void updateOutputsTriSqTri(int32_t writePosition);

	void updateOutputsTriTriSaw(int32_t writePosition);
	void updateOutputsTriTriSq(int32_t writePosition);
	void updateOutputsTriTriTri(int32_t writePosition);

	renderPointer oscCombos[3][3][3] = {
			{
					{
							&ViaSync3::updateOutputsSawSawSaw,
							&ViaSync3::updateOutputsSawSawSq,
							&ViaSync3::updateOutputsSawSawTri,
					},
					{
							&ViaSync3::updateOutputsSawSqSaw,
							&ViaSync3::updateOutputsSawSqSq,
							&ViaSync3::updateOutputsSawSqTri,
					},
					{
							&ViaSync3::updateOutputsSawTriSaw,
							&ViaSync3::updateOutputsSawTriSq,
							&ViaSync3::updateOutputsSawTriTri,
					}
			},
			{
					{
							&ViaSync3::updateOutputsSqSawSaw,
							&ViaSync3::updateOutputsSqSawSq,
							&ViaSync3::updateOutputsSqSawTri,
					},
					{
							&ViaSync3::updateOutputsSqSqSaw,
							&ViaSync3::updateOutputsSqSqSq,
							&ViaSync3::updateOutputsSqSqTri,
					},
					{
							&ViaSync3::updateOutputsSqTriSaw,
							&ViaSync3::updateOutputsSqTriSq,
							&ViaSync3::updateOutputsSqTriTri,
					}
			},
			{
					{
							&ViaSync3::updateOutputsTriSawSaw,
							&ViaSync3::updateOutputsTriSawSq,
							&ViaSync3::updateOutputsTriSawTri,
					},
					{
							&ViaSync3::updateOutputsTriSqSaw,
							&ViaSync3::updateOutputsTriSqSq,
							&ViaSync3::updateOutputsTriSqTri,
					},
					{
							&ViaSync3::updateOutputsTriTriSaw,
							&ViaSync3::updateOutputsTriTriSq,
							&ViaSync3::updateOutputsTriTriTri,
					}
			},
	};

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {

		int32_t reading = TIM2->CNT;

		if (reading < (1440 * 32)) {
			errorPileup ++;
			advanceSubharm();
		} else {
			periodCount = reading;
			TIM2->CNT = 0;
			int32_t playbackPosition = (64 - DMA1_Channel5->CNDTR) & 31;

			advanceSubharm();

			error = phaseLockOn ? phases[playbackPosition] : 0;

			int32_t phaseSpan = (errorPileup + 1);

			increment1 = (45 * (((int64_t) phaseSpan << 32) - error))/(periodCount);
			errorPileup = 0;

			increment2 = fix32Mul(increment1, sync1Div) * numerator1Alt;
			increment3 = fix32Mul(increment1, sync2Div) * numerator2Alt;
			increment4 = fix32Mul(increment1, sync3Div) * numerator3Alt;

//			phase1 *= hardSync;

			uint32_t ratioDelta = (key1 != lastKey1) | (key2 != lastKey2) | (key3 != lastKey3);

			setLogicA(ratioDelta);

			numerator1 = numerator1Select;
			numerator2 = numerator2Select;
			numerator3 = numerator3Select;

			denominator1 = denominator1Select;
			denominator2 = denominator2Select;
			denominator3 = denominator3Select;

			if (runtimeDisplay) {
				setLEDC(ratioDelta);
				setLEDB(1);
			}

			lastKey1 = key1;
			lastKey2 = key2;
			lastKey3 = key3;

		}

	}
	void mainFallingEdgeCallback(void) {

		setLogicA(0);
		if (runtimeDisplay) {
			setLEDC(0);
			setLEDB(0);
		}

	}

	void auxRisingEdgeCallback(void) {

		setSH(1, 1);
		if (runtimeDisplay) {
			setLEDA(1);
		}

	}
	void auxFallingEdgeCallback(void) {

		setSH(0, 0);
		if (runtimeDisplay) {
			setLEDA(0);
		}

	}
	void buttonPressedCallback(void) {

	}
	void buttonReleasedCallback(void) {

	}
	void ioProcessCallback(void) {}
	void halfTransferCallback(void) {

		(this->*updateOutputs)(0);

		phaseMod();

	}
	void transferCompleteCallback(void) {

		(this->*updateOutputs)(VIA_SYNC3_BUFFER_SIZE);

		phaseMod();

	}
	void slowConversionCallback(void) {

		controls.updateSlowExtra();

		int32_t ratio2Mod = -inputs.cv2Samples[0];
		int32_t ratio3Mod = phaseModOn ? ratio2Mod : -inputs.cv3Samples[0];

		ratio2Mod >>= 4;
		ratio3Mod >>= 4;

		ratio2Mod += 2048;
		ratio3Mod += 2048;

		// need to synchronize change

		uint32_t thisIndex1 = controls.knob1Value + controls.cv1Value;
		uint32_t thisIndex2 = controls.knob2Value;
		uint32_t thisIndex3 = controls.knob3Value;
		thisIndex1 = index1Hysterisis(thisIndex1 >> 8, thisIndex1);
		thisIndex2 = index2Hysterisis(thisIndex2 >> 8, thisIndex2);
		thisIndex3 = index3Hysterisis(thisIndex3 >> 8, thisIndex3);
		thisIndex2 += index2CVHysterisis(ratio2Mod >> 8, ratio2Mod);
		thisIndex3 += index3CVHysterisis(ratio3Mod >> 8, ratio3Mod);
		sync1Div = dividedPhases[thisIndex1];
		sync2Div = dividedPhases[thisIndex2];
		sync3Div = dividedPhases[thisIndex3];

		if (phaseLockOn) {
			numerator1Select = numerators[thisIndex1];
			numerator2Select = numerators[thisIndex2];
			numerator3Select = numerators[thisIndex3];

			denominator1Select = denominators[thisIndex1];
			denominator2Select = denominators[thisIndex2];
			denominator3Select = denominators[thisIndex3];

			phase2 += __SSAT((phase1 - (phase2 * denominator1)), 20);
			phase3 += __SSAT(((phase1 + fix32Mul((1<<30), dividedPhases[thisIndex2])) - ((phase3 - phaseModTracker2) * denominator2)), 20);
			phase4 += __SSAT((phase1 - ((phase4 - phaseModTracker2) * denominator3)), 20);
		} else {
			numerator1Alt = numerators[thisIndex1];
			numerator2Alt = numerators[thisIndex2];
			numerator3Alt = numerators[thisIndex3];



		}

		key1 = keys[thisIndex1];
		key2 = keys[thisIndex2];
		key3 = keys[thisIndex3];

	}
	void auxTimer1InterruptCallback(void) {

	}
	void auxTimer2InterruptCallback(void) {

	}

	int32_t numButton1Modes = 3;
	int32_t numButton2Modes = 8;
	int32_t numButton3Modes = 3;
	int32_t numButton4Modes = 2;
	int32_t numButton5Modes = 0;
	int32_t numButton6Modes = 3;

	void handleButton1ModeChange(int32_t);
	void handleButton2ModeChange(int32_t);
	void handleButton3ModeChange(int32_t);
	void handleButton4ModeChange(int32_t);
	void handleButton5ModeChange(int32_t);
	void handleButton6ModeChange(int32_t);

	/// On construction, call subclass constructors and pass each a pointer to the module class.
	ViaSync3() : sync3UI(*this) {

		/// Link the module GPIO registers.
		initializeAuxOutputs();

		/// Initialize the input stream buffers.
		inputs.init(VIA_SYNC3_BUFFER_SIZE);
		/// Initialize the output stream buffers.
		outputs.init(VIA_SYNC3_BUFFER_SIZE);
		/// Set the data members that will be used to determine DMA stream initialization in the hardware executable.
		outputBufferSize = VIA_SYNC3_BUFFER_SIZE;
		inputBufferSize = 1;

		/// Call the UI initialization that needs to happen after outer class construction.
		sync3UI.initialize();

	}

};

#endif /* INC_SYNC3_HPP_ */
