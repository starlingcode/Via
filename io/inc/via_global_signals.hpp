/** \file via_global_signals.hpp
 * \brief Virtual signal definitions.
 *
 *  Container structs and methods to implement signal I/O streams.
 */
#ifndef INC_VIA_GLOBAL_SIGNALS_HPP_
#define INC_VIA_GLOBAL_SIGNALS_HPP_

#include <stdlib.h>
#include "dsp.hpp"

/// Control rate inputs with built in averaging method. TODO add linear interpolation to higher sample rate.

class ViaControls {

	//@{
	/// Accumulators used for boxcar averaging.
	uint32_t knob1Sum = 0;
	uint32_t knob2Sum = 0;
	uint32_t knob3Sum = 0;
	uint32_t cv1Sum = 0;
	//@}

	//@{
	/// Circular buffers used for boxcar averaging.
	longBuffer knob1Buffer;
	longBuffer knob2Buffer;
	longBuffer knob3Buffer;
	longBuffer cv1Buffer;
	//@}

//@{
/// Define the location of the latest conversion value in the DMA array for each control, CV1 is inverted at the input stage
#define knob2 controlRateInputs[3]
#define knob3 controlRateInputs[1]
#define knob1 controlRateInputs[2]
#define cv1 (4095 - controlRateInputs[0])
//@}

public:

	/// Clean out bunk values in the circular buffer memory on construction.
	ViaControls() {

		for (int32_t i = 0; i < 256; i++) {
			writeLongBuffer(&knob1Buffer, 0);
			writeLongBuffer(&knob2Buffer, 0);
			writeLongBuffer(&knob3Buffer, 0);
			writeLongBuffer(&cv1Buffer, 0);
		}

	}

	/// Storage buffer for DMA aquisition containing raw values ordered according to conversion, best to use macros such as \ref knob1.
	uint32_t controlRateInputs[16];

	//@{
	/// Latest averaged value, the one to be referenced by external code.
	uint32_t knob1Value = 0;
	uint32_t knob2Value = 0;
	uint32_t knob3Value = 0;
	uint32_t cv1Value = 0;
	//@}

	/// Update the average for each ADC with the latest value. TODO add methods to update values independently.
	void update(void);
	void updateSlow(void);
	void updateSlowExtra(void);

};


/// Allocation and initialization for the input stream memory.
class ViaInputStreams {

public:
	//@{
	/// Memory address used for DMA transfer. Note the 16 bit signed int type. This is due to the SDADC conversion. Be sure to cast to int32_t before using.
	int16_t * cv2Samples;
	int16_t * cv3Samples;
	//@}
	//@{
	/// Value of the CV when no signal connected. Store an offset value for the SDADC.
	int16_t * cv2VirtualGround;
	int16_t * cv3VirtualGround;
	//@}
	//@{
	/// Memory address for storage in case the state of the logic inputs.
	/** Write a 0 to the variable on rising edge handler for the logic input.
	 * Read the variable at a sample rate interrupt and store it in the sample buffer.
	 * No modules use a buffer because the DAC is updated at 50k so there is no need to store samples.
	 * Instead, they just process the latest input value then set it to 1.
	 */
	int32_t * trigSamples;
	int32_t trigInput;
	int32_t * auxTrigSamples;
	int32_t auxTrigInput;
	//@}

	/// Allocate memory acccording to the input buffer size
	int32_t bufferSize;

	/// This should be a parameterized constructor, it allocates memory, cleans it, and initializes the trig variables to 1.
	void init(int32_t size) {

		bufferSize = size;

		trigInput = 1;
		auxTrigInput = 1;

		cv2Samples = (int16_t*) malloc(2 * bufferSize * sizeof(int32_t));
		cv3Samples = (int16_t*) malloc(2 * bufferSize * sizeof(int32_t));
		cv2VirtualGround = (int16_t*) malloc(2 * bufferSize * sizeof(int32_t));
		cv3VirtualGround = (int16_t*) malloc(2 * bufferSize * sizeof(int32_t));

		trigSamples = (int32_t*) malloc(2 * bufferSize * sizeof(int32_t));
		auxTrigSamples = (int32_t*) malloc(2 * bufferSize * sizeof(int32_t));

		for (int32_t i = 0; i < bufferSize; i++) {

			cv2Samples[i] = 0;
			cv3Samples[i] = 0;
			cv2VirtualGround[i] = 0;
			cv3VirtualGround[i] = 0;

			trigSamples[i] = 0;
			auxTrigSamples[i] = 0;

		}

	}

};

/// Allocation and initialization for the output stream memory.
class ViaOutputStreams {
public:
	//@{
	/// Memory address used for DMA transfer. Data is stored as 32 bit word, but range is 0-4095.
	uint32_t * dac1Samples;
	uint32_t * dac2Samples;
	uint32_t * dac3Samples;
	//@}
	//@{
	/// Memory address used to store a buffer of GPIO output.
	/* Data stored as masks (see ViaModule::setLogicOut()). All current modules use a buffer of length 1 because the GPIO and synthesis methods are updated at 50k. */
	uint32_t * shA;
	uint32_t * shB;
	uint32_t * logicA;
	uint32_t * auxLogic;
	//@}

	/// Number of samples per half transfer callback.
	int32_t bufferSize;

	/// This should be a parameterized constructor, it allocates memory and cleans it.
	void init(int32_t size) {

		bufferSize = size;

		dac1Samples = (uint32_t*) malloc(2 * bufferSize * sizeof(int32_t));
		dac2Samples = (uint32_t*) malloc(2 * bufferSize * sizeof(int32_t));
		dac3Samples = (uint32_t*) malloc(2 * bufferSize * sizeof(int32_t));

		shA = (uint32_t*) malloc(2 * bufferSize * sizeof(int32_t));
		shB = (uint32_t*) malloc(2 * bufferSize * sizeof(int32_t));
		logicA = (uint32_t*) malloc(2 * bufferSize * sizeof(int32_t));
		auxLogic = (uint32_t*) malloc(2 * bufferSize * sizeof(int32_t));


		for (int32_t i = 0; i < bufferSize; i++) {
			dac1Samples[i] = 0;
			dac2Samples[i] = 0;
			dac3Samples[i] = 0;

			shA[i] = 0;
			shB[i] = 0;
			logicA[i] = 0;
			auxLogic[i] = 0;
		}

	}

};

/// Helper struct to hold RBB values, implemented with 12 bits per color channel.
typedef struct {
	/// Red channel.
	int32_t r;
	/// Green channel.
	int32_t g;
	/// Blue channel.
	int32_t b;
} rgb;


#endif /* INC_VIA_GLOBAL_SIGNALS_HPP_ */
