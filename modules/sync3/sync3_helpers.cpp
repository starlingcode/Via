#include "sync3.hpp"

void ViaSync3::updateOutputs(int32_t writePosition) {
	uint32_t phaseAccumulator1 = phase1;
	int32_t thisIncrement1 = increment1;
	uint32_t phaseAccumulator2 = phase2;
	int32_t thisIncrement2 = increment2;
	uint32_t phaseAccumulator3 = phase3;
	int32_t thisIncrement3 = increment3;

	int32_t samplesRemaining = VIA_SYNC3_BUFFER_SIZE;
	while (samplesRemaining) {
		phaseAccumulator1 += thisIncrement1;
		phaseAccumulator2 += thisIncrement2;
		phaseAccumulator3 += thisIncrement3;
		phases[VIA_SYNC3_BUFFER_SIZE - samplesRemaining] = phaseAccumulator1;
		outputs.dac1Samples[writePosition] = (abs((int32_t) phaseAccumulator1) >> 19);
		outputs.dac2Samples[writePosition] = (abs((int32_t) phaseAccumulator2) >> 19);
		outputs.dac3Samples[writePosition] = (abs((int32_t) phaseAccumulator3) >> 19);
		writePosition ++;
		samplesRemaining --;
	}
	phase1 = phaseAccumulator1;
	phase2 = phaseAccumulator2;
	phase3 = phaseAccumulator3;
}
