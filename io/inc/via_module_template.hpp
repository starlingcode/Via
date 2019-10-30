#include "dsp.hpp"
#include "via_global_signals.hpp"

template <typename Target>
struct ViaModuleTest {

public:

	ViaControls controls;

	int32_t inputBufferSize;
	ViaInputStreams inputs;

	int32_t outputBufferSize;
	ViaOutputStreams outputs;

	inline int32_t getCV1(void) {
		return __USAT(controls.cv1Value - cv1Calibration, 12);
	}

	inline int32_t getCV2(void) {
		int32_t sample = inputs.cv2Samples[0];
		return -sample + cv2Calibration;
	}

	inline int32_t getCV3(void) {
		int32_t sample = inputs.cv2Samples[0];
		return -sample + cv2Calibration;
	}

	inline int32_t getKnob1(void) {
		return controls.knob1Value;
	}

	inline int32_t getKnob2(void) {
		return controls.knob1Value;
	}

	inline int32_t getKnob3(void) {
		return controls.knob1Value;
	}

	inline void writeALevel(uint32_t value, uint32_t index) {
		outputs.dac1Samples[index] = value;
	}

	inline void writeBLevel(uint32_t value, uint32_t index) {
		outputs.dac2Samples[index] = value;
	}

	inline void writeAuxLevel(uint32_t value, uint32_t index) {
		outputs.dac2Samples[index] = 4095 - value;
	}

	inline void writeAuxLevelCalibrated(uint32_t value, uint32_t index) {
		outputs.dac3Samples[index] = __USAT((4095 - value) + dac3Calibration, 12);
	}

	void ioStreamInit(void) {

		static_cast<Target*>(this)->ioStreamInitAction();
		
	} 

	int32_t cv2Calibration = 0;
	int32_t cv3Calibration = 0;
	int32_t cv1Calibration = 0;
	int32_t dac3Calibration = 0;

	int32_t calibrationPacket = 0;

	void decodeCalibrationPacket(void) {

		static_cast<Target*>(this)->decodeCalibrationPacketAction();

	}

	void writeOptionBytes(uint16_t firstByte, uint16_t secondByte) {

		static_cast<Target*>(this)->writeOptionBytesAction(firstByte, secondByte);
		
	}

	uint32_t readOptionBytes(void) {

		return static_cast<Target*>(this)->readOptionBytesAction();
		
	}

	volatile uint32_t * aLogicOutput;
	volatile uint32_t * auxLogicOutput;
	volatile uint32_t * shAOutput;
	volatile uint32_t * shBOutput;

	volatile uint32_t * redLevel;
	volatile uint32_t * greenLevel;
	volatile uint32_t * blueLevel;

	volatile uint32_t * ledAOutput;
	volatile uint32_t * ledBOutput;
	volatile uint32_t * ledCOutput;
	volatile uint32_t * ledDOutput;

	void initializeAuxOutputs(void) {

		static_cast<Target*>(this)->initializeAuxOutputsAction();
		
	}

	int32_t button1Input;
	int32_t button2Input;
	int32_t button3Input;
	int32_t button4Input;
	int32_t button5Input;
	int32_t button6Input;

	/// Set main logic output with 1, reset with 0. Accepts 0 or 1, other values will not work, for safety pass (value != 0).
	inline void setLogicA(int32_t high) {
		*aLogicOutput = GET_ALOGIC_MASK(high);
		static_cast<Target*>(this)->processALogic();
	}

	/// Set aux logic output with 1, reset with 0. Accepts 0 or 1, other values will not work, for safety pass (value != 0)
	inline void setAuxLogic(int32_t high) {
		*auxLogicOutput = GET_EXPAND_LOGIC_MASK(high);
		static_cast<Target*>(this)->processAuxLogic();
	}

	/// Set aux logic output with 1, reset with 0. Accepts 0 or 1, other values will not work, for safety pass (value != 0)
	inline void setSH(int32_t sampleA, int32_t sampleB) {

		uint32_t mask = GET_SH_A_MASK(sampleA);
		mask |= GET_SH_B_MASK(sampleB);

		*shAOutput = mask;

		static_cast<Target*>(this)->processSH();

	}

	/**
	 * \brief Set all GPIO outputs at once and set adjacent LEDs.
	 *
	 * This expects GPIO masks as arguments for the logic control.
	 * To convert from 0 as off and 1 as on to masks use GET_ALOGIC_MASK, GET_EXPAND_LOGIC_MASK, GET_SH_A_MASK, and GET_SH_B_MASK.
	 * Set matching LEDs (LED A with SH A, LED C (needs name change) with SH B, LED B (name change) with main logic out.
	 */
	inline void setLogicOutputsLEDOn(uint32_t logicA, uint32_t auxLogic,
			uint32_t shA, uint32_t shB) {

		// LEDA_HIGH_MASK -> SH_A_SAMPLE_MASK >> 16 >> 1 (pin 8 to pin 7, F)
		// LEDB_HIGH_MASK -> SH_B_SAMPLE_MASK >> 16 << 5 (pin 9 to pin 14, C)
		// LEDC_HIGH_MASK -> ALOGIC_HIGH_MASK >> 16 >> 11 (pin 13 to pin 2, A)
		// LEDD_HIGH_MASK -> BLOGIC_HIGH_MASK >> 16 >> 13 (pin 15 to pin 2, B)

	#define LEDA_MASK (__ROR(shA, 16) >> 1)
	#define LEDB_MASK (__ROR(shB, 16) << 5)
	#define LEDC_MASK (__ROR(logicA, 16) >> 11)

		//combine the mask variables for a shared GPIO group with a bitwise or
		*aLogicOutput = (logicA | LEDB_MASK);

		*auxLogicOutput = (auxLogic | LEDC_MASK);

		*shAOutput = (shA | shB);

		*ledAOutput = LEDA_MASK;

		static_cast<Target*>(this)->processAllGPIO();

	}

	inline void setLogicOutputsLEDOnNoA(uint32_t logicA, uint32_t auxLogic,
			uint32_t shA, uint32_t shB) {

		// LEDA_HIGH_MASK -> SH_A_SAMPLE_MASK >> 16 >> 1 (pin 8 to pin 7, F)
		// LEDB_HIGH_MASK -> SH_B_SAMPLE_MASK >> 16 << 5 (pin 9 to pin 14, C)
		// LEDC_HIGH_MASK -> ALOGIC_HIGH_MASK >> 16 >> 11 (pin 13 to pin 2, A)
		// LEDD_HIGH_MASK -> BLOGIC_HIGH_MASK >> 16 >> 13 (pin 15 to pin 2, B)

	#define LEDA_MASK (__ROR(shA, 16) >> 1)
	#define LEDB_MASK (__ROR(shB, 16) << 5)
	#define LEDC_MASK (__ROR(logicA, 16) >> 11)

		//combine the mask variables for a shared GPIO group with a bitwise or
		*aLogicOutput = (logicA | LEDB_MASK);

		*auxLogicOutput = (auxLogic | LEDC_MASK);

		*shAOutput = (shA | shB);

		static_cast<Target*>(this)->processAllGPIO();

	}

	/**
	 * \brief Set all GPIO outputs at once.
	 *
	 * This expects GPIO masks as arguments for the logic control.
	 * To convert from 0 as off and 1 as on to masks use GET_ALOGIC_MASK, GET_EXPAND_LOGIC_MASK, GET_SH_A_MASK, and GET_SH_B_MASK.
	 * Don't set LEDs.
	 */
	inline void setLogicOutputsLEDOff(uint32_t logicA, uint32_t auxLogic,
			uint32_t shA, uint32_t shB) {

		//combine the mask variables for a shared GPIO group with a bitwise or
		*aLogicOutput = (logicA);

		*auxLogicOutput = (auxLogic);

		*shAOutput = (shA | shB);

		static_cast<Target*>(this)->processALogic();
		static_cast<Target*>(this)->processAuxLogic();
		static_cast<Target*>(this)->processSH();

	}

	/**
	 * \brief Set all GPIO outputs at once using the logic streams with flag to disable.
	 *
	 * This expects GPIO masks as arguments for the logic control.
	 * To convert from 0 as off and 1 as on to masks use GET_ALOGIC_MASK, GET_EXPAND_LOGIC_MASK, GET_SH_A_MASK, and GET_SH_B_MASK.
	 * setLogicOutBoolean includes this step.
	 * If runtimeDisplay, set LEDs, else if runtimeDisplay == 0, don't set LEDs.
	 */
	inline void setLogicOut(int32_t writeIndex, int32_t runtimeDisplay) {

		if (runtimeDisplay) {
			setLogicOutputsLEDOn(outputs.logicA[writeIndex], outputs.auxLogic[writeIndex], outputs.shA[writeIndex], outputs.shB[writeIndex]);
		} else {
			setLogicOutputsLEDOff(outputs.logicA[writeIndex], outputs.auxLogic[writeIndex], outputs.shA[writeIndex], outputs.shB[writeIndex]);
		}

	}

	/**
	 *
	 * \brief Same as setLogicOut, but never set the LEDs.
	 */

	inline void setLogicOutNoLED(int32_t writeIndex) {

		setLogicOutputsLEDOff(outputs.logicA[writeIndex], outputs.auxLogic[writeIndex], outputs.shA[writeIndex], outputs.shB[writeIndex]);

	}

	inline void setLogicOutNoA(int32_t writeIndex, int32_t runtimeDisplay) {

		if (runtimeDisplay) {
			setLogicOutputsLEDOnNoA(outputs.logicA[writeIndex], outputs.auxLogic[writeIndex], outputs.shA[writeIndex], outputs.shB[writeIndex]);
		} else {
			setLogicOutputsLEDOff(outputs.logicA[writeIndex], outputs.auxLogic[writeIndex], outputs.shA[writeIndex], outputs.shB[writeIndex]);
		}

	}


	/// Update red LED, 4095 fully on, 0 off.
	inline void setRedLED(int32_t level) {
		*redLevel = level;
	}
	/// Update green LED, 4095 fully on, 0 off.
	inline void setGreenLED(int32_t level) {
		*greenLevel = level;
	}
	/// Update blue LED, 4095 fully on, 0 off.
	inline void setBlueLED(int32_t level) {
		*blueLevel = level;
	}

	/// Update rbg with values 4095 fully on, 0 off. Only update if runtimeDisplay != 0.
	inline void updateRGBDisplay(int32_t red, int32_t green, int32_t blue, int32_t runtimeDisplay) {
		if (runtimeDisplay) {
			setRedLED(red);
			setGreenLED(green);
			setBlueLED(blue);
		}
	}

	/// Update rbg with values 4095 fully on, 0 off. No disable variable.
	void setRGB(rgb color) {
		setRedLED(color.r);
		setGreenLED(color.g);
		setBlueLED(color.b);
	}

	/// Update rbg with values 4095 fully on, 0 off. Scale each value with a 16 bit int, 65535 fully on, 0 off.
	void setRGBScaled(rgb color, int32_t scale) {
		setRedLED((color.r * scale) >> 16);
		setGreenLED((color.g * scale) >> 16);
		setBlueLED((color.b * scale) >> 16);
	}

		/// Set RGB LED to 0.
	void clearRGB() {
		static_cast<Target*>(this)->setBlueLEDAlt(0);
		setRGB({0, 0, 0});
	}

		// Used for preset behavior
	void updateRGBPreset(int32_t uiTimer, int32_t presetNumber) {

		int32_t hue = presetNumber;

		int32_t fade = __USAT((7000 - uiTimer), 12);

		if (hue) {
			updateRGBDisplay((fade * presetHues[hue - 1].r) >> 12,
					(fade * presetHues[hue - 1].g) >> 12,
					(fade * presetHues[hue - 1].b) >> 12,
					1);
		}

	}

		/// 16 samples from the hue space as RGB values with 12 bits per color channel.
	rgb hueSpace[16] = {{4095, 0, 0}, {4095, 1228, 0}, {4095, 2457, 0}, {4095, 3685, 0}, {2047, 4095, 0}, {819, 4095, 0}, {0, 4095, 409}, {0, 4095, 1638}, {0, 4095, 4095}, {0, 2866, 4095}, {0, 1638, 4095}, {0, 409, 4095}, {2047, 0, 4095}, {3276, 0, 4095}, {4095, 0, 3685}, {4095, 0, 2456}};
	rgb presetHues[6] = {{4095, 0, 0}, {0, 4095, 0}, {0, 0, 4095}, {2048, 2048, 0}, {0, 2048, 2048}, {2048, 0, 2048}};


	/// Set LED on with a non zero value and off with 0.

	inline void setLEDA(int32_t on) {
		*ledAOutput = ((uint32_t) GPIO_PIN_7) << (16 * (!on));
		static_cast<Target*>(this)->processLEDA();
	}

	inline void setLEDB(int32_t on) {
		*ledBOutput = ((uint32_t) GPIO_PIN_14) << (16 * (!on));
		static_cast<Target*>(this)->processLEDB();
	}

	inline void setLEDC(int32_t on) {
		*ledCOutput = ((uint32_t) GPIO_PIN_2) << (16 * (!on));
		static_cast<Target*>(this)->processLEDC();
	}

	inline void setLEDD(int32_t on) {
		*ledDOutput = ((uint32_t) GPIO_PIN_2) << (16 * (!on));
		static_cast<Target*>(this)->processLEDD();
	}

	/// Set an enumerated pattern for 8 digits on the white LEDs. The LEDs increment clockwise from top left, with one LED on for 1-4 and 2 LEDs on for 5-8.
	void setLEDs(int32_t digit) {
		switch (digit) {
		case 0:
			setLEDA(1);
			setLEDB(0);
			setLEDC(0);
			setLEDD(0);
			break;
		case 1:
			setLEDA(0);
			setLEDB(0);
			setLEDC(1);
			setLEDD(0);
			break;
		case 2:
			setLEDA(0);
			setLEDB(0);
			setLEDC(0);
			setLEDD(1);
			break;
		case 3:
			setLEDA(0);
			setLEDB(1);
			setLEDC(0);
			setLEDD(0);
			break;
		case 4:
			setLEDA(1);
			setLEDB(0);
			setLEDC(1);
			setLEDD(0);
			break;
		case 5:
			setLEDA(0);
			setLEDB(0);
			setLEDC(1);
			setLEDD(1);
			break;
		case 6:
			setLEDA(0);
			setLEDB(1);
			setLEDC(0);
			setLEDD(1);
			break;
		case 7:
			setLEDA(1);
			setLEDB(1);
			setLEDC(0);
			setLEDD(0);
			break;
		}
	}

	/// Turn the white LEDs off.
	void clearLEDs() {
		setLEDA(0);
		setLEDB(0);
		setLEDC(0);
		setLEDD(0);
	}

};

