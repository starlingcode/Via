#include "sync3.hpp"

#define __BEGIN		uint32_t phaseAccumulator2 = phase2; \
					int32_t thisIncrement2 = increment2; \
					uint32_t phaseAccumulator3 = phase3; \
					int32_t thisIncrement3 = increment3 + phaseModIncrement2; \
					uint32_t phaseAccumulator4 = phase4; \
					int32_t thisIncrement4 = increment4 + phaseModIncrement2; \
					int32_t samplesRemaining = VIA_SYNC3_BUFFER_SIZE; \
					while (samplesRemaining) { \
 						phaseAccumulator2 += thisIncrement2; \
						phaseAccumulator3 += thisIncrement3; \
						phaseAccumulator4 += thisIncrement4; \
						phases2[writePosition] = phaseAccumulator2; \
						phases3[writePosition] = phaseAccumulator3; \
						phases4[writePosition] = phaseAccumulator4; \

#define __OP1TRI 		outputs.dac3Samples[writePosition] = (abs((int32_t) (phaseAccumulator2)) >> 19);
#define __OP2TRI 		outputs.dac1Samples[writePosition] = (abs((int32_t) (phaseAccumulator3)) >> 19);
#define __OP3TRI 		outputs.dac2Samples[writePosition] = (abs((int32_t) (phaseAccumulator4)) >> 19);

#define __OP1SQUARE 	outputs.dac3Samples[writePosition] = (((phaseAccumulator2)) >> 31) * 4095;
#define __OP2SQUARE 	outputs.dac1Samples[writePosition] = (((phaseAccumulator3)) >> 31) * 4095;
#define __OP3SQUARE 	outputs.dac2Samples[writePosition] = (((phaseAccumulator4)) >> 31) * 4095;

#define __OP1SAW 		outputs.dac3Samples[writePosition] = (phaseAccumulator2) >> 20;
#define __OP2SAW 		outputs.dac1Samples[writePosition] = (-phaseAccumulator3) >> 20;
#define __OP3SAW 		outputs.dac2Samples[writePosition] = (-phaseAccumulator4) >> 20;

#define __END			writePosition ++; \
						samplesRemaining --; \
					} \
					phase2 = phaseAccumulator2; \
					phase3 = phaseAccumulator3; \
					phase4 = phaseAccumulator4; \



void ViaSync3::updateOutputsSawSawSaw(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2SAW
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsSawSawSq(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2SAW
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsSawSawTri(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2SAW
	__OP3TRI
	__END

}


void ViaSync3::updateOutputsSawSqSaw(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2SQUARE
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsSawSqSq(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2SQUARE
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsSawSqTri(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2SQUARE
	__OP3TRI
	__END

}

void ViaSync3::updateOutputsSawTriSaw(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2TRI
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsSawTriSq(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2TRI
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsSawTriTri(int32_t writePosition) {

	__BEGIN
	__OP1SAW
	__OP2TRI
	__OP3TRI
	__END

}

void ViaSync3::updateOutputsSqSawSaw(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2SAW
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsSqSawSq(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2SAW
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsSqSawTri(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2SAW
	__OP3TRI
	__END

}


void ViaSync3::updateOutputsSqSqSaw(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2SQUARE
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsSqSqSq(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2SQUARE
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsSqSqTri(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2SQUARE
	__OP3TRI
	__END

}

void ViaSync3::updateOutputsSqTriSaw(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2TRI
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsSqTriSq(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2TRI
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsSqTriTri(int32_t writePosition) {

	__BEGIN
	__OP1SQUARE
	__OP2TRI
	__OP3TRI
	__END

}

void ViaSync3::updateOutputsTriSawSaw(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2SAW
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsTriSawSq(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2SAW
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsTriSawTri(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2SAW
	__OP3TRI
	__END

}


void ViaSync3::updateOutputsTriSqSaw(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2SQUARE
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsTriSqSq(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2SQUARE
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsTriSqTri(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2SQUARE
	__OP3TRI
	__END

}

void ViaSync3::updateOutputsTriTriSaw(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2TRI
	__OP3SAW
	__END

}

void ViaSync3::updateOutputsTriTriSq(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2TRI
	__OP3SQUARE
	__END

}

void ViaSync3::updateOutputsTriTriTri(int32_t writePosition) {

	__BEGIN
	__OP1TRI
	__OP2TRI
	__OP3TRI
	__END

}
