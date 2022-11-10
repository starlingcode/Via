/*
 * sync.hpp
 *
 *  Created on: Sep 16, 2018
 *      Author: willmitchell
 */

#ifndef INC_SYNC_HPP_
#define INC_SYNC_HPP_

#include "user-interface.hpp"
#include <via-platform-binding.hpp>
#include "sync-scale-defs.hpp"
#include "tables.hpp"
#ifdef BUILD_VIRTUAL
#include <stdlib.h>
#include <stdio.h>
#include <string>
#endif


class SyncWavetable {

	int32_t previousPhase = 0;
	int32_t previousPhaseMod = 0;

public:

	// assigned per mode
	int16_t * fm;
	int16_t * pm;
	int16_t * pwm;
	int16_t * morphMod;
	int32_t cv2Offset = 0;
	int32_t cv3Offset = 0;
	uint32_t tableSize = 0;

	// generated externally
	int32_t phaseReset = 1;
	int32_t increment = 0;
	int32_t morphBase = 0;

	// results
	int32_t phaseMod = 0;
	uint32_t phase = 0;
	int32_t ghostPhase = 0;
	int32_t phaseEvent = 0;
	int32_t delta = 0;

	uint32_t phaseOut[32];
	uint32_t purePhaseOut[16];
	uint32_t * signalOut;

	int32_t oversamplingFactor = 3;
	int32_t bufferSize = 8;

	void parseControls(ViaControls * controls);

	void oversample(uint32_t * wavetable, uint32_t writePosition);

	void spline(uint32_t * wavetable, uint32_t writePosition);

	void advance(uint32_t * wavetable, uint32_t writePosition) {
		if (increment > (1 << 22)) {
			oversample(wavetable, writePosition);
		} else {
			spline(wavetable, writePosition);
		}
	}

	uint32_t fix32Mul(uint64_t in0, uint64_t in1) {
		uint64_t result = in0 * in1;
		return result >> 32;
	}

	inline uint32_t phaseDist(uint32_t phase, uint32_t bend, uint32_t bendUp, uint32_t bendDown) {

		bend <<= 16;

		if (phase < bend) {
			uint32_t bendFrac = fix16_mul(phase >> 1, bendUp);
			return bendFrac;

		} else {

			uint32_t bendFrac = fix16_mul((phase - bend) >> 1, bendDown);
			return (1 << 31) + bendFrac;

		}

	}

	SyncWavetable() {
		for (int i = 0; i < 32; i++) {
			phaseOut[i] = 0;
			purePhaseOut[i >> 1] = 0;
		}
	}


};

#define SYNC_BUFFER_SIZE 8

/*
 *
 * Modes
 *
 */

// Mode enums and mode variables

#define SH_MODE button1Mode
#define SCALE_MODE button2Mode
#define X_MODE button3Mode
#define SYNC_MODE button4Mode
#define GROUP_MODE button5Mode
#define TABLE_MODE button6Mode
#define LOGIC_A_MODE aux1Mode
#define QUADRATURE_MODE aux2Mode
#define AUX_OUT_MODE aux3Mode
#define TABLE_GROUP_MODE aux4Mode

#define numButton1Modes 3
#define numButton2Modes 4
#define numButton3Modes 3
#define numButton4Modes 4
#define numButton5Modes 4
#define numButton6Modes 4
#define numAux1Modes 2
#define numAux2Modes 2
#define numAux3Modes 4
#define numAux4Modes 2

enum sync_button1Modes {noSH, sampletrack, resample};
enum sync_button2Modes {scale1, scale2, scale3, scale4};
enum sync_button3Modes {root, pm, pwm};
enum sync_button4Modes {nosync, truePLL, hardsync, wobble};
enum sync_button5Modes {group1, group2, group3, group4};
enum sync_button6Modes {table1, table2, table3, table4};
enum sync_aux1Modes {sync_gate, sync_delta};
enum sync_aux2Modes {sync_phasor, sync_contour};
enum sync_aux3Modes {noOffset, quarter, half, threeQuarters};
enum sync_aux4Modes {groupSpecific, global};

void syncTouchLink (void *);

class ViaSync : public TARGET_VIA {

public:

	class ViaSyncUI: public ViaUI {

	public:

		ViaSync& this_module;

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

		// TODO use enums
		// (sh, scale, mod, sync, group, wave, logic, signal, quadrature, global)
		// harmonic oscillator
		uint32_t stockPreset1 = ENCODE_PRESET(1, 0, 0, 2, 0, 0, 0, 0, 2, 0);
		// arpeggiated oscillator
		uint32_t stockPreset2 = ENCODE_PRESET(0, 2, 0, 1, 1, 0, 1, 0, 0, 0);
		// arpeggiated oscillator bp
		uint32_t stockPreset3 = ENCODE_PRESET(2, 3, 1, 0, 1, 0, 1, 0, 2, 1);
		// voct
		uint32_t stockPreset4 = ENCODE_PRESET(0, 0, 1, 0, 2, 3, 0, 1, 0, 0);
		// sequencer
		uint32_t stockPreset5 = ENCODE_PRESET(2, 1, 2, 3, 3, 3, 0, 0, 0, 0);
		// complex LFO
		uint32_t stockPreset6 = ENCODE_PRESET(0, 0, 2, 2, 3, 0, 1, 1, 0, 0);

		ViaSyncUI(ViaSync& x): this_module(x) {
			linkUI((void *) &syncTouchLink, (void *) this);
		}

	};

	void handleButton1ModeChange(int32_t);
	void handleButton2ModeChange(int32_t);
	void handleButton3ModeChange(int32_t);
	void handleButton4ModeChange(int32_t);
	void handleButton5ModeChange(int32_t);
	void handleButton5ModeInit(int32_t);
	void handleButton6ModeChange(int32_t);
	void handleAux1ModeChange(int32_t);
	void handleAux2ModeChange(int32_t);
	void handleAux3ModeChange(int32_t);
	void handleAux4ModeChange(int32_t);

	/*
	 *
	 * Tables
	 *
	 */

    int32_t tableMemMaxSize = (20 * 16) + (9 * 257 * 20 * 2 * 2);

    // contains load methods
    WavetableSet wavetableSet;

#ifdef BUILD_F373

	const Wavetable * wavetableArray = (const Wavetable *) 0x8020000;
	
#endif
#ifdef BUILD_VIRTUAL
    Wavetable * wavetableArray;
    void readTableSetFromFile(std::string path) {
        FILE * tableFile = fopen(path.c_str(), "rb");
        if (tableFile == NULL) {
            return; // TODO: Error handling for file not exist or something
        }
        fread(wavetableArray, tableMemMaxSize, 1, tableFile);
        fclose(tableFile);
    }
#endif

	// declare arrays to store the active tables
	uint32_t wavetableRead[9][517];

    // declare functions to set the currently active tables
    #ifdef BUILD_F373
    void switchWavetable(const Wavetable * table) {
    #endif
    #ifdef BUILD_VIRTUAL
    void switchWavetable(Wavetable * table) {
    #endif
        wavetableSet.loadWavetableWithDiff(table, (uint32_t *) wavetableRead);
        syncWavetable.tableSize = table->numWaveforms - 1;
    }


	const Scale * scaleArray[4][4];

	const Scale * scale;

	void initializeScales(void);


	/*
	 *
	 * Mode functions
	 *
	 */

	uint16_t virtualFM[2];
	uint16_t virtualMorph[2];

	void (ViaSync::*calculateDac3)(int32_t writeIndex);

	void calculateDac3Phasor(int32_t writeIndex);
	void calculateDac3Contour(int32_t writeIndex);

	void (ViaSync::*calculateLogicA)(int32_t writeIndex);

	void calculateLogicAGate(int32_t writeIndex);
	void calculateLogicADelta(int32_t writeIndex);

	int32_t hemisphereState = 0;
	int32_t lastHemisphereState = 1;
	int32_t hemisphereTransitionSample = 0;
	int32_t hemisphereOutputStable = 0;
	int32_t hemisphereLastSample = 0;

	void hemisphereHysterisis(int32_t thisHemisphereState, int32_t sample) {

		if (hemisphereOutputStable) {
			hemisphereOutputStable = ((lastHemisphereState - thisHemisphereState) == 0);
			hemisphereTransitionSample = sample;
			lastHemisphereState = thisHemisphereState;
			hemisphereState =  thisHemisphereState;
		} else {
			hemisphereOutputStable = abs(sample - hemisphereTransitionSample) > 1;
			hemisphereOutputStable &= (sample - hemisphereTransitionSample) != 511;
			lastHemisphereState = hemisphereOutputStable ? thisHemisphereState : lastHemisphereState;
			hemisphereState = lastHemisphereState;
		}

	}

	int32_t deltaState = 0;
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
			deltaOutputStable = abs(sample - deltaTransitionSample) > 1;
			deltaOutputStable &= (sample - deltaTransitionSample) != 511;
			lastDeltaState = deltaOutputStable ? thisDeltaState : lastDeltaState;
			return lastDeltaState;
		}

	}

	void (ViaSync::*calculateSH)(int32_t writeIndex);
	// No S&H
	void calculateSHMode1(int32_t writeIndex);
	// Track and hold
	void calculateSHMode2(int32_t writeIndex);
	// Decimate
	void calculateSHMode3(int32_t writeIndex);

	void init(void);

	ViaSyncUI syncUI;

	int32_t runtimeDisplay;

	int32_t showYChange;

	SyncWavetable syncWavetable;
	
	uint32_t pllCounter = 0;
	int32_t lastMultiplier = 1;
	int32_t lastYIndex = 0;

	int32_t doCorrection = 1;

	int32_t ratioXTransitionPoint = 0;
	int32_t ratioXStable = 1;

	int32_t ratioXHysterisis(int32_t thisRatioX, int32_t control) {

		if (ratioXStable) {
			ratioXStable = ((lastRatioX - thisRatioX) == 0);
			ratioXTransitionPoint = control & 0b111111100000;
			lastRatioX = thisRatioX;
			return thisRatioX;
		} else {
			ratioXStable = abs(control - ratioXTransitionPoint) > 8;
			lastRatioX = ratioXStable ? thisRatioX : lastRatioX;
			return lastRatioX;
		}

	}

	int32_t ratioYTransitionPoint = 0;
	int32_t ratioYStable = 1;

	int32_t ratioYHysterisis(int32_t control, int32_t shiftAmount) {

		int32_t thisRatioY = control >> shiftAmount;

		if (ratioYStable) {
			ratioYStable = ((lastRatioY - thisRatioY) == 0);
			ratioYTransitionPoint = thisRatioY << shiftAmount;
			lastRatioY = thisRatioY;
			return thisRatioY;
		} else {
			ratioYStable = abs(control - ratioYTransitionPoint) > 8;
			lastRatioY = ratioYStable ? thisRatioY : lastRatioY;
			return lastRatioY;
		}

	}

	int32_t lastRatioX = 1;
	int32_t lastRatioY = 1;

	uint32_t periodCount = 48000;
	uint32_t aggregatePeriod = 48000;
	uint32_t pileUp = 0;
	uint32_t clockDiv = 0;
	uint32_t skipPll = 0;
	int32_t pllNudge = 0;
	buffer nudgeBuffer;
	int32_t nudgeSum = 0;

	uint32_t phaseSignal = 0;
	uint32_t phaseModSignal = 0;
	uint32_t tapTempo = 0;
	uint32_t pllReset = 0;

	int16_t * rootMod;
	uint32_t phaseOffset = 0;
	uint32_t syncMode = 0;
	Scale * selectedScale;
	int32_t cv2Offset;
	int32_t cv1Offset;

	uint32_t fracMultiplier = 0;
	uint32_t intMultiplier = 0;
	uint32_t gcd = 1;

	uint32_t increment = 0;
	uint32_t phaseReset = 0;
	uint32_t ratioChange = 0;
	uint32_t yIndexChange = 0;

	uint32_t errorSig = 0;

	void parseControls(ViaControls * controls, ViaInputStreams * input);

	inline void measureFrequency(uint32_t reading) {

#ifdef BUILD_F373

		// store the length of the last period
		periodCount = reading;
		// reset the timer value
		TIM2->CNT = 0;

#endif

#ifdef BUILD_VIRTUAL

		periodCount = reading;
		resetMeasurementTimer();

#endif

	}

	void doPLL(void);
	void generateFrequency(void);

	inline void updateFrequency(void) {
		if (doCorrection == 0) {
			doPLL();
			generateFrequency();
			syncWavetable.increment = increment;
			doCorrection = 1;
		}
	}

	// average tap tempo
	int32_t lastTap = 0;
	buffer tapStore;
	int32_t tapSum = 0;

	int32_t simultaneousTrigFlag = 0;

	/*
	 *
	 * Event meta_handlers
	 *
	 */

#ifdef BUILD_F373
	ViaSync() : syncUI(*this) {
#endif
#ifdef BUILD_VIRTUAL
    ViaSync(std::string binPath) : syncUI(*this) {
#endif

        #ifdef BUILD_VIRTUAL
        wavetableArray = (Wavetable *) malloc(tableMemMaxSize);
        readTableSetFromFile(binPath);
        #endif
        wavetableSet.startAddress = ((uint16_t *) wavetableArray) + 8 * 20;
		init();
		for (int32_t i = 0; i < 32; i++) {
			writeBuffer(&tapStore, 0);
		}
	}

	void readCalibrationPacket(void) {
		calibrationPacket = syncUI.loadFromMemory(7);
		decodeCalibrationPacket();
	}

	rgb scaleColor;
	int32_t scaleHue;

	void mainRisingEdgeCallback(void) override;
	void mainFallingEdgeCallback(void) override;

	void auxRisingEdgeCallback(void) override;
	void auxFallingEdgeCallback(void) override;

	void buttonPressedCallback(void) override;
	void buttonReleasedCallback(void) override;

	void ioProcessCallback(void) override;
	void halfTransferCallback(void) override;
	void transferCompleteCallback(void) override;
	void slowConversionCallback(void) override;

	void auxTimer1InterruptCallback(void) override;
	void auxTimer2InterruptCallback(void) override;

	void ui_dispatch(int32_t sig) {
		syncUI.dispatch(sig);
	};

	int32_t virtualTimer = 0;
	int32_t virtualTimerEnable = 0;
	int32_t virtualTimerOverflow = 48;

};


#endif /* INC_SYNC_HPP_ */
