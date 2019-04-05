
#include "sync.hpp"

void ViaSync::init(void) {

	initializeAuxOutputs();

	fillWavetableArray();

	// switchWavetable(wavetableArray[0][0]);

	initializeScales();

	pllController.scale = (Scale *) scaleArray[0][0];

	calculateDac3 = &ViaSync::calculateDac3Phasor;
	calculateLogicA = &ViaSync::calculateLogicAGate;
	calculateSH = &ViaSync::calculateSHMode1;

	inputs.init(SYNC_BUFFER_SIZE);
	outputs.init(SYNC_BUFFER_SIZE);
	inputBufferSize = 1;
	outputBufferSize = SYNC_BUFFER_SIZE;

	pllController.rootMod = inputs.cv2Samples;
	syncWavetable.fm = inputs.cv2VirtualGround;
	syncWavetable.pm = inputs.cv2VirtualGround;
	syncWavetable.pwm = inputs.cv2VirtualGround;

	for (int i = 0; i < 32; i++) {
		writeBuffer(&pllController.nudgeBuffer, 0);
	}

	syncWavetable.morphMod = inputs.cv3Samples;

	syncWavetable.phaseReset = 1;
	pllController.phaseReset = 1;

	syncWavetable.increment = 10000;

	syncUI.initialize();

	uint32_t optionBytes = readOptionBytes();
	uint32_t ob1Data = optionBytes & 0xFFFF;
	uint32_t ob2Data = optionBytes >> 16;

	if (ob1Data == 254 && ob2Data == 255) {
		readCalibrationPacket();
		syncUI.writeStockPresets();
		writeOptionBytes(2, 1);
	} else if (ob1Data == 2) {
		readCalibrationPacket();
	} else if (ob1Data != 0) {
		writeOptionBytes(0, 0);
	}

	inputs.cv2VirtualGround[0] = cv2Calibration;
	inputs.cv3VirtualGround[0] = cv3Calibration;
	pllController.cv1Offset = cv1Calibration;
	pllController.cv2Offset = cv2Calibration;
	syncWavetable.cv2Offset = cv2Calibration;
	syncWavetable.cv3Offset = cv3Calibration;

}

