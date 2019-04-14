/*
 * meta.hpp
 *
 *  Created on: Sep 11, 2018
 *      Author: willmitchell
 */

#ifndef INC_META_HPP_
#define INC_META_HPP_

#include "user_interface.hpp"
#include <via_platform_binding.hpp>
#include <oscillators.hpp>
#include "meta_tables.hpp"

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#define META_BUFFER_SIZE 8

/*
 *
 * Modes
 *
 */

// Mode enums and mode variables

#define SH_MODE button1Mode
#define TABLE button2Mode
#define TRIG_MODE button4Mode
#define FREQ_MODE button3Mode
#define LOOP_MODE button6Mode
#define DRUM_AUX_MODE aux1Mode
#define LOGIC_A_MODE aux2Mode
#define DRUM_MODE aux3Mode
#define DAC_3_MODE aux4Mode


#define numButton1Modes 6
#define numButton2Modes 8
#define numButton3Modes 3
#define numButton4Modes 5
#define numButton5Modes 8
#define numButton6Modes 2
#define numAux1Modes 4
#define numAux2Modes 2
#define numAux3Modes 4
#define numAux4Modes 2

enum meta_button1Modes {nosampleandhold, a, b, ab, halfdecimate, meta_decimate};
enum meta_button2Modes {tables};
enum meta_button3Modes {audio, env, seq};
enum meta_button4Modes {noretrigger, meta_hardsync, nongatedretrigger, gated, meta_pendulum};
enum meta_button5Modes {pairedWithButton2};
enum meta_button6Modes {noloop, looping};
enum meta_aux1Modes {drumPhasor, drumContour, drumEnv, noise};
enum meta_aux2Modes {releaseGate, attackGate};
enum meta_aux3Modes {pitchMorphAmp, pitchAmp, morphAmp, amp};
enum meta_aux4Modes {phasor, contour};

void metaTouchLink (void *);

class ViaMeta : public ViaModule {

public:

	class ViaMetaUI: public ViaUI {

	public:

		ViaMeta& this_module;

		void button1TapCallback(void) override;
		void button1HoldCallback(void) override;
		void button2TapCallback(void) override;
		void button2HoldCallback(void) override;
		void button3TapCallback(void) override;
		void button3HoldCallback(void) override;
		void button4TapCallback(void) override;
		void button4HoldCallback(void) override;
		void button5TapCallback(void) override;
		void button5HoldCallback(void) override;
		void button6TapCallback(void) override;
		void button6HoldCallback(void) override;

		void aux1TapCallback(void) override;
		void aux1HoldCallback(void) override;
		void aux2TapCallback(void) override;
		void aux2HoldCallback(void) override;
		void aux2AltTapCallback(void) override;
		void aux2AltHoldCallback(void) override;
		void aux3TapCallback(void) override;
		void aux3HoldCallback(void) override;
		void aux4TapCallback(void) override;
		void aux4HoldCallback(void) override;

		void uiSetLEDs(int) override;

		void recallModuleState(void) override;

		void defaultEnterMenuCallback(void) override;
		void newModeEnterMenuCallback(void) override;
		void newAuxModeEnterMenuCallback(void) override;
		void presetEnterMenuCallback(void) override;

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

		void initialize(void) override;

		void writeStockPresets(void) override;

		// TODO use enums ....
		// drum
		uint32_t stockPreset1 = ENCODE_PRESET(0, 0, 0, 0, 0, 0, 2, 0, 0, 0);
		// oscillator
		uint32_t stockPreset2 = ENCODE_PRESET(2, 4, 0, 4, 4, 1, 0, 1, 0, 0);
		// envelope
		uint32_t stockPreset3 = ENCODE_PRESET(1, 5, 1, 3, 5, 0, 0, 0, 1, 0);
		// looping AR
		uint32_t stockPreset4 = ENCODE_PRESET(3, 3, 1, 1, 3, 1, 0, 1, 0, 0);
		// complex sequence
		uint32_t stockPreset5 = ENCODE_PRESET(4, 2, 2, 5, 2, 0, 0, 0, 1, 0);
		// complex LFO
		uint32_t stockPreset6 = ENCODE_PRESET(0, 0, 2, 1, 0, 1, 0, 1, 0, 0);

		ViaMetaUI(ViaMeta& x): this_module(x) {
			linkUI((void *) &metaTouchLink, (void *) this);
		}

	};

	void handleButton1ModeChange(int32_t);
	void handleButton2ModeChange(int32_t);
	void handleButton3ModeChange(int32_t);
	void handleButton4ModeChange(int32_t);
	void handleButton5ModeChange(int32_t);
	void handleButton6ModeChange(int32_t);
	void handleAux1ModeChange(int32_t);
	void handleAux2ModeChange(int32_t);
	void handleAux3ModeChange(int32_t);
	void handleAux4ModeChange(int32_t);

	void initializeDrum(void);
	void initializeOscillator(void);
	void initializeEnvelope(void);
	void initializeSimpleLFO(void);
	void initializeSequence(void);
	void initializeComplexLFO(void);

	/*
	 *
	 * Tables
	 *
	 */

	MetaWavetableSet wavetableSet;

	// declare an array of pointers to wavetables (stored in flash)
	const Wavetable * wavetableArray[3][8];

	// declare arrays to store the active tables
	uint32_t wavetableRead[9][517];
	uint32_t wavetableReadDrum[517];
	uint32_t wavetableReadDrum2[517];

	// declare functions to set the currently active tables
	void switchWavetable(const Wavetable *);
	// phase distortion table is fixed
	void initPhaseDistTable(void);
	void fillWavetableArray(void);
	void initDrum(void);
	int16_t drumWrite[4];
	int16_t drum2Write[4];
	int16_t drum3Write[4];
	int16_t drumFullScale[4];
	int32_t drumOff[4];

	void readCalibrationPacket(void) {
		calibrationPacket = metaUI.loadFromMemory(7);
		decodeCalibrationPacket();
	}

	/*
	 *
	 * Mode functions
	 *
	 */

	void (ViaMeta::*outputStage)(int32_t writeIndex);

	int32_t dac3OffsetCompensation;

	void oversample(int32_t writeIndex);
	void addThreeBits(int32_t writeIndex);
	void drumMode(int32_t writeIndex);

	uint16_t virtualFM[2];
	uint16_t virtualMorph[2];

	void (ViaMeta::*calculateDac3)(int32_t writeIndex);

	void calculateDac3Phasor(int32_t writeIndex);
	void calculateDac3Contour(int32_t writeIndex);
	void calculateDac3DrumEnv(int32_t writeIndex);
	void calculateDac3Noise(int32_t writeIndex);
	void calculateDac3PhasorEnv(int32_t writeIndex);
	void calculateDac3ContourEnv(int32_t writeIndex);

	void (ViaMeta::*calculateLogicA)(int32_t writeIndex);

	void calculateLogicAReleaseGate(int32_t writeIndex);
	void calculateLogicAAttackGate(int32_t writeIndex);
	int32_t lastLogicAState = 1;
	int32_t logicATransitionSample = 0;
	int32_t logicAOutputStable = 0;

	int32_t logicAHysterisis(int32_t thisLogicAState, int32_t sample) {

		if (logicAOutputStable) {
			logicAOutputStable = ((lastLogicAState - thisLogicAState) == 0);
			logicATransitionSample = sample;
			lastLogicAState = thisLogicAState;
			return thisLogicAState;
		} else {
			logicAOutputStable = (abs(sample - logicATransitionSample) > 1);
			lastLogicAState = logicAOutputStable ? thisLogicAState : lastLogicAState;
			return lastLogicAState;
		}

	}

	void calculateDelta(int32_t writeIndex);
	int32_t lastDeltaState = 1;
	int32_t deltaTransitionSample = 0;
	int32_t deltaOutputStable = 0;

	int32_t deltaHysterisis(int32_t thisDeltaState, int32_t sample) {

		if (deltaOutputStable) {
			deltaOutputStable = ((lastDeltaState - thisDeltaState) == 0);
			deltaTransitionSample = sample;
			lastDeltaState = thisDeltaState;
			return thisDeltaState;
		} else {
			deltaOutputStable = (abs(sample - deltaTransitionSample) > 1);
			lastDeltaState = deltaOutputStable ? thisDeltaState : lastDeltaState;
			return lastDeltaState;
		}

	}

	int32_t deltaOut = 0;

	int32_t lastSample = 0;

	int32_t lfsrState = 1;
	void advanceLFSR(void) {
		lfsrState ^= lfsrState << 13;
		lfsrState ^= lfsrState << 17;
		lfsrState ^= lfsrState << 5;
		lfsrState &= 4095;
	}

	void (ViaMeta::*calculateSH)(int32_t writeIndex);
	// No S&H
	void calculateSHMode1(int32_t writeIndex);
	// Sample A from A to B
	void calculateSHMode2(int32_t writeIndex);
	// Resample B at A
	void calculateSHMode3(int32_t writeIndex);
	// Sample A from A to B and resample B at A
	void calculateSHMode4(int32_t writeIndex);
	// Half Decimate (Sample A from A to B sample B from B to A
	void calculateSHMode5(int32_t writeIndex);
	// Decimate (Resample A at B, resample B at A
	void calculateSHMode6(int32_t writeIndex);

	void (ViaMeta::*updateRGB)(void);
	void (ViaMeta::*currentRGBBehavior)(void);

	#ifdef BUILD_VIRTUAL 

		int32_t blinkTimerEnable = 0;
		int32_t blinkTimerCount = 0;
		int32_t blinkTimerOverflow = 100;

		int32_t blankTimerEnable = 0;
		int32_t blankTimerCount = 0;
		int32_t blankTimerOverflow = 100;

	#endif

	void updateRGBOsc(void) {

		int32_t displayFreq = abs(fix16_mul(__USAT(controls.knob1Value + controls.cv1Value - 1000, 12), metaController.fm[0] + 32767));

		int32_t redSignal = 4095 - displayFreq;
		int32_t blueSignal = displayFreq;
		int32_t greenSignal = __USAT(((-inputs.cv3Samples[0] >> 4) + controls.knob3Value), 12) >> 1;

		updateRGBDisplay(redSignal, greenSignal, blueSignal, runtimeDisplay);
	}
	void updateRGBDrum(void) {
		
		int32_t displayFreq = abs(fix16_mul(__USAT(controls.knob1Value + controls.cv1Value - 1000, 12), metaController.fm[0] + 32767));
		uint32_t drumEnvelopeLevel = ampEnvelope.output[0] << 1;

		int32_t redSignal = fix16_mul(4095 - displayFreq, drumEnvelopeLevel);
		int32_t blueSignal = fix16_mul(displayFreq, drumEnvelopeLevel);
		int32_t greenSignal = fix16_mul(__USAT((-inputs.cv3Samples[0] >> 4) + controls.knob3Value, 12), drumEnvelopeLevel) >> 1;

		updateRGBDisplay(redSignal, greenSignal, blueSignal, runtimeDisplay);

	}
	void updateRGBSubaudio(void) {

		int32_t sample = outputs.dac2Samples[0];
		int32_t lastPhaseValue = metaController.ghostPhase;

		int32_t redSignal = sample * (lastPhaseValue >> 24);
		int32_t blueSignal = (sample * (!(lastPhaseValue >> 24))) >> 1;
		int32_t greenSignal = (metaUI.button3Mode == 2) * sample;

		updateRGBDisplay(redSignal, greenSignal, blueSignal, runtimeDisplay);

	}

	void updateRGBBlink(void) {

		updateRGBDisplay(4095, 4095, 4095, runtimeDisplay);

	}

	void updateRGBBlank(void) {

		updateRGBDisplay(0, 0, 0, runtimeDisplay);

	}



	void init(void);

	ViaMetaUI metaUI;

	int32_t runtimeDisplay;

	MetaWavetable metaWavetable;
	MetaController metaController;
	SimpleEnvelope ampEnvelope;
	SimpleEnvelope freqTransient;
	SimpleEnvelope morphEnvelope;

	int32_t morphAttackMultiplier = 1 << 8;
	int32_t morphReleaseMultiplier = 1 << 2;
	uint32_t morphReleaseClamp = 0;
	int32_t freqAttackMultiplier = 1 << 10;
	int32_t freqReleaseMultiplier = 1<< 8;

	int32_t transientScale = 1 << 3;
	uint32_t minTransientLength = 14;

	/*
	 *
	 * Event meta_handlers
	 *
	 */

	ViaMeta() : metaUI(*this) {
		init();
	}

	void mainRisingEdgeCallback(void);
	void mainFallingEdgeCallback(void);

	void auxRisingEdgeCallback(void);
	void auxFallingEdgeCallback(void);

	void buttonPressedCallback(void);
	void buttonReleasedCallback(void);

	void ioProcessCallback(void);
	void halfTransferCallback(void);
	void transferCompleteCallback(void);
	void slowConversionCallback(void);

	void auxTimer1InterruptCallback(void);
	void auxTimer2InterruptCallback(void);


	void ui_dispatch(int32_t sig) {
		this->metaUI.dispatch(sig);
	};

};



#endif /* INC_META_HPP_ */
