#ifndef INC_Calib_HPP_
#define INC_Calib_HPP_

#include "user_interface.hpp"
#include <via_platform_binding.hpp>
#include <dsp.hpp>

/// Macro used to specify the number of samples to per DAC transfer.
#define SINEBEAT_BUFFER_SIZE 1

/// Callback to link to the C code in the STM32 Touch Sense Library.
void sinebeatTouchLink (void *);

/// From https://gist.github.com/Skoddiethecat/174d4d9d35ea1a80f94da899a2c2075d
class ViaSinebeat : public TARGET_VIA {

public:

	/// Calibration UI implementation.
	/**
	 * Override virtual methods in the UI drivers to implement the touch interface.
	 * I believe this should use template meta-programming instead of virtual functions because specific functionality is determined at compile time.
	 * One C++ trick at a time for now.
	 **/

	class ViaSinebeatUI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		ViaSinebeat& this_module;


		//@{
		/// Functions to handle a tap event on a touch sensor or shift combo.
		void button1TapCallback(void) override;
		void button2TapCallback(void) override;
		void button3TapCallback(void) override;
		void button4TapCallback(void) override;
		void button5TapCallback(void) override;
		void button6TapCallback(void) override;

		void aux1TapCallback(void) override;
		void aux2TapCallback(void) override;
		void aux2AltTapCallback(void) override;
		void aux3TapCallback(void) override;
		void aux4TapCallback(void) override;
		//@}

		//@{
		/// Functions to handle a hold event on a touch sensor or shift combo.
		void button1HoldCallback(void) override;
		void button2HoldCallback(void) override;
		void button3HoldCallback(void) override;
		void button4HoldCallback(void) override;
		void button5HoldCallback(void) override;
		void button6HoldCallback(void) override;

		void aux1HoldCallback(void) override;
		void aux2HoldCallback(void) override;
		void aux2AltHoldCallback(void) override;
		void aux3HoldCallback(void) override;
		void aux4HoldCallback(void) override;
		//@}

		//@{
		/// A method to access the LED control functions of this_module in the UI driver.
		void uiSetLEDs(int) override;
		//@}

		//@{
		/// A method to recall the module's last state at power on or restore presets.
		void recallModuleState(void) override;
		//@}

		//@{
		/// Methods to handle entry transitions into the main module states.
		void defaultEnterMenuCallback(void) override;
		void newModeEnterMenuCallback(void) override;
		void newAuxModeEnterMenuCallback(void) override;
		void presetEnterMenuCallback(void) override;
		//@}

		//@{
		/// Methods to handle entry transitions into touch sensor menu states.
		void button1EnterMenuCallback(void) override;
		void button2EnterMenuCallback(void) override;
		void button3EnterMenuCallback(void) override;
		void button4EnterMenuCallback(void) override;
		void button5EnterMenuCallback(void) override;
		void button6EnterMenuCallback(void) override;

		void aux1EnterMenuCallback(void) override;
		void aux2EnterMenuCallback(void) override;
		void aux2AltEnterMenuCallback(void) override;
		void aux3EnterMenuCallback(void) override;
		void aux4EnterMenuCallback(void) override;
		//@}

		/// A method to handle any initialization that needs to be done after some or all of the outer class constructor.
		void initialize(void) override;

		/// A utility method to write the factory presets from memory.
		void writeStockPresets(void) override {}

		void blinkOnCallback(void) override {
			restoreRed = *(this_module.redLevel);
			restoreGreen = *(this_module.greenLevel);
			restoreBlue = *(this_module.blueLevel);
			this_module.updateRGBDisplay(4095, 4095, 4095, 1);
		}

		void blinkOffCallback(void) override {
			this_module.updateRGBDisplay(restoreRed, restoreGreen,
					restoreBlue, 1);
		}

		/// On construction, link the calibTouchLink callback to the STM32 touch sense library.
		ViaSinebeatUI(ViaSinebeat& x): this_module(x) {
			linkUI((void *) &sinebeatTouchLink, (void *) this);
		}

	};

	/// An instance of the UI implementation.
	ViaSinebeatUI sinebeatUI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the touch sense library.
	/// Called in the executable code in user_interrupts.c.
	/// For some reason I have it wrapped like this?
	/// Perhaps the C code in the hardware executable mangled the namespace? Gotta check.
	void ui_dispatch(int32_t sig) {
		this->sinebeatUI.dispatch(sig);
	};


	/*
	 *
	 * State functions
	 *
	 */

	/// Instance of the exponential converter class.
	ExpoConverter expo;

#define SINE_TO_SIGNED16BIT(X) ((X - 16383) << 1)

	class SineBeat {
	public:
		Sine oscillator1;
		Sine oscillator2;
		Sine oscillator3;
		int32_t outerFM;
		uint32_t t;
		virtual uint32_t execute(void) {
			return 0;
		}
		inline void advance(int32_t increment) {
			t += increment;
		}
	};

	SineBeat * sineBeat;

	class SineBeat1: public SineBeat {
	public:
		uint32_t execute(void) override {
			int32_t modulator = ((t&48>>t%13)&(((t%64==0)?t>>2:t<<2)>>((t%64)/16))&((t%6==0)?(37&t>>11):(11&t<<37))|(t&t>>5)&(t*3<<4)&((t&2048)?((t>>11)/4):(t>>7))-2);
			outerFM = modulator;
			return oscillator1.evaluateSineBeat(modulator);
		}
	};
	SineBeat1 sineBeat1;

	class SineBeat2: public SineBeat {
	public:
		uint32_t execute(void) override {
			oscillator2.freq = oscillator1.freq >> 1;
			oscillator3.freq = oscillator1.freq >> 2;
			int32_t modulator = (((((t%256)?(t>>4):(t<<4))-7))/2)|((t>>63)|(((9-(1^4&(t>>11)))|(t&t%((t%1024==0)?1023:1024))-256|(t&(t-16)>>23)))|((((t%4096==0)?t>>5:t<<3)>>((t%256)/32)|(t*3>>13))/1024)/1);
			outerFM = modulator;
			modulator = oscillator3.evaluateSineBeat(modulator);
			modulator = oscillator2.evaluateSineBeat(modulator);
			return oscillator1.evaluateSineBeat(modulator);
		}
	};
	SineBeat2 sineBeat2;

	class SineBeat3: public SineBeat {
	public:
		uint32_t execute(void) override {
			oscillator2.freq = oscillator1.freq >> 1;
			oscillator3.freq = oscillator1.freq >> 2;
			int32_t modulator = ((t%511)^(t&t%255)|(t&t%511)|(t&t%1023)|(t|t%2047));
			modulator = oscillator3.evaluateSineBeat(modulator);
			modulator = oscillator2.evaluateSineBeat(modulator);
			outerFM = modulator;
			return oscillator1.evaluateSineBeat(outerFM);
		}
	};
	SineBeat3 sineBeat3;

	class SineBeat4: public SineBeat {
	public:
		inline int32_t phaseindex4drums(unsigned long t){
			int iDrums;
			if (t<(2*1024*1024)){iDrums = 3;}
			else if (t<(4*1024*1024)){iDrums = 6;}
			else if (t<(6*1024*1024)){iDrums = 5;}
			else if (t<(8*1024*1024)){iDrums = 9;}
			else if (t<(10*1024*1024)){iDrums = 8;}
			else if (t<(12*1024*1024)){iDrums = 7;}
			else if (t<(14*1024*1024)){iDrums = 4;}
			else {iDrums = 10;}
			return t>>iDrums;
		}

		uint32_t execute(void) override {
			oscillator2.freq = oscillator1.freq >> 1;
			oscillator3.freq = oscillator1.freq >> 2;
			int32_t modulator = ((phaseindex4drums(t)|(t>>(4-(1^7&(t>>14))))|(9015+(t%8192)?((t*9)%4096):t/2)/2));
			outerFM = modulator;
			modulator = oscillator3.evaluateSineBeat(modulator);
			modulator = oscillator2.evaluateSineBeat(modulator);
			return oscillator1.evaluateSineBeat(modulator);
		}

	};
	SineBeat4 sineBeat4;

	void handleSineBeatChange(int32_t mode) {
		if (mode == 0) {
			sineBeat = &sineBeat1;
			sineBeat->t = 0;
		} else if (mode == 1) {
			sineBeat = &sineBeat2;
			sineBeat->t = 0;
		} else if (mode == 2) {
			sineBeat = &sineBeat3;
			sineBeat->t = 0;
		} else if (mode == 3) {
			sineBeat = &sineBeat4;
			sineBeat->t = 0;
		}
	}

	int32_t * sharedTable;

	// this is goofy, the oscillator class should have a "load prediff fuction" and the pointer should be shared by other instances of the class
	// i think this is maybe how the static keyword works
	void initializeOscs(void) {
		sharedTable = (int32_t *) malloc(4095*sizeof(int32_t));
		for (int i; i < 4096; i++) {
			sharedTable[i] = sineBeat2.oscillator1.big_sine[i] | ((sineBeat2.oscillator1.big_sine[i + 1] - sineBeat2.oscillator1.big_sine[i]) << 16);
		}
		sineBeat1.oscillator1.tableRead = sharedTable;
		sineBeat1.oscillator2.tableRead = sharedTable;
		sineBeat1.oscillator3.tableRead = sharedTable;
		sineBeat2.oscillator1.tableRead = sharedTable;
		sineBeat2.oscillator2.tableRead = sharedTable;
		sineBeat2.oscillator3.tableRead = sharedTable;
		sineBeat3.oscillator1.tableRead = sharedTable;
		sineBeat3.oscillator2.tableRead = sharedTable;
		sineBeat3.oscillator3.tableRead = sharedTable;
		sineBeat4.oscillator1.tableRead = sharedTable;
		sineBeat4.oscillator2.tableRead = sharedTable;
		sineBeat4.oscillator3.tableRead = sharedTable;
	}

	/// Fill the dac buffers with fixed outputs.
	void render(int32_t writePosition) {
		int32_t incrementMod = inputs.cv3Samples[0];
		incrementMod >>= 4;
		int32_t outputSample = sineBeat->execute();
		outputSample <<= 4;
		sineBeat->advance(__USAT(incrementMod + controls.knob3, 12));
		outputs.dac1Samples[writePosition] = 4095 - outputSample;
		outputs.dac2Samples[writePosition] = outputSample;
		outputs.dac3Samples[writePosition] = sineBeat->outerFM << 4;
		outputs.logicA[0] = GET_ALOGIC_MASK((sineBeat->t & 0x4000) != 0);
	}

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) {
		sineBeat->t = 0;
	}
	void mainFallingEdgeCallback(void) {
	}
	void auxRisingEdgeCallback(void) {

	}
	void auxFallingEdgeCallback(void) {
	}
	void buttonPressedCallback(void) {
	}
	void buttonReleasedCallback(void) {}
	void ioProcessCallback(void) {}
	void halfTransferCallback(void) {
		setLogicOut(0, runtimeDisplay);
		render(0);
	}
	void transferCompleteCallback(void) {
		setLogicOut(0, runtimeDisplay);
		render(SINEBEAT_BUFFER_SIZE);
	}
	void slowConversionCallback(void) {
		controls.update();
		sineBeat->oscillator1.freq = fix16_mul(expo.convert((controls.knob1Value * 3) >> 2) >> 1,
				expo.convert(controls.cv1Value) >> 2);
		if (runtimeDisplay) {
			setBlueLED(4095);
		}
		int32_t srMod = inputs.cv2Samples[0];
		#ifdef BUILD_F373
		TIM6->ARR = 1440 + controls.knob2Value + (srMod >> 4) + 2048;
		#endif
	}
	void auxTimer1InterruptCallback(void) {

	}
	void auxTimer2InterruptCallback(void) {

	}

	/// On construction, call subclass constructors and pass each a pointer to the module class.
	ViaSinebeat() : sinebeatUI(*this) {

		/// Link the module GPIO registers.
		initializeAuxOutputs();

		/// Initialize the input stream buffers.
		inputs.init(SINEBEAT_BUFFER_SIZE);
		/// Initialize the output stream buffers.
		outputs.init(SINEBEAT_BUFFER_SIZE);
		/// Set the data members that will be used to determine DMA stream initialization in the hardware executable.
		outputBufferSize = SINEBEAT_BUFFER_SIZE;
		inputBufferSize = 1;

		initializeOscs();
		sineBeat = &sineBeat1;

		/// Call the UI initialization that needs to happen after outer class construction.
		sinebeatUI.initialize();


	}

};

#endif /* INC_Calib_HPP_ */
