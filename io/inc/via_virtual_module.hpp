/**
 * \file via_virtual_module.hpp
 *
 *  \brief Generalized module class, needs to be updated.
 */

/// \cond

#ifndef IO_INC_VIA_VIRTUAL_MODULE_HPP_
#define IO_INC_VIA_VIRTUAL_MODULE_HPP_

#ifdef BUILD_VIRTUAL

#include "dsp.hpp"
#include "via_virtual_system.hpp"
#include "via_global_signals.hpp"

/// Virtual module class.
/** Global module class inherited by each module if built as a Rack plugin or other virtual implementation.*/
class ViaModule {
public:

	//@{
	/// Variable to represent the state of the virtual GPIO outputs. See setLogicOut() for value information.
	uint32_t aLogicOutput = 0;
	uint32_t auxLogicOutput = 0;
	uint32_t shAOutput = 0;
	uint32_t shBOutput = 0;
	//@}

	//@{
	/// Variable to represent the brightness of each of the RGB LEDs. Valid range 0 - 4095.
	uint32_t redLevel = 0;
	uint32_t greenLevel = 0;
	uint32_t blueLevel = 0;
	//}

	//@{
	/// Variable to represent the LED state, see setLEDA() for value information.
	uint32_t ledAOutput = 0;
	uint32_t ledBOutput = 0;
	uint32_t ledCOutput = 0;
	uint32_t ledDOutput = 0;
	//@}

	//@{
	/// Variable to represent the state of each touch button.
	int32_t button1Input = 0;
	int32_t button2Input = 0;
	int32_t button3Input = 0;
	int32_t button4Input = 0;
	int32_t button5Input = 0;
	int32_t button6Input = 0;
	//@}

	/// Control rate inputs with averaging, see ViaControls for details.
	ViaControls controls;

	/// Length of input buffer.
	/**
	 * Match to the size of the stream host.
	 */
	int32_t inputBufferSize;
	/// Length of input buffer.
	/**
	 * Match to the size of the stream host.
	 */
	int32_t outputBufferSize;
	/// Audio rate inputs from CV2 and CV3 SDADCs, see ViaInputStreams for details.
	ViaInputStreams inputs;
	/// Audio rate outputs to the three DAC channels, see ViaOutputStreams for details.
	ViaOutputStreams outputs;

	/// Unused; currently the VCVRack plugin just writes and read values directly into and out of the module stream memory.
	void ioStreamInit() {

	}

	/// Unused; currently the VCVRack plugin just reads values from the virtual GPIO output variables.
	void initializeAuxOutputs(void) {


	}

	/// 16 samples from the hue space as RGB values with 12 bits per color channel.
	rgb hueSpace[16] = {{4095, 0, 0}, {4095, 1228, 0}, {4095, 2457, 0}, {4095, 3685, 0}, {2047, 4095, 0}, {819, 4095, 0}, {0, 4095, 409}, {0, 4095, 1638}, {0, 4095, 4095}, {0, 2866, 4095}, {0, 1638, 4095}, {0, 409, 4095}, {2047, 0, 4095}, {3276, 0, 4095}, {4095, 0, 3685}, {4095, 0, 2456}};


	/*
	 *
	 * Logic output handling
	 *
	 */

	/// Needs to be rewritten
	inline void setLogicA(int32_t high) {
		aLogicOutput = 1 + high;
	}
	/// Needs to be rewritten
	inline void setAuxLogic(int32_t high) {
		auxLogicOutput = 1 + high;
	}
	/// Needs to be rewritten
	inline void setSH(int32_t sampleA, int32_t sampleB) {

		shAOutput = 1 + sampleA;
		shBOutput = 1 + sampleB;

	}
	/// Needs to be rewritten
	inline void setLEDA(int32_t on) {
		ledAOutput = 1 + on;
	}
	/// Needs to be rewritten
	inline void setLEDB(int32_t on) {
		ledBOutput = 1 + on;
	}
	/// Needs to be rewritten
	inline void setLEDC(int32_t on) {
		ledCOutput = 1 + on;
	}
	/// Needs to be rewritten
	inline void setLEDD(int32_t on) {
		ledDOutput = 1 + on;
	}
	/// Needs to be rewritten
	void SET_BLUE_LED_ONOFF(int32_t X) {
		blueLevel = X * 4095;
	} 
	/// Needs to be rewritten
	enum ViaVirtualGPIO {
		VIA_GPIO_NOP,
		VIA_GPIO_LOW,
		VIA_GPIO_HIGH
	};
	/// Needs to be rewritten
	inline void setLogicOutputsLEDOn(uint32_t logicA, uint32_t auxLogic,
			uint32_t shA, uint32_t shB) {

		logicA = GET_ALOGIC_VIRTUAL_MASK(logicA);
		auxLogic = GET_EXPAND_LOGIC_VIRTUAL_MASK(auxLogic);
		shA = GET_SH_A_VIRTUAL_MASK(shA);
		shB = GET_SH_B_VIRTUAL_MASK(shB);

		setLEDA(shA - 1);
		setLEDB(shB - 1);
		setLEDC(logicA - 1);

		aLogicOutput = (logicA);

		auxLogicOutput = (auxLogic);

		shAOutput = (shA);

		shBOutput = (shB);

	}
	/// Needs to be rewritten
	inline void setLogicOutputsLEDOff(uint32_t logicA, uint32_t auxLogic,
			uint32_t shA, uint32_t shB) {

		logicA = GET_ALOGIC_VIRTUAL_MASK(logicA);
		auxLogic = GET_EXPAND_LOGIC_VIRTUAL_MASK(auxLogic);
		shA = GET_SH_A_VIRTUAL_MASK(shA);
		shB = GET_SH_B_VIRTUAL_MASK(shB);

		//combine the mask variables for a shared GPIO group with a bitwise or
		aLogicOutput = (logicA);

		auxLogicOutput = (auxLogic);

		shAOutput = (shA);

		shBOutput = (shB);

	}
	/// Needs to be rewritten
	inline void setLogicOut(int32_t writeIndex, int32_t runtimeDisplay) {

		int32_t logicA = outputs.logicA[writeIndex];
		int32_t auxLogic = outputs.auxLogic[writeIndex];
		int32_t shA = outputs.shA[writeIndex];
		int32_t shB = outputs.shB[writeIndex];

		if (runtimeDisplay) {
			setLogicOutputsLEDOn(logicA, auxLogic, shA, shB);
		} else {
			setLogicOutputsLEDOff(logicA, auxLogic, shA, shB);
		}

	}
	/// Needs to be rewritten
	inline void setLogicOutNoLED(int32_t writeIndex) {

		setLogicOutputsLEDOff(outputs.logicA[writeIndex], outputs.auxLogic[writeIndex], outputs.shA[writeIndex], outputs.shB[writeIndex]);

	}

	int32_t cv2Calibration = 0;
	int32_t cv3Calibration = 0;
	int32_t cv1Calibration = 0;
	int32_t dac3Calibration = 0;

	int32_t calibrationPacket = 0;

	void decodeCalibrationPacket(void) {

		cv2Calibration = 0;
		cv3Calibration = 0;
		cv1Calibration = 0;
		dac3Calibration = 0;

	}

	uint32_t readOptionBytes(void) {

		return 0;

	}

	void writeOptionBytes(uint16_t bottomHalf, uint16_t topHalf) {
		
	}

	/// Needs to be rewritten
//	inline void setLogicOutBoolean(int32_t writeIndex, int32_t runtimeDisplay) {
//
//		int32_t logicA = GET_ALOGIC_MASK(outputs.logicA[writeIndex]);
//		int32_t auxLogic = GET_EXPAND_LOGIC_MASK(outputs.auxLogic[writeIndex]);
//		int32_t shA = GET_SH_A_MASK(outputs.shA[writeIndex]);
//		int32_t shB = GET_SH_B_MASK(outputs.shB[writeIndex]);
//
//		if (runtimeDisplay) {
//			setLogicOutputsLEDOn(logicA, auxLogic, shA, shB);
//		} else {
//			setLogicOutputsLEDOff(logicA, auxLogic, shA, shB);
//		}
//
//	}

	/*
	 *
	 * LED handling
	 *
	 */

	/// Update red LED, 4095 fully on, 0 off.
	inline void setRedLED(int32_t level) {
		redLevel = level;
	}
	/// Update green LED, 4095 fully on, 0 off.
	inline void setGreenLED(int32_t level) {
		greenLevel = level;
	}
	/// Update blue LED, 4095 fully on, 0 off.
	inline void setBlueLED(int32_t level) {
		blueLevel = level;
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
		setRedLED((color.r * scale) >> 12);
		setGreenLED((color.g * scale) >> 12);
		setBlueLED((color.b * scale) >> 12);
	}

	/// Set RGB LED to 0.
	void clearRGB() {
		setRedLED(0);
		setGreenLED(0);
		setBlueLED(0);
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

#endif

/// \endcond

#endif /* IO_INC_VIA_VIRTUAL_MODULE_HPP_ */
