/**
 * \file gateseq.hpp
 *
 *  \brief Define ViaGateseq class
 */

#ifndef INC_TRIGSEQ_HPP_
#define INC_TRIGSEQ_HPP_

#include <via_platform_binding.hpp>
#include <user_interface.hpp>

#ifdef BUILD_VIRTUAL
#include <stdlib.h>
#include <stdio.h>
#include <string>
#endif

enum {
	// Dual soft gate (use crossfader as dual and gate with followed by mixing "or" stage)
	SOFT_GATE_EXECUTE,
	SOFT_GATE_HIGH,
	SOFT_GATE_LOW
};

struct GateseqPatternBank {
    uint32_t patternOffsets[16];
    uint32_t lengths[16];
};
/*
 *
 * Dual euclidean sequencer
 *
 */

class DualEuclidean {

#ifdef BUILD_F373

	uint32_t aLength = 1;
	uint32_t bLength = 1;
	uint32_t aPatternMorph = 0;
	uint32_t bPatternMorph = 0;
	uint32_t aPatternIndex = 0;
	uint32_t bPatternIndex = 0;

#endif

public:

#ifdef BUILD_VIRTUAL

	uint32_t aLength = 1;
	uint32_t bLength = 1;
	uint32_t aPatternMorph = 0;
	uint32_t bPatternMorph = 0;
	uint32_t aPatternIndex = 0;
	uint32_t bPatternIndex = 0;

	int32_t virtualTimer1Count = 0;
	int32_t virtualTimer1Overflow = 0; //not used
	int32_t virtualTimer1Enable = 1; //not used

	int32_t virtualTimer2Count = 0;
	int32_t virtualTimer2Overflow = 48000; // default
	int32_t virtualTimer2Enable = 1; //not used

	int32_t virtualTimer3Count = 0;
	int32_t virtualTimer3Overflow = 0;
	int32_t virtualTimer3Enable = 0;

	int32_t virtualTimer4Count = 0;
	int32_t virtualTimer4Overflow = 48; // 1ms at 48k
	int32_t virtualTimer4Enable = 0;


#endif

	uint32_t offset = 0;

	uint32_t periodCount = 48000;
	uint32_t clockOn = 0;
	uint32_t multiplier = 1;
	uint32_t multReset = 0;
	uint32_t divider = 1;
	uint32_t perStepReset = 0;
	uint32_t clockPeriod = 0;
	uint32_t skipA;
	uint32_t shuffleOn = 0;
	uint32_t shuffle = 32767;
	uint32_t shuffleDelay = 0;
	uint32_t shuffledStep = 0;
	uint32_t skipClock = 0;
	uint32_t modulateMultiplier = 0;
	uint32_t multiplierCount = 0;
	uint32_t virtualGateHigh = 0;
	uint32_t mainGateHigh = 0;
	uint32_t processSeq1 = 0;

	int32_t cv1Offset = 0;
	int32_t cv2Offset = 0;
	int32_t cv3Offset = 0;

	int32_t multipliers[8] = {1, 2, 3, 4, 6, 8, 12, 16};
	int32_t multiplierResets[8] = {0, 0, 6, 4, 6, 8, 12, 16};
	int32_t perStepResets[8] = {0, 0, 0, 1, 1, 1, 1, 1};

	// "inputs"
	uint32_t aCounter = 0;
	uint32_t bCounter = 0;
	uint32_t sampleA = 0;
	uint32_t sampleB = 0;
	uint32_t trackA = 0;
	uint32_t trackB = 0;
	uint32_t andA = 0;
	uint32_t andB = 0;
	uint32_t gateAEvent = 0;
	uint32_t gateBEvent = 0;
	uint32_t auxLogicMode = 0;

#ifdef BUILD_F373
	const uint32_t * currentAPattern;
	const uint32_t * currentBPattern;
    const GateseqPatternBank * currentABank;
    const GateseqPatternBank * currentBBank;
    const uint32_t * bankStart; 
#endif
#ifdef BUILD_VIRTUAL
	uint32_t * currentAPattern;
	uint32_t * currentBPattern;
    GateseqPatternBank * currentABank;
    GateseqPatternBank * currentBBank;
    uint32_t * bankBaseAddress; 
#endif

	// "outputs"
	uint32_t aOutput = 0;
	uint32_t bOutput = 0;
	uint32_t logicOutput = 0;

	uint32_t shASignal = 0;
	uint32_t shBSignal = 0;

	void processMainRisingEdge(void);
	void processInternalRisingEdge(void);
	void processMainFallingEdge(void);
	void processInternalFallingEdge(void);
	void advanceSequencerA(void);
	void advanceSequencerB(void);
	void updateLogicOutput(void);
	void parseControls(ViaControls *, ViaInputStreams *);

};


/*
 *
 * Shared resources
 *
 */

class SoftGate {

	void gateAHigh(void);
	void gateALow(void);

	void gateARise(void);
	void gateAFall(void);

	void gateBHigh(void);
	void gateBLow(void);

	void gateBRise(void);
	void gateBFall(void);

	void (SoftGate::*manageGateA)(void);
	void (SoftGate::*manageGateB)(void);

	int32_t gateA = 0;
	int32_t gateB = 0;

public:

	int32_t attackTimeA = 500;
	int32_t releaseTimeA = 100;
	int32_t attackTimeB = 500;
	int32_t releaseTimeB = 100;

	int32_t aSignal = 0;
	int32_t bSignal = 0;

	int32_t updateGateA(int32_t signal);

	int32_t updateGateB(int32_t signal);

	SoftGate(void) {
		manageGateA = &SoftGate::gateALow;
		manageGateB = &SoftGate::gateBLow;
	}

};


/*
 *
 * Modes
 *
 */

//@{
/// Mode enums and mode variables
#define SH_A_MODE button1Mode
#define AND_A_MODE button2Mode
#define SEQ1_MODE button3Mode
#define SH_B_MODE button4Mode
#define AND_B_MODE button5Mode
#define SEQ2_MODE button6Mode
#define LOGIC_MODE aux2Mode
//@}

//@{
/// Number of modes per parameter
#define numButton1Modes 3
#define numButton2Modes 3
#define numButton3Modes 4
#define numButton4Modes 3
#define numButton5Modes 3
#define numButton6Modes 4
#define numAux1Modes 0
#define numAux2Modes 4
#define numAux3Modes 0
#define numAux4Modes 0
//@}

//@{
/// Mode enumeration
enum gateseq_button1Modes {
	aNoSH, aResample, aSampleTrack
};
enum gateseq_button2Modes {
	andAoff, andAon, softGateA
};
enum gateseq_button3Modes {
	aPatternBankSelections
};
enum gateseq_button4Modes {
	bNoSH, bResample, bSampleTrack
};
enum gateseq_button5Modes {
	andBoff, andBon, softGateB
};
enum gateseq_button6Modes {
	bPatternBankSelections
};
enum gateseq_aux1Modes {
	gateseq_aux1NotUsed
};
enum gateseq_aux2Modes {
	_and, _or, _xor, _sr
};
enum gateseq_aux3Modes {
	gateseq_aux3NotUsed
};
enum gateseq_aux4Modes {
	gateseq_aux4NotUsed
};
//@}

/// Buffer size of 1 aka no oversampling
#define GATESEQ_BUFFER_SIZE 1

/// callback to link gateseq UI to the STM32 touch sense library (which is C code, this could use an explanation)
void gateseqTouchLink (void *);

/// Gateseq module
class ViaGateseq : public TARGET_VIA {

public:

	/// Implementation of the UI library for the Gateseq module
	class ViaGateseqUI: public ViaUI {

	public:

		/// Pointer to the containing class
		/// Allows class methods to access methods and variables of the containing class
		ViaGateseq& this_module;

		/// Increment, store, and change SH_A_MODE.
		void button1TapCallback(void) override;
		/// Return to default without changing mode.
		void button1HoldCallback(void) override;
		/// Increment, store, and change AND_A_MODE.
		void button2TapCallback(void) override;
		/// Return to default without changing mode.
		void button2HoldCallback(void) override;
		/// Increment, store, and change SEQ1_MODE.
		void button3TapCallback(void) override;
		/// Return to default without changing mode.
		void button3HoldCallback(void) override;
		/// Increment, store, and change SH_B_MODE.
		void button4TapCallback(void) override;
		/// Return to default without changing mode.
		void button4HoldCallback(void) override;
		/// Increment, store, and change AND_B_MODE.
		void button5TapCallback(void) override;
		/// Return to default without changing mode.
		void button5HoldCallback(void) override;
		/// Increment, store, and change SEQ2_MODE.
		void button6TapCallback(void) override;
		/// Return to default without changing mode.
		void button6HoldCallback(void) override;
		/// Increment, store, and change LOGIC_MODE.
		/// Alt is button 5 + button 2 instead of button 3
		void aux2AltTapCallback(void) override;
		/// Return to default without changing mode.
		void aux2AltHoldCallback(void) override;

		//@{
		/// Unused
		void aux1TapCallback(void) override;
		void aux1HoldCallback(void) override;
		void aux2TapCallback(void) override;
		void aux2HoldCallback(void) override;
		void aux3TapCallback(void) override;
		void aux3HoldCallback(void) override;
		void aux4TapCallback(void) override;
		void aux4HoldCallback(void) override;
		//@}

		/// Provide a means to set the LEDs from within the UI library
		void uiSetLEDs(int) override;

		/// Call mode change handlers to restore the module state per modes decoded from modeStateBuffer
		/// Used at initialization or preset recall
		void recallModuleState(void) override;

		/// Clear LEDs and turn runtimeDisplay on
		void defaultEnterMenuCallback(void) override;
		void newModeEnterMenuCallback(void) override;
		void newAuxModeEnterMenuCallback(void) override;
		void presetEnterMenuCallback(void) override;

		/// Display SH_A_MODE and turn runtimeDisplay off
		void button1EnterMenuCallback(void) override;
		/// Display AND_A_MODE and turn runtimeDisplay off
		void button2EnterMenuCallback(void) override;
		/// Display SEQ1_MODE and turn runtimeDisplay off
		void button3EnterMenuCallback(void) override;
		/// Display SH_B_MODE and turn runtimeDisplay off
		void button4EnterMenuCallback(void) override;
		/// Display AND_B_MODE and turn runtimeDisplay off
		void button5EnterMenuCallback(void) override;
		/// Display SEQ2_MODE and turn runtimeDisplay off
		void button6EnterMenuCallback(void) override;
		/// Display LOGIC_MODE and turn runtimeDisplay off
		void aux2AltEnterMenuCallback(void) override;

		//@{
		/// Unused, return to button 5 menu
		void aux1EnterMenuCallback(void) override;
		void aux2EnterMenuCallback(void) override;
		void aux3EnterMenuCallback(void) override;
		void aux4EnterMenuCallback(void) override;
		//@}

		void writeStockPresets(void) override;

		void blinkOnCallback(void) override {
			restoreRed = *(this_module.redLevel);
			restoreGreen = *(this_module.greenLevel);
			restoreBlue = *(this_module.blueLevel);
			this_module.updateRGBDisplay(4095, 4095, 4095, 1);
		#ifdef BUILD_F373
			this_module.setBlueLEDAlt(1);
		#endif
		}

		void blinkOffCallback(void) override {
			this_module.updateRGBDisplay(restoreRed, restoreGreen,
					restoreBlue, 1);
		#ifdef BUILD_F373
			this_module.setBlueLEDAlt(0);
		#endif

		}

		// TODO use enums
		// (shA, gateA, pattern 1, shB, gateB, pattern 2, logic, na, na, na)
		// dual euclidean
		uint32_t stockPreset1 = ENCODE_PRESET(0, 2, 0, 0, 2, 0, 0, 0, 0, 0);
		// 2 vs 3
		uint32_t stockPreset2 = ENCODE_PRESET(0, 2, 1, 0, 2, 1, 3, 0, 0, 0);
		// shuffle/swing
		uint32_t stockPreset3 = ENCODE_PRESET(0, 2, 2, 0, 2, 2, 1, 0, 0, 0);
		// clock multiplier
		uint32_t stockPreset4 = ENCODE_PRESET(0, 2, 3, 0, 2, 3, 1, 0, 0, 0);
		// s&track
		uint32_t stockPreset5 = ENCODE_PRESET(1, 0, 0, 0, 1, 0, 3, 0, 0, 0);
		// resample
		uint32_t stockPreset6 = ENCODE_PRESET(2, 0, 0, 2, 0, 0, 2, 0, 0, 0);

		/// Initialization that for some reason happens outside of the constructor?
		void initialize(void) override;

		/// Link the module's UI to the touch sense library.
		ViaGateseqUI(ViaGateseq& x): this_module(x) {
			linkUI((void *) &gateseqTouchLink, (void *) this);
		}

	};

	/// Update the mode of the A channel sample and hold.
	void handleButton1ModeChange(int32_t);
	/// Update the mode of the A channel and gate.
	void handleButton2ModeChange(int32_t);
	/// Update the sequencer I mode.
	void handleButton3ModeChange(int32_t);
	/// Update the mode of the B channel sample and hold.
	void handleButton4ModeChange(int32_t);
	/// Update the mode of the B channel and gate.
	void handleButton5ModeChange(int32_t);
	/// Update the sequencer II mode.
	void handleButton6ModeChange(int32_t);
	/// Not used
	void handleAux1ModeChange(int32_t);
	/// Update the logic output mode.
	void handleAux2ModeChange(int32_t);
	/// Not used
	void handleAux3ModeChange(int32_t);
	/// Not used
	void handleAux4ModeChange(int32_t);

	/*
	 *
	 * Tables
	 *
	 */


#ifdef BUILD_F373
	const struct GateseqPatternBank * banks = (const struct GateseqPatternBank *) 0x8020000;
#endif
#ifdef BUILD_VIRTUAL
    struct GateseqPatternBank * banks = NULL;
    void readPatternsFromFile(std::string path) {
        FILE * patternsFile = fopen(path.c_str(), "r");
        if (patternsFile == NULL) {
            return; // TODO: Error handling for file not exist or something
        }
        
        fseek(patternsFile, 0, SEEK_END);
        uint32_t lSize = ftell(patternsFile);
        rewind(patternsFile);
        printf("Reading file of size: %d\n", lSize);

        free(banks);
        banks = (GateseqPatternBank *) malloc(lSize);
        sequencer.bankBaseAddress = (uint32_t *) banks;

        fread(banks, 1, lSize, patternsFile);
        fclose(patternsFile);
    };
#endif

	/*
	 *
	 * Mode functions
	 *
	 */

	/// Called in the constructor and I still dont know why its not the constructor
	void init(void);

	/// Declare an instance of the module-specific UI implementation
	ViaGateseqUI gateseqUI;

	/// Flag that the UI uses to disable runtime LED feedback
	int32_t runtimeDisplay;

	/// Synthesis object to handle the sequence generation
	DualEuclidean sequencer;

	/// Gate controller to set the dac levels and implement the and gates
	SoftGate gateController;

	/// Variable to indicate that a rising edge just occurred at the main logic output
	uint32_t simultaneousTrigFlag;

	//@{
	/// Mode variable to enable or disable the and gate
	int32_t softGateAOn;
	int32_t softGateBOn;
	//@}

	//@{
	/// Variables to pass gate event messages asynchronously from the sequencer to the gate controller
	int32_t readGateAEvent;
	int32_t readGateBEvent;
	//@}

	uint32_t pwmCounter;

	uint32_t blueLED;

#ifdef BUILD_F373

	void initializeAuxOutputsGateseq(void) {

		/// Tie the output pointers of the module to GPIO control registers.
		aLogicOutput = &(GPIOC->BSRR);
		auxLogicOutput = &(GPIOA->BSRR);
		shAOutput = &(GPIOB->BSRR);
		shBOutput = &(GPIOB->BSRR);

		ledAOutput = &(GPIOF->BSRR);
		ledBOutput = &(GPIOC->BSRR);
		ledCOutput = &(GPIOA->BSRR);
		ledDOutput = &(GPIOB->BSRR);

		/// Tie the PWM control output pointers of the module class to GPIO control registers.
		// tim3 channel 2
		redLevel = &TIM3->CCR1 + 1;
		// tim4 channel 4
		greenLevel = &TIM4->CCR1 + 3;
		// tim5 channel 1
		blueLevel = (volatile uint32_t *) &blueLED;

	}

#endif

#ifdef BUILD_VIRTUAL

	void initializeAuxOutputsGateseq(void) {

		initializeAuxOutputs();

	}

#endif

	/*
	 *
	 * Event handlers
	 *
	 */

	/// Call the init function which is basically the constructor and should be inlined here
#ifdef BUILD_F373
	ViaGateseq() : gateseqUI(*this) {
#endif
#ifdef BUILD_VIRTUAL
	ViaGateseq(std::string binPath) : gateseqUI(*this) {
        readPatternsFromFile(binPath);
#endif
		init();
	}

	void readCalibrationPacket(void) {
		calibrationPacket = gateseqUI.loadFromMemory(7);
		decodeCalibrationPacket();
	}

	/// Handle a rising edge at the main logic input
	void mainRisingEdgeCallback(void) override;
	/// Handle a falling edge at the main logic input
	void mainFallingEdgeCallback(void) override;

	/// Handle a rising edge at the expander logic input
	void auxRisingEdgeCallback(void) override;
	/// Handle a falling edge at the expander logic input
	void auxFallingEdgeCallback(void) override;

	/// Handle a press event on the expander control button
	void buttonPressedCallback(void) override;
	/// Handle a release event on the expander control button
	void buttonReleasedCallback(void) override;

	/// unused
	void ioProcessCallback(void) override;

	/// Fill the first half of the dac buffers
	void halfTransferCallback(void) override;
	/// Fill the second half of the dac buffers
	void transferCompleteCallback(void) override;
	/// Handle an end of conversion event for the 12 bit control rate ADCs
	void slowConversionCallback(void) override;

	/// Handle an overflow interrupt from aux timer 1
	void auxTimer1InterruptCallback(void) override;
	/// Handle an overflow interrupt from aux timer 2
	void auxTimer2InterruptCallback(void) override;
	/// Handle an overflow interrupt from aux timer 3
	void auxTimer3InterruptCallback(void) override;

	/// Dispatch a signal to the touch sense interface
	void ui_dispatch(int32_t sig) {
		gateseqUI.dispatch(sig);
	};

};

#endif /* INC_TRIGSEQ_HPP_ */
