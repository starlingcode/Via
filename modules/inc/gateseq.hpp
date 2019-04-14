/**
 * \file gateseq.hpp
 *
 *  \brief Define ViaGateseq class
 */

#ifndef INC_TRIGSEQ_HPP_
#define INC_TRIGSEQ_HPP_

#ifdef __cplusplus
extern "C" {
#endif

#include "sequencers.hpp"
#include <via_platform_binding.hpp>
#include <user_interface.hpp>

#ifdef __cplusplus
}
#endif

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
class ViaGateseq : public ViaModule {

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
		uint32_t stockPreset5 = ENCODE_PRESET(2, 0, 0, 0, 1, 0, 3, 0, 0, 0);
		// resample
		uint32_t stockPreset6 = ENCODE_PRESET(1, 0, 0, 1, 0, 0, 2, 0, 0, 0);

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

	/// Pointer to an array of type booleanSequenceBank for use with sequencer I.
	const booleanSequenceBank *seq1Banks[4];
	/// Pointer to an array of type booleanSequenceBank for use with sequencer II.
	const booleanSequenceBank *seq2Banks[4];

	/// Load seq1Banks and seq2Banks a set of sequence banks for each sequencer.
	/// Each of the sequencer's 4 modes gets a bank.
	void initializePatterns(void);

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

	/*
	 *
	 * Event meta_handlers
	 *
	 */

	/// Call the init function which is basically the constructor and should be inlined here
	ViaGateseq() : gateseqUI(*this) {
		init();
	}

	void readCalibrationPacket(void) {
		calibrationPacket = gateseqUI.loadFromMemory(7);
		decodeCalibrationPacket();
	}

	/// Handle a rising edge at the main logic input
	void mainRisingEdgeCallback(void);
	/// Handle a falling edge at the main logic input
	void mainFallingEdgeCallback(void);

	/// Handle a rising edge at the expander logic input
	void auxRisingEdgeCallback(void);
	/// Handle a falling edge at the expander logic input
	void auxFallingEdgeCallback(void);

	/// Handle a press event on the expander control button
	void buttonPressedCallback(void);
	/// Handle a release event on the expander control button
	void buttonReleasedCallback(void);

	/// unused
	void ioProcessCallback(void);

	/// Fill the first half of the dac buffers
	void halfTransferCallback(void);
	/// Fill the second half of the dac buffers
	void transferCompleteCallback(void);
	/// Handle an end of conversion event for the 12 bit control rate ADCs
	void slowConversionCallback(void);

	/// Handle an overflow interrupt from aux timer 1
	void auxTimer1InterruptCallback(void);
	/// Handle an overflow interrupt from aux timer 2
	void auxTimer2InterruptCallback(void);
	/// Handle an overflow interrupt from aux timer 3
	void auxTimer3InterruptCallback(void);

	/// Dispatch a signal to the touch sense interface
	void ui_dispatch(int32_t sig) {
		gateseqUI.dispatch(sig);
	};

};

#endif /* INC_TRIGSEQ_HPP_ */
