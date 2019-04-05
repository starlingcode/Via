/*
 * sync_aux_outputs.c
 *
 *  Created on: Aug 30, 2018
 *      Author: willmitchell
 */

#include "sync.hpp"

void ViaSync::calculateLogicAGate(int32_t writeIndex) {

	outputs.logicA[writeIndex] = GET_ALOGIC_MASK(hemisphereState);

}

void ViaSync::calculateLogicADelta(int32_t writeIndex) {

	int32_t thisState = (syncWavetable.delta == 0) ? lastDeltaState : ((uint32_t) syncWavetable.delta >> 31);

	int32_t thisSample = syncWavetable.ghostPhase >> 16;

	outputs.logicA[writeIndex] = GET_ALOGIC_MASK(deltaHysterisis(thisState, thisSample));

}

void ViaSync::calculateDac3Phasor(int32_t writeIndex) {

	int32_t samplesRemaining = outputBufferSize;
	int32_t readIndex = 0;

	while (samplesRemaining) {

		if (syncWavetable.phaseOut[readIndex] >> 24) {
			outputs.dac3Samples[writeIndex] = 8191 - (syncWavetable.phaseOut[readIndex] >> 12);
		} else {
			outputs.dac3Samples[writeIndex] = syncWavetable.phaseOut[readIndex] >> 12;
		}

		readIndex ++;
		writeIndex ++;
		samplesRemaining --;

	}

}

void ViaSync::calculateDac3Contour(int32_t writeIndex) {

	int32_t samplesRemaining = outputBufferSize;
	int32_t readIndex = 0;

	while (samplesRemaining) {
		outputs.dac3Samples[writeIndex] = syncWavetable.signalOut[readIndex];

		writeIndex ++;
		readIndex ++;
		samplesRemaining --;

	}

}

// No S&H

void ViaSync::calculateSHMode1(int32_t writeIndex) {


	outputs.shA[writeIndex] = SH_A_TRACK_MASK;
	outputs.shB[writeIndex] = SH_B_TRACK_MASK;


}

// Sample A from A to B

void ViaSync::calculateSHMode2(int32_t writeIndex) {

	outputs.shA[writeIndex] = GET_SH_A_MASK(hemisphereState);
	outputs.shB[writeIndex] = GET_SH_B_MASK(!hemisphereState);

}

// Resample B at A

void ViaSync::calculateSHMode3(int32_t writeIndex) {


	outputs.shA[writeIndex] = GET_SH_A_MASK(!((!hemisphereState) & hemisphereLastSample));
	outputs.shB[writeIndex] = GET_SH_B_MASK(!(hemisphereState & !hemisphereLastSample));


}
