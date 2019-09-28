/**
 * \file via_virtual_module.hpp
 *
 *  \brief Generalized module class, needs to be updated.
 */

/// \cond

#ifndef IO_INC_VIA_VIRTUAL_MODULE_HPP_
#define IO_INC_VIA_VIRTUAL_MODULE_HPP_

#ifdef BUILD_VIRTUAL

#include <via_virtual_system.hpp>
#include "via_module_template.hpp"

struct ViaModuleGeneric: ViaModuleTest<ViaModuleGeneric> {

	uint32_t GPIOC = 0;
	uint32_t GPIOA = 0;
	uint32_t GPIOB = 0;
	uint32_t GPIOF = 0;

	uint32_t redLevelOut = 0;
	uint32_t blueLevelOut = 0;
	uint32_t greenLevelOut = 0;

	/// Unused; currently the VCVRack plugin just writes and read values directly into and out of the module stream memory.
	void ioStreamInitAction() {

	}

	void initializeAuxOutputsAction(void) {

		/// Tie the output pointers of the module to GPIO control registers.
		aLogicOutput = &(GPIOC);
		auxLogicOutput = &(GPIOA);
		shAOutput = &(GPIOB);
		shBOutput = &(GPIOB);

		ledAOutput = &(GPIOF);
		ledBOutput = &(GPIOC);
		ledCOutput = &(GPIOA);
		ledDOutput = &(GPIOB);

		redLevel = &redLevelOut;
		blueLevel = &blueLevelOut;
		greenLevel = &greenLevelOut;

	}

	void writeOptionBytesAction(uint16_t bottomHalf, uint16_t topHalf) {



	}

	uint32_t readOptionBytesAction(void) {

		return 0;

	}

	void decodeCalibrationPacketAction(void) {

		cv2Calibration = 0;
		cv3Calibration = 0;
		cv1Calibration = 0;
		dac3Calibration = 0;

	}

	inline void setBlueLEDAlt(uint32_t onOff) {

		blueLevelOut = onOff * 4095;

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

#endif

/// \endcond

#endif /* IO_INC_VIA_VIRTUAL_MODULE_HPP_ */
