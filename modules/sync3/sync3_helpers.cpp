#include "sync3.hpp"

void ViaSync3::updateOutputsSaw(int32_t writePosition) {
	uint32_t phaseAccumulator1 = phase1;
	int32_t thisIncrement1 = increment1;
	uint32_t phaseAccumulator2 = phase2;
	int32_t thisIncrement2 = increment2;
	uint32_t phaseAccumulator3 = phase3;
	int32_t thisIncrement3 = increment3;
	uint32_t phaseAccumulator4 = phase4;
	int32_t thisIncrement4 = increment4;

	int32_t samplesRemaining = VIA_SYNC3_BUFFER_SIZE;
	while (samplesRemaining) {
		phaseAccumulator1 += thisIncrement1;
		phaseAccumulator2 += thisIncrement2;
		phaseAccumulator3 += thisIncrement3;
		phaseAccumulator4 += thisIncrement4;
		phases[VIA_SYNC3_BUFFER_SIZE - samplesRemaining] = phaseAccumulator1;
		outputs.dac3Samples[writePosition] = phaseAccumulator2 >> 20;
		outputs.dac1Samples[writePosition] = phaseAccumulator3 >> 20;
		outputs.dac2Samples[writePosition] = phaseAccumulator4 >> 20;
		writePosition ++;
		samplesRemaining --;
	}
	phase1 = phaseAccumulator1;
	phase2 = phaseAccumulator2;
	phase3 = phaseAccumulator3;
	phase4 = phaseAccumulator4;
}

void ViaSync3::updateOutputsSquare(int32_t writePosition) {
	uint32_t phaseAccumulator1 = phase1;
	int32_t thisIncrement1 = increment1;
	uint32_t phaseAccumulator2 = phase2;
	int32_t thisIncrement2 = increment2;
	uint32_t phaseAccumulator3 = phase3;
	int32_t thisIncrement3 = increment3;
	uint32_t phaseAccumulator4 = phase4;
	int32_t thisIncrement4 = increment4;

	int32_t samplesRemaining = VIA_SYNC3_BUFFER_SIZE;
	while (samplesRemaining) {
		phaseAccumulator1 += thisIncrement1;
		phaseAccumulator2 += thisIncrement2;
		phaseAccumulator3 += thisIncrement3;
		phaseAccumulator4 += thisIncrement4;
		phases[VIA_SYNC3_BUFFER_SIZE - samplesRemaining] = phaseAccumulator1;
		outputs.dac3Samples[writePosition] = ((phaseAccumulator2) >> 31) * 4095;
		outputs.dac1Samples[writePosition] = ((phaseAccumulator3) >> 31) * 4095;
		outputs.dac2Samples[writePosition] = ((phaseAccumulator4) >> 31) * 4095;
		writePosition ++;
		samplesRemaining --;
	}
	phase1 = phaseAccumulator1;
	phase2 = phaseAccumulator2;
	phase3 = phaseAccumulator3;
	phase4 = phaseAccumulator4;
}


void ViaSync3::updateOutputsTriangle(int32_t writePosition) {
	uint32_t phaseAccumulator1 = phase1;
	int32_t thisIncrement1 = increment1;
	uint32_t phaseAccumulator2 = phase2;
	int32_t thisIncrement2 = increment2;
	uint32_t phaseAccumulator3 = phase3;
	int32_t thisIncrement3 = increment3;
	uint32_t phaseAccumulator4 = phase4;
	int32_t thisIncrement4 = increment4;

	int32_t samplesRemaining = VIA_SYNC3_BUFFER_SIZE;
	while (samplesRemaining) {
		phaseAccumulator1 += thisIncrement1;
		phaseAccumulator2 += thisIncrement2;
		phaseAccumulator3 += thisIncrement3;
		phaseAccumulator4 += thisIncrement4;
		phases[VIA_SYNC3_BUFFER_SIZE - samplesRemaining] = phaseAccumulator1;
		outputs.dac3Samples[writePosition] = (abs((int32_t) phaseAccumulator2) >> 19);
		outputs.dac1Samples[writePosition] = (abs((int32_t) phaseAccumulator3) >> 19);
		outputs.dac2Samples[writePosition] = (abs((int32_t) phaseAccumulator4) >> 19);
		writePosition ++;
		samplesRemaining --;
	}
	phase1 = phaseAccumulator1;
	phase2 = phaseAccumulator2;
	phase3 = phaseAccumulator3;
	phase4 = phaseAccumulator4;
}
