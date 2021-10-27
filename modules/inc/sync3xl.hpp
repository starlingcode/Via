
#ifndef INC_SYNC3XL_HPP_
#define INC_SYNC3XL_HPP_

#include "sync3.hpp"


class ViaSync3XL : public ViaSync3 {

public:
    ViaSync3XL(std::string binPath) : ViaSync3(binPath) {}

    uint32_t tempPM = 0;

    void setLogic1(uint32_t level) {setLogicA(level);}
    void setLogic2(uint32_t level) {setRedLED(4095 * level);}
    void setLogic3(uint32_t level) {setBlueLED(4095 * level);}
    void setSyncLED(uint32_t level) {setGreenLED(4095 * level);}
    void setPMLED(uint32_t level) {tempPM = level;}

    void displayRatio(void) {setLEDs(sync3UI.button2Mode);}
    void clearRGB(void) {}

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {
		#ifdef BUILD_F373
		uint32_t reading = TIM2->CNT;
		#endif
		#ifdef BUILD_VIRTUAL
		uint32_t reading = readMeasurementTimer();
		#endif

		if (reading < (1440 * 64)) {
			uint32_t valid = (reading > (periodCount >> 8));
			errorPileup += valid;
			advanceSubharm(valid);
		} else {
			#ifdef BUILD_F373
			TIM2->CNT = 0;
			int32_t playbackPosition = (VIA_SYNC3_BUFFER_SIZE * 2) - DMA1_Channel5->CNDTR;
			#endif
			#ifdef BUILD_VIRTUAL
			resetMeasurementTimer();
			int32_t playbackPosition = (reading % 1440)/60;
			#endif

			periodCount = reading;

			advanceSubharm(1);

			error1 = phases2[playbackPosition];
			error2 = phases3[playbackPosition];
			error3 = phases4[playbackPosition];

//			playbackPosition = (playbackPosition > 23) ? playbackPosition - 24 : playbackPosition;
//
//			phaseModTracker = phaseModTracker3 + phaseModIncrement2 * playbackPosition;

			phaseModTracker = phaseModTracker3;

			freqCorrect = 0;

			measurementDivider = errorPileup + 1;

			errorPileup = 0;

			uint32_t ratio1Delta = (key1 != lastKey1);
			uint32_t ratio2Delta = (key2 != lastKey2);
			uint32_t ratio3Delta = (key3 != lastKey3);

			setLogic1(ratio1Delta);
			setLogic2(ratio2Delta);
			setLogic3(ratio3Delta);
		    setSyncLED(1);

			lastKey1 = key1;
			lastKey2 = key2;
			lastKey3 = key3;

		}

		tapTempo = 0;

	}
	void mainFallingEdgeCallback(void) {

		setLogic1(0);
		setLogic2(0);
		setLogic3(0);
		setSyncLED(0);

	}

	void slowConversionCallback(void) {

		controls.updateExtraNoCV1();

		if (ratioRoundRobin == 0) {

			uint32_t thisIndex1 = controls.knob1Value + __USAT(controls.cv1Value - cv1Calibration, 12);
			thisIndex1 = index1Hysterisis(thisIndex1 >> 8, thisIndex1);
			sync1Div = dividedPhases[thisIndex1];
			denominator1Select = denominators[thisIndex1];
			numerator1Alt = numerators[thisIndex1];
			key1 = keys[thisIndex1];

		} else if (ratioRoundRobin == 1) {

			int32_t ratio2Mod = -inputs.cv2Samples[0];
			ratio2Mod += cv2Calibration;
			ratio2Mod >>= 4;
			ratio2Mod += 2048;
			ratio2Mod = __USAT(ratio2Mod, 12);
			uint32_t thisIndex2 = controls.knob2Value;
			thisIndex2 = index2Hysterisis(thisIndex2 >> 8, thisIndex2);
			thisIndex2 += index2CVHysterisis(ratio2Mod >> 8, ratio2Mod);
			sync2Div = dividedPhases[thisIndex2];
			denominator2Select = denominators[thisIndex2];
			numerator2Alt = numerators[thisIndex2];
			key2 = keys[thisIndex2];

		} else if (ratioRoundRobin == 2) {

			int32_t ratio3Mod = phaseModOn ? -inputs.cv2Samples[0] : -inputs.cv3Samples[0];
			ratio3Mod += cv3Calibration;
			ratio3Mod >>= 4;
			ratio3Mod += 2048;
			ratio3Mod = __USAT(ratio3Mod, 12);
			uint32_t thisIndex3 = controls.knob3Value;
			thisIndex3 = index3Hysterisis(thisIndex3 >> 8, thisIndex3);
			thisIndex3 += index3CVHysterisis(ratio3Mod >> 8, ratio3Mod);
			sync3Div = dividedPhases[thisIndex3];
			denominator3Select = denominators[thisIndex3];
			numerator3Alt = numerators[thisIndex3];
			key3 = keys[thisIndex3];

		}

		ratioRoundRobin = (ratioRoundRobin >= 2) ? 0 : ratioRoundRobin + 1;

		#ifdef BUILD_F373

		if (!tapTempo && (TIM2->CNT > periodCount * 4)) {
			tapTempo = 1;
			TIM17->CNT = 0;
			TIM17->CR1 |= TIM_CR1_CEN;
			TIM17->ARR = periodCount >> 12;
			TIM18->CNT = 0;
			TIM18->CR1 |= TIM_CR1_CEN;
			TIM18->ARR = periodCount >> 13;
            setSyncLED(1);
		}

		#endif

		#ifdef BUILD_VIRTUAL

		if (!tapTempo && (readMeasurementTimer() > periodCount * 4)) {
			tapTempo = 1;
			resetTimer1();
			enableTimer1();
			timer1Reload = (float)periodCount/4096;
			resetTimer2();
			enableTimer2();
			timer2Reload = (float)periodCount/8192;
            setSyncLED(1);
		}

		#endif

	}
	void auxTimer1InterruptCallback(void) {

		if (tapTempo) {

			#ifdef BUILD_F373

			int32_t playbackPosition = (VIA_SYNC3_BUFFER_SIZE * 2) - DMA1_Channel5->CNDTR;

			#endif

			#ifdef BUILD_VIRTUAL

			int32_t playbackPosition = 0;

			#endif

			advanceSubharm(1);

			error1 = phases2[playbackPosition];
			error2 = phases3[playbackPosition];
			error3 = phases4[playbackPosition];

			playbackPosition = (playbackPosition > 23) ? playbackPosition - 24 : playbackPosition;

			phaseModTracker = phaseModTracker3 + phaseModIncrement2 * playbackPosition;

			freqCorrect = 0;

			errorPileup = 0;

			uint32_t ratio1Delta = (key1 != lastKey1);
			uint32_t ratio2Delta = (key2 != lastKey2);
			uint32_t ratio3Delta = (key3 != lastKey3);

			setLogic1(ratio1Delta);
			setLogic2(ratio2Delta);
			setLogic3(ratio3Delta);
		    setSyncLED(1);

			lastKey1 = key1;
			lastKey2 = key2;
			lastKey3 = key3;
			#ifdef BUILD_F373
			TIM18->CR1 |= TIM_CR1_CEN;
			#endif
			#ifdef BUILD_VIRTUAL
			enableTimer2();
			#endif

		} else {
			#ifdef BUILD_F373
			TIM17->CR1 &= ~TIM_CR1_CEN;
			#endif
			#ifdef BUILD_VIRTUAL
			disableTimer1();
			#endif
		}

	}
	void auxTimer2InterruptCallback(void) {

		setLogic1(0);
		setLogic2(0);
		setLogic3(0);
		setSyncLED(0);

		#ifdef BUILD_VIRTUAL 
		disableTimer2();
		#endif

	}

    void handleButton4ModeChange(int32_t mode) {

        phaseModOn = mode;
        phaseModTracker2 = 0;
        setPMLED(mode);

    }
};

#endif /* INC_SYNC3XL_HPP_ */
