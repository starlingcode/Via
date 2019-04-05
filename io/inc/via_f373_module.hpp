/** \file via_f373_module.hpp
 * \brief Hardware module class definition.
 *
 *  Define class and system methods for the hardware implementation of the module using an STM32F373 MCU.
 */

#ifndef INC_VIA_F373_MODULE_HPP_
#define INC_VIA_F373_MODULE_HPP_

#include <via_f373_system.hpp>
#include "dsp.hpp"
#include "via_global_signals.hpp"

/// Hardware module class.
/** Global module class inherited by each module if built as module firmware.*/
class ViaModule {
public:

	//@{
	/// Pointer to GPIO control registers. See setLogicOut() for value information.
	volatile uint32_t * aLogicOutput;
	volatile uint32_t * auxLogicOutput;
	volatile uint32_t * shAOutput;
	volatile uint32_t * shBOutput;
	//@}

	//@{
	/// Pointer to RGB LED timer PWM control registers. Valid range 0 - 4095.
	volatile uint32_t * redLevel;
	volatile uint32_t * greenLevel;
	volatile uint32_t * blueLevel;
	//@}

	//@{
	/// Pointer to the LED control register, see setLEDA() for value information.
	volatile uint32_t * ledAOutput;
	volatile uint32_t * ledBOutput;
	volatile uint32_t * ledCOutput;
	volatile uint32_t * ledDOutput;
	//@}

	//@{
	/// Pointer to touch button state from the touch sense library.
	int32_t * button1Input;
	int32_t * button2Input;
	int32_t * button3Input;
	int32_t * button4Input;
	int32_t * button5Input;
	int32_t * button6Input;
	//@}

	/// Control rate inputs with averaging, see ViaControls for details.
	ViaControls controls;

	/// Length of input buffer.
	/**
	 * To update the first element in the input arrays at 50k (full speed), use 1.
	 * To copy N values at an update frequency of 50000/N, use 2 * N.
	 */
	int32_t inputBufferSize;
	/// Length of output buffer.
	/**
	 * Number of samples per half transfer complete callback.
	 */
	int32_t outputBufferSize;
	/// Audio rate inputs from CV2 and CV3 SDADCs, see ViaInputStreams for details.
	ViaInputStreams inputs;
	/// Audio rate outputs to the three DAC channels, see ViaOutputStreams for details.
	ViaOutputStreams outputs;

	int32_t cv2Calibration = 0;
	int32_t cv3Calibration = 0;
	int32_t cv1Calibration = 0;
	int32_t dac3Calibration = 0;

	int32_t calibrationPacket = 0;

	void encodeCalibrationPacket(uint32_t cv2Calibration, uint32_t cv3Calibration,
			uint32_t cv1Calibration, uint32_t dac3Calibration) {

		// from lsb, first 10 bits are cv2Calibration, next 10 are cv3Calibration, next 5 are cv1Calibration, last 7 are dac3 calibration
		calibrationPacket = (cv2Calibration >> 1) | ((cv3Calibration >> 1) << 9) |
				((cv1Calibration & 0b1111111) << 18) | ((dac3Calibration >> 2) << 25);

	}

	void decodeCalibrationPacket(void) {

		cv2Calibration = (calibrationPacket & 0b111111111) << 1;
		cv3Calibration = ((calibrationPacket >> 9) & 0b111111111) << 1;
		cv1Calibration = (calibrationPacket >> 18) & 0b1111111;
		if (cv1Calibration >> 6) {
			cv1Calibration = -(~cv1Calibration & 0b111111);
		}
		dac3Calibration = ((calibrationPacket >> 25) & 0b1111111);
		dac3Calibration <<= 2;

	}

	void writeOptionBytes(uint16_t bottomHalf, uint16_t topHalf) {

		FLASH_OBProgramInitTypeDef pOBInit;
		HAL_FLASHEx_OBGetConfig(&pOBInit);
		HAL_StatusTypeDef obStatus;

		pOBInit.OptionType = OPTIONBYTE_WRP;
		pOBInit.WRPState = OB_WRPSTATE_DISABLE;

		HAL_FLASH_Unlock();
		HAL_FLASH_OB_Unlock();
		obStatus = HAL_FLASHEx_OBProgram(&pOBInit);

		pOBInit.OptionType = OPTIONBYTE_DATA;
		pOBInit.DATAAddress = OB_DATA_ADDRESS_DATA0;
		pOBInit.DATAData = bottomHalf;

		obStatus = HAL_FLASHEx_OBProgram(&pOBInit);

		pOBInit.DATAAddress = OB_DATA_ADDRESS_DATA1;
		pOBInit.DATAData = topHalf;

		obStatus = HAL_FLASHEx_OBProgram(&pOBInit);
		HAL_FLASH_OB_Launch();
		HAL_FLASH_Lock();


	}

	uint32_t readOptionBytes(void) {

		return HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA0) |
				(HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA1) << 16);

	}

	/// Connect module hardware IO
	/** Connect the virtual module sample data streams to the DMA controller on the hardware. */
	void ioStreamInit() {

		/// Initialize the control rate 12 bit ADC DMA. The buffer length of 16 in conjunction with the ADC sample rate causes it to update at 2k. End of conversion events are serviced by the slowConversionCallback method of each module.
		HAL_ADC_Start_DMA(&hadc1, controls.controlRateInputs, 16);

		/// Start calibration calibration and poll for completion for each sigma delta ADC (CV2 and CV3).
		if (HAL_SDADC_CalibrationStart(&hsdadc1, SDADC_CALIBRATION_SEQ_1)
				!= HAL_OK) {
		}
		if (HAL_SDADC_PollForCalibEvent(&hsdadc1, 1000) != HAL_OK) {
		}

		if (HAL_SDADC_CalibrationStart(&hsdadc2, SDADC_CALIBRATION_SEQ_1)
				!= HAL_OK) {
		}
		if (HAL_SDADC_PollForCalibEvent(&hsdadc2, 1000) != HAL_OK) {
		}

		/// Initialize the DAC DMA with buffer length per half transfer callback set by the outputBufferSize member.
		HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, outputs.dac1Samples,
				2 * outputBufferSize, DAC_ALIGN_12B_R);
		HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, outputs.dac2Samples,
				2 * outputBufferSize, DAC_ALIGN_12B_R);
		HAL_DAC_Start_DMA(&hdac2, DAC_CHANNEL_1, outputs.dac3Samples,
				2 * outputBufferSize, DAC_ALIGN_12B_R);

		/// Initialize the DAC DMA with buffer length per full transfer set by the inputBufferSize member. This does not multiply the buffer size by two so that inputs.cv2Samples[0] updates at 50k. A buffer size of 1 is used for all existing modules as they update the output by 50k.
		HAL_SDADC_Start_DMA(&hsdadc1, (uint32_t *) inputs.cv2Samples, inputBufferSize);
		HAL_SDADC_Start_DMA(&hsdadc2, (uint32_t *) inputs.cv3Samples, inputBufferSize);

		/// Ensure that the sample and hold circuits initialize in tracking state.
		setSH(0, 0);

	}

	/// Initialize GPIO and timer outputs.
	/** Tie the output pointers to the appropriate control registers on the microcontroller. */
	void initializeAuxOutputs(void) {

		/// Tie the output pointers of the module to GPIO control registers.
		aLogicOutput = &(GPIOC->BSRR);
		auxLogicOutput = &(GPIOA->BSRR);
		shAOutput = &(GPIOB->BSRR);
		shBOutput = &(GPIOB->BSRR);

		ledAOutput = &(GPIOF->BSRR);
		ledBOutput = &(GPIOC->BSRR);
		ledCOutput = &(GPIOA->BSRR);
		ledDOutput = &(GPIOB->BSRR);

		/// Tie the PWM control output pointers of the module class to GPIO control registers.
		// tim3 channel 2
		redLevel = &TIM3->CCR1 + 1;
		// tim4 channel 4
		greenLevel = &TIM4->CCR1 + 3;
		// tim5 channel 1
		blueLevel = &TIM5->CCR1;

	}

	/// 16 samples from the hue space as RGB values with 12 bits per color channel.
	rgb hueSpace[16] = {{4095, 0, 0}, {4095, 1228, 0}, {4095, 2457, 0}, {4095, 3685, 0}, {2047, 4095, 0}, {819, 4095, 0}, {0, 4095, 409}, {0, 4095, 1638}, {0, 4095, 4095}, {0, 2866, 4095}, {0, 1638, 4095}, {0, 409, 4095}, {2047, 0, 4095}, {3276, 0, 4095}, {4095, 0, 3685}, {4095, 0, 2456}};


	/*
	 *
	 * Logic output handling
	 *
	 */

	/// Set main logic output with 1, reset with 0. Accepts 0 or 1, other values will not work, for safety pass (value != 0).
	inline void setLogicA(int32_t high) {
		*aLogicOutput = GET_ALOGIC_MASK(high);
	}

	/// Set aux logic output with 1, reset with 0. Accepts 0 or 1, other values will not work, for safety pass (value != 0)
	inline void setAuxLogic(int32_t high) {
		*auxLogicOutput = GET_EXPAND_LOGIC_MASK(high);
	}

	/// Set aux logic output with 1, reset with 0. Accepts 0 or 1, other values will not work, for safety pass (value != 0)
	inline void setSH(int32_t sampleA, int32_t sampleB) {

		uint32_t mask = GET_SH_A_MASK(sampleA);
		mask |= GET_SH_B_MASK(sampleB);

		*shAOutput = mask;

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

	/**
	 * \brief Set logic outputs and corresponding LEDs with disable flag.
	 *
	 * Set all GPIO outputs at once using the logic streams.
	 * This converts 1 = high/sample, 0 = low/track to the appropriate mask
	 * If runtimeDisplay, set LEDs, else if runtimeDisplay == 0, don't set LEDs.
	 */

	inline void setLogicOutBoolean(int32_t writeIndex, int32_t runtimeDisplay) {

		int32_t logicA = GET_ALOGIC_MASK(outputs.logicA[writeIndex]);
		int32_t auxLogic = GET_EXPAND_LOGIC_MASK(outputs.auxLogic[writeIndex]);
		int32_t shA = GET_SH_A_MASK(outputs.shA[writeIndex]);
		int32_t shB = GET_SH_B_MASK(outputs.shB[writeIndex]);

		if (runtimeDisplay) {
			setLogicOutputsLEDOn(logicA, auxLogic, shA, shB);
		} else {
			setLogicOutputsLEDOff(logicA, auxLogic, shA, shB);
		}

	}

	/*
	 *
	 * LED handling
	 *
	 */

	//@{

	/// Set LED on with a non zero value and off with 0.

	inline void setLEDA(int32_t on) {
		*ledAOutput = ((uint32_t) GPIO_PIN_7) << (16 * (!on));
	}

	inline void setLEDB(int32_t on) {
		*ledBOutput = ((uint32_t) GPIO_PIN_14) << (16 * (!on));
	}

	inline void setLEDC(int32_t on) {
		*ledCOutput = ((uint32_t) GPIO_PIN_2) << (16 * (!on));
	}

	inline void setLEDD(int32_t on) {
		*ledDOutput = ((uint32_t) GPIO_PIN_2) << (16 * (!on));
	}

	//@}

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
		setRGB({0, 0, 0});
		SET_BLUE_LED_ONOFF(0);
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



#endif /* INC_VIA_F373_MODULE_HPP_ */
