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

	int32_t logicAState = 0;
	int32_t auxLogicState = 0;
	int32_t shAState = 0;
	int32_t shBState = 0;
	int32_t ledAState = 0;
	int32_t ledBState = 0;
	int32_t ledCState = 0;
	int32_t ledDState = 0;

	// 2 sets the "GPIO" high, 1 sets it low, 0 is a no-op
    inline int32_t virtualLogicOut(int32_t logicOut, int32_t GPIO, uint32_t reg) {
        uint32_t on = (((GPIO >> (reg + 16))) & 1);
        uint32_t off = (((GPIO >> (reg))) & 1);
        logicOut = logicOut + (on * 2) - off;
        if (logicOut > 1) {
        	return 1;
        } else if (logicOut < 0) {
        	return 0;
        } else {
        	return logicOut;
        }
    }

	void processALogic(void) {

		logicAState = virtualLogicOut(logicAState, GPIOC, 13);
		GPIOC = 0;

	}
	void processAuxLogic(void) {

		auxLogicState = virtualLogicOut(auxLogicState, GPIOA, 12);
		GPIOA = 0;

	}
	void processSH(void) {

		shAState = virtualLogicOut(shAState, GPIOB, 8);
		shBState = virtualLogicOut(shBState, GPIOB, 9);
		GPIOB = 0;

	}
	void processLEDA(void) {

		ledAState = virtualLogicOut(ledAState, GPIOF, 7);
		GPIOF = 0;

	}
	void processLEDB(void) {

		ledBState = virtualLogicOut(ledBState, GPIOC, 14);
		GPIOC = 0;

	}
	void processLEDC(void) {

		ledCState = virtualLogicOut(ledCState, GPIOA, 2);
		GPIOA = 0;

	}
	void processLEDD(void) {

		ledDState = virtualLogicOut(ledDState, GPIOB, 2);
		GPIOB = 0;		

	}

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

	uint32_t measurementTimerCount = 0;
	uint32_t measurementTimerFractional = 0;
	inline void advanceMeasurementTimer(void) {
		measurementTimerCount += 1440;
	}
	inline void resetMeasurementTimer(void) {
		measurementTimerCount = -measurementTimerFractional;
	}
	inline uint32_t readMeasurementTimer(void) {
		return measurementTimerCount + measurementTimerFractional;
	}

	float timer1Count = 0.0f;
	float timer1Prescaler = 0.3515625f;
	float timer1Reload = 65536.0f;
	float timer1Enable = 0.0f;

	inline void advanceTimer1(void) {
		timer1Count += timer1Prescaler * timer1Enable;
		if (timer1Count >= timer1Reload) {
			timer1Count = 0.0f;
			auxTimer1InterruptCallback();
		}
	}
	inline void resetTimer1(void) {
		timer1Count = 0.0f;
	}
	inline float readTimer1(void) {
		return timer1Count;
	}
	inline void enableTimer1(void) {
		timer1Enable = 1.0f;
	}
	inline void disableTimer1(void) {
		timer1Enable = 0.0f;
	}

	float timer2Count = 0.0f;
	float timer2Prescaler = 0.3515625f;
	float timer2Reload = 65536.0f;
	float timer2Enable = 0.0f;

	inline void advanceTimer2(void) {
		timer2Count += timer2Prescaler * timer2Enable;
		if (timer2Count >= timer2Reload) {
			timer2Count = 0.0;
			auxTimer2InterruptCallback();
		}
	}
	inline void resetTimer2(void) {
		timer2Count = 0.0f;
	}
	inline float readTimer2(void) {
		return timer2Count;
	}
	inline void enableTimer2(void) {
		timer2Enable = 1.0f;
	}
	inline void disableTimer2(void) {
		timer2Enable = 0.0f;
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
