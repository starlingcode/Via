/*
 * meta_init.cpp
 *
 *  Created on: Sep 12, 2018
 *      Author: willmitchell
 */

#include "meta.hpp"


void ViaMeta::init() {

	initializeAuxOutputs();

	outputStage = &ViaMeta::oversample;
	updateRGB = &ViaMeta::updateRGBSubaudio;
	calculateDac3 = &ViaMeta::calculateDac3Phasor;
	calculateLogicA = &ViaMeta::calculateLogicAReleaseGate;
	calculateSH = &ViaMeta::calculateSHMode1;

	metaController.parseControls = &MetaController::parseControlsDrum;
	metaController.generateIncrements = &MetaController::generateIncrementsDrum;
	metaController.incrementArbiter = &MetaController::noRetrigAttackState;
	metaController.advancePhase = &MetaController::advancePhaseOversampled;

	metaController.loopHandler = &MetaController::handleLoopOn;

	ampEnvelope.incrementArbiter = &SimpleEnvelope::restingState;
	freqTransient.incrementArbiter = &SimpleEnvelope::restingState;
	morphEnvelope.incrementArbiter = &SimpleEnvelope::restingState;



	// initialize our touch sensors

	initDrum();
	ampEnvelope.output = (int32_t*) drumWrite;
	freqTransient.output = (int32_t*) drum2Write;
	morphEnvelope.output = (int32_t*) drum3Write;



	inputs.init(META_BUFFER_SIZE);
	outputs.init(META_BUFFER_SIZE);
	outputBufferSize = META_BUFFER_SIZE;
	inputBufferSize = 1;

	metaUI.initialize();

	// switchWavetable(wavetableArray[0][0]);

	metaController.triggerSignal = 1;
	metaController.gateSignal = 0;
	metaController.freeze = 1;

	metaWavetable.morphMod = inputs.cv3Samples;

	uint32_t optionBytes = readOptionBytes();
	uint32_t ob1Data = optionBytes & 0xFFFF;
	uint32_t ob2Data = optionBytes >> 16;

	if (ob1Data == 254 && ob2Data == 255) {
		readCalibrationPacket();
		metaUI.writeStockPresets();
		writeOptionBytes(1, 3);
	} else if (ob1Data == 1) {
		readCalibrationPacket();
		if (ob2Data != 3) {
			writeOptionBytes(1, 3);
		}
	} else if (ob1Data != 0) {
		writeOptionBytes(0, 0);
	}

	metaController.cv1Offset = cv1Calibration;
	metaController.cv2Offset = cv2Calibration;
	metaWavetable.morphModOffset = cv3Calibration;
	dac3OffsetCompensation = (65535 - (dac3Calibration << 5));

}
