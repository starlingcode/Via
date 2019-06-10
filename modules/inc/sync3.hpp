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

	class ViaSync3UI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		ViaSync3& this_module;


//		//@{
//		/// Functions to handle a tap event on a sync3 sensor or shift combo.
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
//		/// Functions to handle a hold event on a sync3 sensor or shift combo.
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
//		/// Methods to handle entry transitions into sync3 sensor menu states.
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

	// Good stuff

	PllController pll;

	uint32_t phase1 = 0;
	uint32_t phase2 = 0;
	uint32_t phase3 = 0;
	uint32_t phases[VIA_SYNC3_BUFFER_SIZE];
	int32_t increment1 = 1000;
	int32_t increment2 = 1000;
	int32_t increment3 = 1000;

	uint32_t periodCount = 0;

	int32_t errorPileup = 0;

	lilBuffer errorBuffer;
	int32_t errorSum = 0;

	uint32_t numerator = 1;
	uint32_t denominator = 7;

	uint32_t divCounter = 0;

	int32_t dividedPhase = 0;

	uint32_t numerators[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 8};
	uint32_t denominators[16] = {8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	uint32_t dividedPhases[16] = {536870912, 613566756, 715827882, 858993459, 1073741824, 1431655765, 2147483648, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	void updateOutputs(int32_t writePosition);

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {

		int32_t reading = TIM2->CNT;

		if (reading < (1440 * 25)) {
			errorPileup ++;
			divCounter = divCounter < (denominator - 1) ? divCounter + 1 : 0;
		} else {
			periodCount = reading;
			TIM2->CNT = 0;

			int32_t playbackPosition = (64 - DMA1_Channel5->CNDTR) & 31;

			divCounter = divCounter < (denominator - 1) ? divCounter + 1 : 0;

			uint32_t targetPhase = dividedPhase * divCounter;
			int32_t error = phases[playbackPosition] - targetPhase;

			int32_t phaseSpan = numerator * (errorPileup + 1);

			increment1 = __USAT((45 * ((int64_t) phaseSpan << 32) - error)/(periodCount * denominator), 30);
			errorPileup = 0;

			increment2 = increment1 << 1;
			increment3 = increment1 >> 1;
		}

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
		updateOutputs(0);
	}
	void transferCompleteCallback(void) {
		updateOutputs(VIA_SYNC3_BUFFER_SIZE);
	}
	void slowConversionCallback(void) {

		controls.updateSlowExtra();
		uint32_t index = controls.knob1Value >> 8;
		numerator = numerators[index];
		denominator = denominators[index];
		dividedPhase = dividedPhases[index];

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
