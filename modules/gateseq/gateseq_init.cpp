#include <gateseq.hpp>

void ViaGateseq::init(void) {

	initializeAuxOutputs();

	initializePatterns();

	gateController.updateGateA(SOFT_GATE_HIGH);
	gateController.updateGateB(SOFT_GATE_HIGH);

	sequencer.currentABank = seq1Banks[0];
	sequencer.currentBBank = seq2Banks[0];

	sequencer.gateAEvent = SOFT_GATE_EXECUTE;
	sequencer.gateBEvent = SOFT_GATE_EXECUTE;

	inputs.init(GATESEQ_BUFFER_SIZE);
	outputs.init(GATESEQ_BUFFER_SIZE);
	inputBufferSize = GATESEQ_BUFFER_SIZE;
	outputBufferSize = GATESEQ_BUFFER_SIZE;

	gateseqUI.initialize();

	uint32_t optionBytes = readOptionBytes();
	uint32_t ob1Data = optionBytes & 0xFFFF;
	uint32_t ob2Data = optionBytes >> 16;

	if (ob1Data == 254 && ob2Data == 255) {
		readCalibrationPacket();
		gateseqUI.writeStockPresets();
		writeOptionBytes(4, 1);
	} else if (ob1Data == 4) {
		readCalibrationPacket();
	}
//	} else if (ob1Data != 0) {
//		writeOptionBytes(0, 0);
//	}

	sequencer.cv1Offset = cv1Calibration;
	sequencer.cv2Offset = cv2Calibration;
	sequencer.cv3Offset = cv3Calibration;

}

