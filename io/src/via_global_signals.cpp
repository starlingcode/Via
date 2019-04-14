/** \file via_global_signals.cpp
 * \brief Control update/averaging.
 *
 */

#include "via_global_signals.hpp"

void ViaControls::update(void) {

	/// Implement a running average on the control rate ADC conversions.
	cv1Sum = cv1 + cv1Sum - readLongBuffer(&cv1Buffer, 7);
	knob1Sum = knob1 + knob1Sum - readLongBuffer(&knob1Buffer, 63);
	knob2Sum = knob2 + knob2Sum - readLongBuffer(&knob2Buffer, 63);
	knob3Sum = knob3 + knob3Sum - readLongBuffer(&knob3Buffer, 63);

	/// Write the averaged controls to the output variables.
	cv1Value = cv1Sum >> 3;
	knob1Value = knob1Sum >> 6;
	knob2Value = knob2Sum >> 6;
	knob3Value = knob3Sum >> 6;

	/// Store the newest value in the circular buffer.
	writeLongBuffer(&cv1Buffer, cv1);
	writeLongBuffer(&knob1Buffer, knob1);
	writeLongBuffer(&knob2Buffer, knob2);
	writeLongBuffer(&knob3Buffer, knob3);

}

void ViaControls::updateSlow(void) {

	/// Implement a running average on the control rate ADC conversions.
	cv1Sum = cv1 + cv1Sum - readLongBuffer(&cv1Buffer, 255);
	knob1Sum = knob1 + knob1Sum - readLongBuffer(&knob1Buffer, 255);
	knob2Sum = knob2 + knob2Sum - readLongBuffer(&knob2Buffer, 255);
	knob3Sum = knob3 + knob3Sum - readLongBuffer(&knob3Buffer, 255);

	/// Write the averaged controls to the output variables.
	cv1Value = cv1Sum >> 8;
	knob1Value = knob1Sum >> 8;
	knob2Value = knob2Sum >> 8;
	knob3Value = knob3Sum >> 8;

	/// Store the newest value in the circular buffer.
	writeLongBuffer(&cv1Buffer, cv1);
	writeLongBuffer(&knob1Buffer, knob1);
	writeLongBuffer(&knob2Buffer, knob2);
	writeLongBuffer(&knob3Buffer, knob3);

}

void ViaControls::updateSlowExtra(void) {

	int32_t cv1Average = 0;
	int32_t knob1Average = 0;
	int32_t knob2Average = 0;
	int32_t knob3Average = 0;

#ifdef BUILD_F373
	for (int i = 0; i < 16; i = i + 4) {
		cv1Average += controlRateInputs[i];
		knob1Average += controlRateInputs[i + 2];
		knob2Average += controlRateInputs[i + 3];
		knob3Average += controlRateInputs[i + 1];
	}

	cv1Average = 4095 - (cv1Average >> 2);
	knob1Average = knob1Average >> 2;
	knob2Average = knob2Average >> 2;
	knob3Average = knob3Average >> 2;
#endif
#ifdef BUILD_VIRTUAL
	cv1Average = cv1;
	knob1Average = knob1;
	knob2Average = knob2;
	knob3Average = knob3;
#endif


	/// Implement a running average on the control rate ADC conversions.
	cv1Sum = cv1Average + cv1Sum - readLongBuffer(&cv1Buffer, 255);
	knob1Sum = knob1Average + knob1Sum - readLongBuffer(&knob1Buffer, 255);
	knob2Sum = knob2Average + knob2Sum - readLongBuffer(&knob2Buffer, 255);
	knob3Sum = knob3Average + knob3Sum - readLongBuffer(&knob3Buffer, 255);

	/// Write the averaged controls to the output variables.
	cv1Value = cv1Sum >> 8;
	knob1Value = knob1Sum >> 8;
	knob2Value = knob2Sum >> 8;
	knob3Value = knob3Sum >> 8;

	/// Store the newest value in the circular buffer.
	writeLongBuffer(&cv1Buffer, cv1Average);
	writeLongBuffer(&knob1Buffer, knob1Average);
	writeLongBuffer(&knob2Buffer, knob2Average);
	writeLongBuffer(&knob3Buffer, knob3Average);

}


