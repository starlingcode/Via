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


	PllController pll;

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
	int32_t phaseModTracker = 0;
	int32_t lastPhaseMod = 0;
	int32_t phaseModIncrement = 0;

	int32_t sync1Mult = 0;
	int32_t sync2Mult = 0;
	int32_t sync3Mult = 0;

	uint32_t periodCount = 0;

	int32_t errorPileup = 0;

	lilBuffer errorBuffer;
	int32_t errorSum = 0;

	uint32_t numerator = 1;
	uint32_t denominator = 1;

	uint32_t divCounter = 0;

	int32_t dividedPhase = 0;

	uint32_t numerator1 = 1;
	uint32_t numerator2 = 1;
	uint32_t numerator3 = 1;

	uint32_t numerators[16] = {1, 9, 3, 2, 3, 4, 9, 1, 2, 9, 12, 8, 3, 16, 18, 4};
	uint32_t dividedPhases[16] = {2147483647, 268435455, 858993459, 1431655765, 1073741823, 858993459, 429496729, 4294967295, 4294967295, 1073741823, 858993459, 1431655765, 4294967295, 858993459, 858993459, 4294967295};
	uint32_t denominators[16] = {2, 16, 5, 3, 4, 5, 10, 1, 1, 4, 5, 3, 1, 5, 5, 1};

#define UNITY 65536
	uint32_t multipliersInt[16] = {UNITY/8, UNITY/7, UNITY/6, UNITY/5, UNITY/4, UNITY/3, UNITY/2, UNITY/1,
								UNITY/1, UNITY*2, UNITY*3, UNITY*4, UNITY*5, UNITY*6, UNITY*7, UNITY*8};

	uint32_t multipliersMinor[16] = {UNITY/2, (UNITY*9)/16, (UNITY*3)/5, (UNITY*2)/3, (UNITY*3)/4, (UNITY*4)/5, (UNITY*9)/10, UNITY/1,
									UNITY/1, (UNITY*9)/8, (UNITY*6)/5, (UNITY*4)/3, (UNITY*3)/2, (UNITY*8)/5, (UNITY*9)/5, UNITY*2};

	uint32_t multipliersRhythms[16] = {UNITY/16, UNITY/12, UNITY/8, UNITY/6, UNITY/4, UNITY/3, UNITY/2, UNITY/1,
										UNITY/1, UNITY*2, UNITY*3, UNITY*4, UNITY*6, UNITY*8, UNITY*12, UNITY*16};

	uint32_t multipliersMinorPent[16] = {(UNITY*3)/8, (UNITY*9)/20, UNITY/2, (UNITY*3)/5, (UNITY*2)/3, (UNITY*3)/4, (UNITY*9)/10, UNITY/1,
									UNITY/1, (UNITY*6)/5, (UNITY*4)/3, (UNITY*3)/2, (UNITY*9)/5, UNITY*2, (UNITY*9)/4, (UNITY*12)/5};

	uint32_t * multipliers = multipliersInt;

	inline uint32_t fix32Mul(int32_t in, uint32_t frac) {
		return ((int64_t) frac * (int64_t) in) >> 32;
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
		divCounter = divCounter < (denominator - 1) ? divCounter + 1 : 0;

		if (reading < (1440 * 25)) {
			errorPileup ++;
		} else {
			periodCount = reading;
			TIM2->CNT = 0;

			int32_t playbackPosition = (64 - DMA1_Channel5->CNDTR) & 31;

			uint32_t targetPhase = dividedPhase * divCounter;
			int32_t error = phases[playbackPosition] - targetPhase;

			int32_t phaseSpan = numerator * (errorPileup + 1);

			increment1 = __USAT((45 * ((int64_t) phaseSpan << 32) - error)/(periodCount * denominator), 30);
			errorPileup = 0;

			increment2 = fix32Mul(increment1, sync1Mult);
			increment3 = fix32Mul(increment1, sync2Mult);
			increment4 = fix32Mul(increment1, sync3Mult);
		}

	}
	void mainFallingEdgeCallback(void) {

	}
	void auxRisingEdgeCallback(void) {

		setSH(1, 1);

	}
	void auxFallingEdgeCallback(void) {

		setSH(0, 0);

	}
	void buttonPressedCallback(void) {

	}
	void buttonReleasedCallback(void) {

	}
	void ioProcessCallback(void) {}
	void halfTransferCallback(void) {

		int32_t phaseMod = -inputs.cv3Samples[0];
		phaseModIncrement = (phaseMod - lastPhaseMod) << 13;
		phaseModIncrement *= phaseModOn;
		phaseModTracker += phaseModIncrement;
		lastPhaseMod = phaseMod;

		(this->*updateOutputs)(0);

	}
	void transferCompleteCallback(void) {

		int32_t phaseMod = -inputs.cv3Samples[0];
		phaseModIncrement = (phaseMod - lastPhaseMod) << 13;
		phaseModIncrement *= phaseModOn;
		phaseModTracker += phaseModIncrement;
		lastPhaseMod = phaseMod;

		(this->*updateOutputs)(VIA_SYNC3_BUFFER_SIZE);

	}
	void slowConversionCallback(void) {

		controls.updateSlowExtra();

		int32_t ratio2Mod = -inputs.cv2Samples[0];
		int32_t ratio3Mod = phaseModOn ? ratio2Mod : -inputs.cv3Samples[0];

		ratio2Mod >>= 4;
		ratio3Mod >>= 4;

		uint32_t index1 = __USAT(controls.knob1Value + controls.cv1Value - 2048, 12) >> 8;
		uint32_t index2 = __USAT(controls.knob2Value + ratio2Mod, 12) >> 8;
		uint32_t index3 = __USAT(controls.knob3Value + ratio3Mod, 12) >> 8;
		sync1Mult = dividedPhases[index1];
		sync2Mult = dividedPhases[index2];
		sync3Mult = dividedPhases[index3];
		numerator1 = numerators[index1];
		numerator2 = numerators[index2];
		numerator3 = numerators[index3];

		phase2 += fix32Mul((phase1 - (phase2 * denominators[index1])), dividedPhases[index1]);
		phase3 += fix32Mul(((phase1 + (1<<30)) - (phase3 * denominators[index2])), dividedPhases[index2]);
		phase4 += fix32Mul((phase1 - (phase4 * denominators[index3])), dividedPhases[index3]);

	}
	void auxTimer1InterruptCallback(void) {

	}
	void auxTimer2InterruptCallback(void) {

	}

	int32_t numButton1Modes = 3;
	int32_t numButton2Modes = 1;
	int32_t numButton3Modes = 3;
	int32_t numButton4Modes = 2;
	int32_t numButton5Modes = 4;
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

		for (int i = 0; i < 8; i++) {
			writeLilBuffer(&errorBuffer, 0);
		}

		/// Call the UI initialization that needs to happen after outer class construction.
		sync3UI.initialize();

	}

};

#endif /* INC_SYNC3_HPP_ */
