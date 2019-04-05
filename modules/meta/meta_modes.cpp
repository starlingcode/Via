/*
 * modes.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */

#include "meta.hpp"

void ViaMeta::handleButton1ModeChange(int32_t mode) {

	switch (mode) {
	case nosampleandhold:
		calculateSH = &ViaMeta::calculateSHMode1;
		break;
	case a:
		calculateSH = &ViaMeta::calculateSHMode2;
		break;
	case b:
		calculateSH = &ViaMeta::calculateSHMode3;
		break;
	case ab:
		calculateSH = &ViaMeta::calculateSHMode4;
		break;
	case halfdecimate:
		calculateSH = &ViaMeta::calculateSHMode5;
		break;
	case meta_decimate:
		calculateSH = &ViaMeta::calculateSHMode6;
		break;
	}

	setSH(0, 0);
}

void ViaMeta::handleButton2ModeChange(int32_t mode) {

	switchWavetable(wavetableArray[metaUI.FREQ_MODE][mode]);

}

void ViaMeta::handleButton3ModeChange(int32_t mode) {

	switchWavetable(wavetableArray[mode][metaUI.TABLE]);

	switch (mode) {
	case audio:

		if (metaUI.LOOP_MODE == noloop) {
			updateRGBDisplay(0, 4095, 4095, 1);
			initializeDrum();
		} else {
			updateRGBDisplay(0, 0, 4095, 1);
			initializeOscillator();
		}

		break;
	case env:

		updateRGBDisplay(0, 4095, 0, 1);

		if (metaUI.LOOP_MODE == noloop) {
			initializeEnvelope();
		} else {
			initializeSimpleLFO();
		}

		break;
	case seq:

		updateRGBDisplay(4095, 0, 0, 1);

		if (metaUI.LOOP_MODE == noloop) {
			initializeSequence();
		} else {
			initializeComplexLFO();
		}

		break;
	}

}

void ViaMeta::handleButton4ModeChange(int32_t mode) {

	metaController.atB = 0;

	switch (mode) {
	case noretrigger:
		metaController.incrementArbiter = &MetaController::noRetrigAttackState;
		break;
	case meta_hardsync:
		metaController.incrementArbiter = &MetaController::hardSyncAttackState;
		break;
	case nongatedretrigger:
		metaController.incrementArbiter = &MetaController::envAttackState;
		break;
	case gated:
		metaController.incrementArbiter = &MetaController::gateAttackState;
		metaController.phase = 0;
		metaController.gateOn = 1;
		break;
	case meta_pendulum:
		if (metaUI.LOOP_MODE == 1) {
			metaController.incrementArbiter = &MetaController::pendulumForwardAttackState;
		} else {
			metaController.incrementArbiter = &MetaController::stickyPendulumRestingState;
		}
		metaController.gateOn = 0;
		break;
	}

}

void ViaMeta::handleButton5ModeChange(int32_t mode) {

	switchWavetable(wavetableArray[metaUI.FREQ_MODE][mode]);

}

void ViaMeta::handleButton6ModeChange(int32_t mode) {

	switch (mode) {
	case noloop:
		if (metaUI.FREQ_MODE == audio) {
			initializeDrum();
		} else {
			if (metaUI.TRIG_MODE == meta_pendulum)  {
				metaController.incrementArbiter = &MetaController::stickyPendulumRestingState;
			}
			if (metaUI.FREQ_MODE == env)  {
				initializeEnvelope();
			} else {
				initializeSequence();
			}
		}

		break;
	case looping:
		if (metaUI.FREQ_MODE == audio) {
			initializeOscillator();
		} else if (metaUI.FREQ_MODE == env)  {
			initializeSimpleLFO();
		} else {
			initializeComplexLFO();
		}
		if (metaUI.TRIG_MODE == meta_pendulum)  {
			metaController.incrementArbiter = &MetaController::pendulumForwardAttackState;
		}
		break;
	}

}

void ViaMeta::handleAux1ModeChange(int32_t mode) {

	switch (mode) {
	case drumPhasor:
		calculateDac3 = &ViaMeta::calculateDac3Phasor;
		break;
	case drumContour:
		calculateDac3 = &ViaMeta::calculateDac3Contour;
		break;
	case drumEnv:
		calculateDac3 = &ViaMeta::calculateDac3DrumEnv;
		break;
	case noise:
		calculateDac3 = &ViaMeta::calculateDac3Noise;
	}


}

void ViaMeta::handleAux2ModeChange(int32_t mode) {

	switch (mode) {
	case releaseGate:
		calculateLogicA = &ViaMeta::calculateLogicAReleaseGate;
		break;
	case attackGate:
		calculateLogicA = &ViaMeta::calculateLogicAAttackGate;
		break;
	}

}

void ViaMeta::handleAux3ModeChange(int32_t mode) {

	switch (mode) {
	case pitchMorphAmp:
		metaController.fm = (int16_t*) morphEnvelope.output;
		metaWavetable.morphScale = (int16_t*) ampEnvelope.output;

		ampEnvelope.attack = 2 << 16;
		morphAttackMultiplier = 1 << 8;
		morphReleaseMultiplier = 1 << 3;
		freqAttackMultiplier = 1 << 13;
		freqReleaseMultiplier = 1<< 12;
		transientScale = 1 << 3;
		minTransientLength = 10000;
		morphReleaseClamp = 1000;

		break;
	case pitchAmp:
		metaController.fm = (int16_t*) morphEnvelope.output;
		metaWavetable.morphScale = drumFullScale;

		ampEnvelope.attack = 1 << 16;
		morphAttackMultiplier = 1 << 8;
		morphReleaseMultiplier = 1 << 1;
		freqAttackMultiplier = 1 << 11;
		freqReleaseMultiplier = 1<< 7;
		transientScale = 1 << 1;
		minTransientLength = 10000;
		morphReleaseClamp = 0;

		break;
	case morphAmp:
		metaController.fm = drumFullScale;
		metaWavetable.morphScale = (int16_t*) morphEnvelope.output;

		ampEnvelope.attack = 2 << 16;
		morphAttackMultiplier = 1 << 6;
		morphReleaseMultiplier = 1;
		freqAttackMultiplier = 1 << 13;
		freqReleaseMultiplier = 1<< 12;
		transientScale = 1 << 2;
		minTransientLength = 0;
		morphReleaseClamp = 0;


		break;
	case amp:
		metaController.fm = drumFullScale;
		metaWavetable.morphScale = drumFullScale;

		ampEnvelope.attack = 2 << 16;
		morphAttackMultiplier = 1 << 8;
		morphReleaseMultiplier = 1 << 2;
		freqAttackMultiplier = 1 << 13;
		freqReleaseMultiplier = 1<< 12;
		transientScale = 1;
		minTransientLength = 20;
		morphReleaseClamp = 0;

		break;
	}

}

void ViaMeta::handleAux4ModeChange(int32_t mode) {

	switch (mode) {
	case phasor:
		if (metaUI.FREQ_MODE == env) {
			calculateDac3 = &ViaMeta::calculateDac3PhasorEnv;
		} else {
			calculateDac3 = &ViaMeta::calculateDac3Phasor;
		}
		break;
	case contour:
		if (metaUI.FREQ_MODE == env) {
			calculateDac3 = &ViaMeta::calculateDac3ContourEnv;
		} else {
			calculateDac3 = &ViaMeta::calculateDac3Contour;
		}

		break;
	}

}

void ViaMeta::initializeDrum(void) {

	updateRGB = &ViaMeta::updateRGBDrum;
	currentRGBBehavior = &ViaMeta::updateRGBDrum;

	metaController.generateIncrements = &MetaController::generateIncrementsDrum;
	metaController.parseControls = &MetaController::parseControlsDrum;
	// metaController.fm = drumFullScale; // set in the drum mode handler
	metaController.expoFM = freqTransient.output;
	metaController.advancePhase = &MetaController::advancePhaseOversampled;
	metaController.loopHandler = &MetaController::handleLoopOn;
	metaController.loopMode = 1;

	// metaWavetable.morphScale = drumFullScale; // set in the drum mode handler
	metaWavetable.oversamplingFactor = 0;

	outputStage = &ViaMeta::drumMode;

	handleButton4ModeChange(0);
	handleAux1ModeChange(metaUI.DRUM_AUX_MODE);
	handleAux3ModeChange(metaUI.DRUM_MODE);
}
void ViaMeta::initializeOscillator(void) {

	updateRGB = &ViaMeta::updateRGBOsc;
	currentRGBBehavior = &ViaMeta::updateRGBOsc;

	metaController.parseControls = &MetaController::parseControlsAudio;
	metaController.generateIncrements = &MetaController::generateIncrementsAudio;
	metaController.advancePhase = &MetaController::advancePhaseOversampled;
	metaController.fm = inputs.cv2Samples;
	metaController.expoFM = drumOff;
	metaController.loopHandler = &MetaController::handleLoopOn;
	metaController.loopMode = 1;

	metaWavetable.morphScale = drumFullScale;
	metaWavetable.oversamplingFactor = 3;

	outputStage = &ViaMeta::oversample;

	handleButton4ModeChange(metaUI.TRIG_MODE);
	handleAux4ModeChange(metaUI.DAC_3_MODE);

}
void ViaMeta::initializeEnvelope(void) {

	updateRGB = &ViaMeta::updateRGBSubaudio;
	currentRGBBehavior = &ViaMeta::updateRGBSubaudio;

	metaController.parseControls = &MetaController::parseControlsEnv;
	metaController.generateIncrements = &MetaController::generateIncrementsEnv;
	metaController.advancePhase = &MetaController::advancePhasePWM;
	metaController.fm = inputs.cv2Samples;
	metaController.expoFM = drumOff;
	metaController.loopHandler = &MetaController::handleLoopOff;
	metaController.loopMode = 0;

	metaWavetable.oversamplingFactor = 0;
	metaWavetable.morphScale = drumFullScale;

	outputStage = &ViaMeta::addThreeBits;

	handleButton4ModeChange(metaUI.TRIG_MODE);
	handleAux4ModeChange(metaUI.DAC_3_MODE);

}
void ViaMeta::initializeSimpleLFO(void) {

	updateRGB = &ViaMeta::updateRGBSubaudio;
	currentRGBBehavior = &ViaMeta::updateRGBSubaudio;

	metaController.parseControls = &MetaController::parseControlsEnv;
	metaController.generateIncrements = &MetaController::generateIncrementsEnv;
	metaController.advancePhase = &MetaController::advancePhasePWM;
	metaController.fm = inputs.cv2Samples;
	metaController.expoFM = drumOff;
	metaController.loopHandler = &MetaController::handleLoopOn;
	metaController.loopMode = 1;

	metaWavetable.oversamplingFactor = 0;
	metaWavetable.morphScale = drumFullScale;

	outputStage = &ViaMeta::addThreeBits;

	handleButton4ModeChange(metaUI.TRIG_MODE);
	handleAux4ModeChange(metaUI.DAC_3_MODE);

}
void ViaMeta::initializeSequence(void) {

	updateRGB = &ViaMeta::updateRGBSubaudio;
	currentRGBBehavior = &ViaMeta::updateRGBSubaudio;

	metaController.parseControls = &MetaController::parseControlsSeq;
	metaController.generateIncrements = &MetaController::generateIncrementsSeq;
	metaController.advancePhase = &MetaController::advancePhasePWM;
	metaController.fm = drumFullScale;
	metaController.expoFM = drumOff;
	metaController.loopHandler = &MetaController::handleLoopOff;
	metaController.loopMode = 0;

	metaWavetable.oversamplingFactor = 0;
	metaWavetable.morphScale = drumFullScale;

	outputStage = &ViaMeta::addThreeBits;

	handleButton4ModeChange(metaUI.TRIG_MODE);
	handleAux4ModeChange(metaUI.DAC_3_MODE);

}
void ViaMeta::initializeComplexLFO(void) {

	updateRGB = &ViaMeta::updateRGBSubaudio;
	currentRGBBehavior = &ViaMeta::updateRGBSubaudio;

	metaController.parseControls = &MetaController::parseControlsSeq;
	metaController.generateIncrements = &MetaController::generateIncrementsSeq;
	metaController.advancePhase = &MetaController::advancePhasePWM;
	metaController.fm = drumFullScale;
	metaController.expoFM = drumOff;
	metaController.loopHandler = &MetaController::handleLoopOn;
	metaController.loopMode = 1;

	metaWavetable.oversamplingFactor = 0;
	metaWavetable.morphScale = drumFullScale;

	outputStage = &ViaMeta::addThreeBits;

	handleButton4ModeChange(metaUI.TRIG_MODE);
	handleAux4ModeChange(metaUI.DAC_3_MODE);

}



