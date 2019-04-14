/*
 * calibration_helpers.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: willmitchell
 */

#include "calib.hpp"

constexpr int32_t Sine::big_sine[4097];

/// Pass in the position in the output buffer that needs to be filled.
void ViaCalib::renderFixedOutputs(int32_t writePosition) {

	/// Use a downward counting while loop to iterate over the relevant section of the buffer. This allows the loop to branch on a comparison with 0 (cheaper than for loop).
	int32_t samplesRemaining = CALIB_BUFFER_SIZE;

	while (samplesRemaining) {
		/// Write full scale 12 bit (4095) to the dac 1 and dac 2 buffers (the ADC control signals)
		outputs.dac1Samples[writePosition] = 4095;
		outputs.dac2Samples[writePosition] = 4095;
		/// Write half-scale (2048) to set the signal output to virtual ground.
		outputs.dac3Samples[writePosition] = 2048;
		/// Each iteration, decrement the variable counting remaining samples and increment the write index in the buffer.
		samplesRemaining --;
		writePosition ++;
	}

}

/// Iterate over the relevant section of the dac output buffers like in renderFixedOutputs().
void ViaCalib::renderTestOutputs(int32_t writePosition) {

	int32_t sample;

	int32_t samplesRemaining = CALIB_BUFFER_SIZE;

	while (samplesRemaining) {

		/// At each iteration, increment the sine oscillator.
		sinePhase += sineFreq;
		/// Magic number alert, the sine output is 15 bits so we scale down to 12 bits.
		sample = oscillator.evaluate(sinePhase) >> 3;

		/// Write the 12 bit "inverse" (4095 - sample) of the sine sample to the channel A VCA control.
		outputs.dac1Samples[writePosition] = 4095 - sample;
		/// Write the sample to the to the channel A VCA control. This accomplishes the crossfade contour output method.
		outputs.dac2Samples[writePosition] = sample;
		/// Write the dac3Output sample rendered in advanceLFO().
		outputs.dac3Samples[writePosition] = dac3Output;

		samplesRemaining --;
		writePosition ++;
	}

}

/// Handrolled square wave LFO using a phasor and a comparison against half scale.
void ViaCalib::advanceLFO(void) {

	/// Magic number alert, the knob1 average is multiplied by a large number to set the frequency range of the LFO
	lfoIncrement = controls.knob1 << 9;
	/// Magic number alert, the knob 2 average is scaled from 0-4095 to 0-7 by right shifting by 9 (12 bits -> 3 bits)
	dac3Range = controls.knob2 >> 9;
	/// Magic number alert, the knob 2 average is scaled from 0-4095 to 0-7 by right shifting by 9 (12 bits -> 3 bits) and offset by half scale to be bipolar.
	dac3Base = (controls.knob3 >> 9) - 3;

	/// Add the increment (frequency control) to the accumulator. It wraps at full scale 32 bit.
	lfoPhasor += lfoIncrement;

	/// Look at the top bit of the (unsigned) accumulator to determine square wave state. The multiplication sets the state to 0 if knob1 is fully left (the increment variable is 0).
	uint32_t state = (lfoPhasor >> 31) * (lfoIncrement != 0);

	/// Add the LFO output base voltage to state*range to calculate the signal output.
	int32_t octaveOffset = __SSAT(dac3Base + dac3Range*state, 3);

	/// Bias it up from bipolar to unipolar by adding half-scale 12 bits, multiply by the span of one octave (4095/(10.6666666) = 384) to scale the LFO to an octave span.
	dac3Output = 2048 - octaveOffset * 384;

	/// Translate the state of the LFO to a GPIO set/reset instruction to drive the logic outputs and sample and hold controls.
	outputs.logicA[0] = GET_ALOGIC_MASK(state);
	outputs.auxLogic[0] = GET_EXPAND_LOGIC_MASK(state);
	outputs.shA[0] = GET_SH_A_MASK(state);
	outputs.shB[0] = GET_SH_B_MASK(state);

	/// Set the logic outputs with setLogicOut(); we aren't using a buffer of outputs so you can set the first argument to 0, and we aren't using the default logic out LED display so you can set runtimeDisplay to 0.
	setLogicOut(0, 0);

}

void ViaCalib::cv1TunerExecute(void) {

	// when at rest, wait for change
	// when change, wait for settle
	// when settle, wait for average completion
	// when average complete, measure and display, storing average

	// {resting, changeDetected, stable, averaging, measuring}

	int32_t actualCV1Value = 4095 - controls.controlRateInputs[0];
	int32_t lastReading;
	int32_t error;

	switch (tunerState) {
	case resting:
		if (abs(actualCV1Value - (int32_t)controls.cv1Value) > 100) {
			tunerState = changeDetected;
			setLEDA(1);
			setGreenLED(0);
			setRedLED(0);
			setBlueLED(0);
		}
		break;
	case changeDetected:
		if (abs(actualCV1Value - (int32_t)controls.cv1Value) < 5) {
			tunerState = averaging;
		}
		break;
	case averaging:
		if (extraCV1Counter < 2048) {
			extraCV1Sum += actualCV1Value;
			extraCV1Counter += 1;
		} else {
			extraCV1Counter = 0;
			tunerState = measuring;
		}
		break;
	case measuring:
		lastReading = (extraCV1Sum >> 11);

		if (baseCV1 < lastReading) {
			/// Use the current average value to measure the size of the jump module an ideal octave (384)
			error = abs(baseCV1 - lastReading) % 384;
			/// Blank LED A indicating a measurement has been made.
			setLEDA(0);
			/// If the error is 0, turn on the green LED, otherwise show an undershoot on the blue LED and an overshoot on the red LED.
			/// The blue and red error readings get slightly dimmer as the get smaller, with a min value added so even small error is immediately apparent.
			if (error == 0) {
				setGreenLED(1024);
				setRedLED(0);
				setBlueLED(0);
			} else if (error > 256) {
				setBlueLED(((384 - error) << 3) + 300);
				setRedLED(0);
				setGreenLED(0);
			} else if (error < 256) {
				setBlueLED(0);
				setRedLED((error << 3) + 300);
				setGreenLED(0);
			}
		} else {
			setGreenLED(0);
			setRedLED(0);
			setBlueLED(0);
		}
		extraCV1Sum = 0;
		baseCV1 = lastReading;
		tunerState = resting;
		break;
	default:
		break;
	}



//	/// Implement an extra running average of length 256
//	extraCV1Sum += controls.cv1Value - readLongBuffer(&extraCV1Buffer, 255);
//	writeLongBuffer(&extraCV1Buffer, controls.cv1Value);
//	int32_t longerAverage = extraCV1Sum >> 8;
//
//	/// Measure the deviation in the unaveraged CV from the average value.
//	int32_t cv1Change = (int32_t)((4095 - controls.controlRateInputs[0]) - longerAverage);
//
//	/// If the CV1 stable flag is not set
//	if (!cv1Stable) {
//
//		/// Take the difference in the average from the last sample.
//		int32_t averageValueDifferential = longerAverage - lastLongerAverage;
//
//		/// If its 0, the average is stable.
//		if ((averageValueDifferential == 0)) {
//			/// Set the stable flag.
//			cv1Stable = 1;
//			/// Use the current average value to measure the size of the jump module an ideal octave (384)
//			int32_t error = abs(baseCV1 - longerAverage) % 384;
//			/// Blank LED A indicating a measurement has been made.
//			setLEDA(0);
//			/// If the error is 0, turn on the green LED, otherwise show an undershoot on the blue LED and an overshoot on the red LED.
//			/// The blue and red error readings get slightly dimmer as the get smaller, with a min value added so even small error is immediately apparent.
//			if (error == 0) {
//				setGreenLED(1024);
//				setRedLED(0);
//				setBlueLED(0);
//			} else if (error > 256) {
//				setBlueLED(((384 - error) << 3) + 300);
//				setRedLED(0);
//				setGreenLED(0);
//			} else if (error < 256) {
//				setBlueLED(0);
//				setRedLED((error << 3) + 300);
//				setGreenLED(0);
//			}
//		}
//	/// If the stable flag is high, look for a CV deviation greater than 300.
//	} else if ((abs(cv1Change) > 300)) {
//		/// Set LED A indicating that the CV is unstable and a reading will be made when it stabilizes.
//		setLEDA(1);
//		/// Set the stable flag low.
//		cv1Stable = 0;
//		/// Store the last average before the change to measure step size.
//		baseCV1 = lastLongerAverage;
//	}
//	/// Store the current average to use on the next tuner execution call.
//	lastLongerAverage = longerAverage;

}

void ViaCalib::measureCVOffsets(void) {

	/// Accumulate (one accumulator per CV) the first 1024 samples from start of calibration
	if (cvCalibCounter < 1024) {
		cv1Sum += controls.cv1Value;
		int32_t cv2Sample = inputs.cv2Samples[0];
		cv2Sum += cv2Sample;
		int32_t cv3Sample = inputs.cv3Samples[0];
		cv3Sum += cv3Sample;
		cvCalibCounter ++;
	} else if (cvCalibCounter == 1024) {
		/// Then divide the accumulated value by 64 to get an average and store the averages.
		cv1Read = (cv1Sum >> 10) - 2048;
		cv2Read = cv2Sum >> 10;
		cv3Read = cv3Sum >> 10;
		cvCalibCounter ++;
	} else {
		/// After storing, turn on the green LED.
		setGreenLED(4095);

	}
}

void ViaCalib::measureDAC3Offset(void) {

	/// Accumulate (one accumulator per CV) the first 1024 samples from start of calibration
	if (cvCalibCounter < 1024) {
		cv1Sum += controls.cv1Value;
		cvCalibCounter ++;
	} else if (cvCalibCounter == 1024) {
		/// Measure the DAC offset by compensating for the CV offset we just measured and then measuring deviation from the ideal 2048.
		dac3Offset = 2048 + (int32_t) cv1Read - (int32_t) controls.cv1Value;
		/// Encode the offsets into a 32 bit word for storage.
		encodeCalibrationPacket(cv2Read, cv3Read, cv1Read, dac3Offset);
		/// Set the white LEDs to indicate stage completion.
		setLEDA(1);
		setLEDB(1);
		setLEDC(1);
		setLEDD(1);
		cvCalibCounter ++;
	}

}

void ViaCalib::verifyCV2CV3(void) {

	/// Heads up! When reading from the CV2 and CV3 DMA array, the first thing you should do is cast to 32 bit int and invert.
	/// The DMA stream encodes the ADC reading as a 16 bit integer which gets mangled without type casting, input circuit inverts the value.
	int32_t cv2Sample = -inputs.cv2Samples[0];
	int32_t cv3Sample = inputs.cv3Samples[0];

	/// Check if the CVs are significantly above ground. If they are, turn the red LED on for CV2 and the blue LED on for CV3, if they go back to ground, turn the LEDs off.
	if (cv2Sample > 10000) {
		setRedLED(2048);
		cv2High = 1;
	} else if (cv2High != 0) {
		cv2High = 0;
		setRedLED(0);
	}
	if (cv3Sample > 10000) {
		setBlueLED(2048);
		cv3High = 1;
	} else if (cv3High != 0) {
		cv3High = 0;
		setBlueLED(0);
	}

}

