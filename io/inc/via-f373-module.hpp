/** \file via_f373_module.hpp
 * \brief Hardware module class definition.
 *
 *  Define class and system methods for the hardware implementation of the module using an STM32F373 MCU.
 */

#ifndef INC_VIA_F373_MODULE_HPP_
#define INC_VIA_F373_MODULE_HPP_

#include <via-f373-system.hpp>
#include "via-module-template.hpp"

struct ViaModuleF373: ViaModuleTest<ViaModuleF373> {

	void processALogic(void) {

	}
	void processAuxLogic(void) {

	}
	void processSH(void) {

	}
	void processLEDA(void) {

	}
	void processLEDB(void) {

	}
	void processLEDC(void) {

	}
	void processLEDD(void) {

	}
	void processAllGPIO(void) {
		
	}

	/// Connect module hardware IO
	/** Connect the virtual module sample data streams to the DMA controller on the hardware. */
	void ioStreamInitAction() {

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
	void initializeAuxOutputsAction(void) {

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

	void writeOptionBytesAction(uint16_t bottomHalf, uint16_t topHalf) {

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

		if (obStatus) {

		}

	}

	uint32_t readOptionBytesAction(void) {

		return HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA0) |
				(HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA1) << 16);

	}

	void decodeCalibrationPacketAction(void) {

		cv2Calibration = (calibrationPacket & 0b111111111) << 1;
		cv3Calibration = ((calibrationPacket >> 9) & 0b111111111) << 1;
		cv1Calibration = (calibrationPacket >> 18) & 0b1111111;
		if (cv1Calibration >> 6) {
			cv1Calibration = -(~cv1Calibration & 0b111111);
		}
		dac3Calibration = ((calibrationPacket >> 25) & 0b1111111);
		dac3Calibration <<= 2;

	}

	inline void setBlueLEDAlt(uint32_t onOff) {
		SET_BLUE_LED_ONOFF(onOff);
	}

	/// Handle a rising edge at the main logic input
	virtual void mainRisingEdgeCallback(void) {};
	/// Handle a falling edge at the main logic input
	virtual void mainFallingEdgeCallback(void) {};

	/// Handle a rising edge at the expander logic input
	virtual void auxRisingEdgeCallback(void) {};
	/// Handle a falling edge at the expander logic input
	virtual void auxFallingEdgeCallback(void) {};

	/// Handle a press event on the expander control button
	virtual void buttonPressedCallback(void) {};
	/// Handle a release event on the expander control button
	virtual void buttonReleasedCallback(void) {};

	/// unused
	virtual void ioProcessCallback(void) {};

	/// Fill the first half of the dac buffers
	virtual void halfTransferCallback(void) {};
	/// Fill the second half of the dac buffers
	virtual void transferCompleteCallback(void) {};
	/// Handle an end of conversion event for the 12 bit control rate ADCs
	virtual void slowConversionCallback(void) {};

	/// Handle an overflow interrupt from aux timer 1
	virtual void auxTimer1InterruptCallback(void) {};
	/// Handle an overflow interrupt from aux timer 2
	virtual void auxTimer2InterruptCallback(void) {};
	/// Handle an overflow interrupt from aux timer 3
	virtual void auxTimer3InterruptCallback(void) {};

};




#endif /* INC_VIA_F373_MODULE_HPP_ */
