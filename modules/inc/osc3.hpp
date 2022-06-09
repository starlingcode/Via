/** \file calib.hpp
 * \brief Calibration module implementation.
 *
 *  A module implementing a simple testing/calibration helper program that serves as a template.
 */

/// Macro used to specify the number of samples to per DAC transfer.
#define OSC3_BUFFER_SIZE 32

/// Callback to link to the C code in the STM32 Touch Sense Library.
void osc3TouchLink (void *);

#ifndef INC_Calib_HPP_
#define INC_Calib_HPP_

#include "user-interface.hpp"
#include <via-platform-binding.hpp>
#include <dsp.hpp>
#ifdef BUILD_VIRTUAL
#include <stdlib.h>
#include <stdio.h>
#include <string>
#endif

/// Calibration/template module class.

/** A simple self calibration tool that doubles as an introductory template.*/
class ViaOsc3 : public TARGET_VIA {

public:

	/// Calibration UI implementation.
	/**
	 * Override virtual methods in the UI drivers to implement the touch interface.
	 * I believe this should use template meta-programming instead of virtual functions because specific functionality is determined at compile time.
	 * One C++ trick at a time for now.
	 **/

	class ViaOsc3UI: public ViaUI {

	public:

		/**
		 * Pointer to the outer class to allow access to data and methods.
		 * See constructor and outer class constructor for details.
		 */
		ViaOsc3& this_module;


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

		/// On construction, link the calibTouchLink callback to the STM32 touch sense library.
		ViaOsc3UI(ViaOsc3& x): this_module(x) {
			linkUI((void *) &osc3TouchLink, (void *) this);
		}

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

		//@{
		/// Some firmware-specific data members used to register touch events on each sensor for testing.
		int32_t touch1OK = 0;
		int32_t touch2OK = 0;
		int32_t touch3OK = 0;
		int32_t touch4OK = 0;
		int32_t touch5OK = 0;
		int32_t touch6OK = 0;
		///

	};

	/// An instance of the UI implementation.
	ViaOsc3UI osc3UI;

	/// A member that the UI implementation can use to turn the module's runtime display off.
	int32_t runtimeDisplay = 1;

	/// A utility method to pass UI interaction events to the touch sense library.
	/// Called in the executable code in user_interrupts.c.
	/// For some reason I have it wrapped like this?
	/// Perhaps the C code in the hardware executable mangled the namespace? Gotta check.
	void ui_dispatch(int32_t sig) {
		this->osc3UI.dispatch(sig);
	};


	/*
	 *
	 * State functions
	 *
	 */

	void (ViaOsc3::*render)(int32_t writePosition);

	void renderSaw(int32_t writePosition);
	void renderSquare(int32_t writePosition);
	void renderTri(int32_t writePosition);
	void renderTrap(int32_t writePosition);

	int32_t detuneBase = 0;
	int32_t detune = 0;
	int32_t pm = 0;
	int32_t pmTracker = 0;
	int32_t aFreq = 0;
	int32_t bFreq = 0;
	int32_t cFreq = 0;
	uint32_t aPhase = 0;
	uint32_t bPhase = 0;
	uint32_t cPhase = 0;
	int32_t aBasePitch = 0;
	int32_t bBasePitch = 0;
	int32_t cBasePitch = 0;
	int32_t chordTranspose = 0;

	#ifdef BUILD_F373

	static const int32_t absoluteTune = 43900;

	#endif

	#ifdef BUILD_VIRTUAL

	int32_t absoluteTune = 45729;

	#endif

	int32_t octave = 0;
	int32_t octaveRange = 0;
	int32_t octaveMult = 1;
	int32_t unity = 0;

	int32_t beatTimer = 1;
	int32_t clockedBeat = 1;
	int32_t beatPhaseLock = 0;
	int32_t pllPileup = 0;

	int32_t lastLogicA = 0;
	int32_t lastLogicB = 0;
	int32_t lastPM = 0;

	int32_t shAOn = 1;
	int32_t shBOn = 1;

	int32_t lastOffset = 0;
	int32_t lastRoot = 0;
	int32_t lastChord = 0;
	int32_t noteChange = 1;
	int32_t noteChangeCounter = 0;

	int32_t beat = 1;

	int32_t chordStable = 0;
	int32_t chordTransitionPoint = 0;
	int32_t lastStableChord = 0;

	int32_t chordHysterisis(int32_t chord, int32_t control) {

		if (chordStable) {
			chordStable = ((chord - lastStableChord) == 0);
			chordTransitionPoint = (chord > lastStableChord) ? control & 0xF000 : lastStableChord << 12;
			lastStableChord = chord;
			return chord;
		} else {
			chordStable = abs(control - chordTransitionPoint) > 1024;
			lastStableChord = chordStable ? chord : lastStableChord;
			return lastStableChord;
		}

	}

	int32_t rootStable = 0;
	int32_t rootTransitionPoint = 0;
	int32_t lastStableRoot = 0;

	int32_t rootHysterisis(int32_t root, int32_t control) {

		if (rootStable) {
			rootStable = ((root - lastStableRoot) == 0);
			rootTransitionPoint = (root > lastStableRoot) ? control & 0b1111111111100000 : lastStableRoot << 5;
			lastStableRoot = root;
			return root;
		} else {
			rootStable = abs(control - rootTransitionPoint) > 8;
			lastStableRoot = rootStable ? root : lastStableRoot;
			return lastStableRoot;
		}

	}


	int32_t vOctStableSemi = 0;
	int32_t vOctTransitionPointSemi = 0;
	int32_t lastStableVOctSemi = 0;

	int32_t vOctHysterisisSemi(int32_t offset, int32_t control) {

		if (vOctStableSemi) {
			vOctStableSemi = ((offset - lastStableVOctSemi) == 0);
			vOctTransitionPointSemi = (offset > lastStableVOctSemi) ? control & 0b1111111111100000 : lastStableVOctSemi << 5;
			lastStableVOctSemi = offset;
			return offset;
		} else {
			vOctStableSemi = abs(control - vOctTransitionPointSemi) > 8;
			lastStableVOctSemi = vOctStableSemi ? offset : lastStableVOctSemi;
			return lastStableVOctSemi;
		}

	}

	int32_t vOctStable = 0;
	int32_t vOctTransitionPoint = 0;
	int32_t lastStableVOct = 0;

	int32_t vOctHysterisisScale(int32_t offset, int32_t control) {

		if (vOctStable) {
			vOctStable = ((offset - lastStableVOct) == 0);
			vOctTransitionPoint = (offset > lastStableVOct) ? control & 0b1111111111110000 : lastStableVOct << 4;
			lastStableVOct = offset;
			return offset;
		} else {
			vOctStable = abs(control - vOctTransitionPoint) > 8;
			lastStableVOct = vOctStable ? offset : lastStableVOct;
			return lastStableVOct;
		}

	}

// TODO: see if code can read from flash in a timely fashion or does this need to load flash into ram buffers?

// A scale is some subset of the 12TET pitch classes
    struct Osc3Scale {
        int32_t notes[128];
        int32_t intervals[37];
        int32_t degrees[12];
        int32_t chords[34];
    };

#ifdef BUILD_F373

    const struct Osc3Scale * scales = (const struct Osc3Scale *) 0x8020000;

	const int32_t * scale = scales[0].notes;
	const int32_t * intervals = scales[0].intervals;
	const int32_t * chords = scales[0].chords;
	const int32_t * scaleDegrees = scales[0].degrees;

#endif
#ifdef BUILD_VIRTUAL
    struct Osc3Scale * scales;
    void readScalesFromFile(std::string path) {
        FILE * scaleFile = fopen(path.c_str(), "r");
        if (scaleFile == NULL) {
            return; // TODO: Error handling for file not exist or something
        }
        // words in struct * 4 bytes per word * 3 structs in array
        fread(scales, 4 * (128 + 37 + 34 + 12), 3, scaleFile);
        fclose(scaleFile);
    }

	int32_t * scale = scales[0].notes;
	int32_t * intervals = scales[0].intervals;
	int32_t * chords = scales[0].chords;
	int32_t * scaleDegrees = scales[0].degrees;
#endif

//	int32_t chromatic[128] = {0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123};
//	int32_t major[128] = {0, 0, 0, 0, 0, 0, 2, 2, 4, 5, 5, 7, 7, 9, 9, 11, 12, 12, 14, 14, 16, 17, 17, 19, 19, 21, 21, 23, 24, 24, 26, 26, 28, 29, 29, 31, 31, 33, 33, 35, 36, 36, 38, 38, 40, 41, 41, 43, 43, 45, 45, 47, 48, 48, 50, 50, 52, 53, 53, 55, 55, 57, 57, 59, 60, 60, 62, 62, 64, 65, 65, 67, 67, 69, 69, 71, 72, 72, 74, 74, 76, 77, 77, 79, 79, 81, 81, 83, 84, 84, 86, 86, 88, 89, 89, 91, 91, 93, 93, 95, 96, 96, 98, 98, 100, 101, 101, 103, 103, 105, 105, 107, 108, 108, 110, 110, 112, 113, 113, 115, 115, 117, 117, 119, 120, 120, 122, 123};
//	int32_t minor[128] = {0, 0, 0, 0, 0, 0, 2, 3, 3, 5, 5, 7, 8, 8, 10, 10, 12, 12, 14, 15, 15, 17, 17, 19, 20, 20, 22, 22, 24, 24, 26, 27, 27, 29, 29, 31, 32, 32, 34, 34, 36, 36, 38, 39, 39, 41, 41, 43, 44, 44, 46, 46, 48, 48, 50, 51, 51, 53, 53, 55, 56, 56, 58, 58, 60, 60, 62, 63, 63, 65, 65, 67, 68, 68, 70, 70, 72, 72, 74, 75, 75, 77, 77, 79, 80, 80, 82, 82, 84, 84, 86, 87, 87, 89, 89, 91, 92, 92, 94, 94, 96, 96, 98, 99, 99, 101, 101, 103, 104, 104, 106, 106, 108, 108, 110, 111, 111, 113, 113, 115, 116, 116, 118, 118, 120, 120, 122, 123};
//
//    // Array size is (max scale size) + (max pitch class) + (max scale size) + 1 = 12 + 11 + 12 + 1
//	int32_t minorIntervals[36] = {0, 0, 0, 0, 0, -12, -10, -9, -7, -5, -4, -2, 0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24};
//	int32_t majorIntervals[36] = {0, 0, 0, 0, 0, -12, -10, -8, -7, -5, -3, -1, 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24};
//	int32_t scaleDegrees[12] = {0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6};
//
//	int32_t chords[17][2] = {
//		{0, 0}, {-1, 0}, {-1, 1},{-1, 2}, {-2, 2}, {-3, 2}, {-3, 3}, {-2, 3},
//          {-4, 2}, {-5, 4}, {-5, 5}, {-4, 5}, {-6, 5}, {-6, 6}, {-6, 7}, {-7, 7}, {-7, 7}
//	};


	int32_t scaleMode = 0;
	int32_t chordMode = 0;

	void (ViaOsc3::*updateBaseFreqs)(void) = &ViaOsc3::updateBaseFreqsSmooth;

	void updateBaseFreqsScale(void) {

		int32_t root;
		int32_t offset;

        // Read knob to number in range [0, 48] effectively quantizing to semitones
		int32_t knob1Index = (controls.knob1Value * 3) >> 3;
		root = knob1Index >> 5;
		root = rootHysterisis(root, knob1Index);
		
        // Get CV
		int32_t cv1Index = __USAT(controls.cv1Value - cv1Calibration, 12);
		int32_t offsetIndex = vOctHysterisisScale(cv1Index >> 4, cv1Index);
		offset = scale[offsetIndex >> 1];
        
        // Round to quantized pitch
        int32_t next = scale[__USAT((offsetIndex >> 1) + 1, 7)];
		int32_t distance = offset - next;
		int32_t remainder = offsetIndex & 1;
		if (distance == 2) {
			offset = next; 
		} else if (remainder) {
			offset = next;
		} // else truncate

		if (!chordMode) {

            // This ends up specifying a phase increment (frequency) that is
            // The root frequency selected by the coarse knob
            // Transposed by the quantized CV value
			cBasePitch = fix16_mul(expo.convert(root << 5) >> 3,
					expo.convert(offset << 5) >> 2);
            // Correct to an absolute pitch based on sample rate
			cBasePitch = fix16_mul(cBasePitch, absoluteTune);
            // Detune from fine tune knob
			cBasePitch = fix16_mul(cBasePitch, 65535 + (controls.knob2Value << 3));
			detuneBase = clockedBeat + (controls.knob3Value << 4);
            // Calculate pitch change trigger
			if ((root != lastRoot) || (offset != lastOffset)) {
				noteChange = 1;
			} else {
				noteChange = 0;
			}

		} else { // Chord mode

            // Find the specified center frequency similar to the non-chord section			
			int32_t fineTune = 65535 + (controls.knob2Value << 3);
			int32_t coarseTune = expo.convert(root << 5) >> 3;
			coarseTune = fix16_mul(coarseTune, absoluteTune);
			coarseTune = fix16_mul(coarseTune, fineTune);
			cBasePitch = fix16_mul(coarseTune,
					expo.convert(offset << 5) >> 2);

            // CV transpose amount reduced to the 12 semitones
			int32_t pitchClass = offset % 12;
            // Map that pitch class to the corresponding degree in the scale 
			int32_t scaleDegree = scaleDegrees[pitchClass];
            // Get a value that sets an octave amount for the CV offset
            // Think like "integer part" with the decimal as the mod 12 instead of mod 10
			int32_t cvOctave = offset - pitchClass;

            // Chord knob and CV to number [0, 15]
			int32_t chord = __USAT(((controls.knob3Value << 4)) + (int32_t) -inputs.cv3Samples[0], 16);
			chord = chordHysterisis(chord >> 12, chord);
			
            // Do the weird transpose thing to keep chords from being voiced at ugly low pitches
			int32_t chordTranspose = 0;
			int32_t fundamentalPitch = root + 12 * octaveRange;
			if (fundamentalPitch < 36) {
				chordTranspose = 1 + abs((fundamentalPitch - 36)/12);
			}

            // This is the most cursed expression
            // 4 + cvOctave: generates an integer that will give us the root in the scale array
            // intervals[14 + scaleDegree + chords[chord][1]]: 
			int32_t chordMultiplier = scale[__USAT(4 + cvOctave + intervals[12 + scaleDegree + chords[chord*2 + 1]], 7)] << 5;
			aBasePitch = fix16_mul(coarseTune, expo.convert(chordMultiplier) >> 2) << chordTranspose;

			chordMultiplier = scale[__USAT(4 + cvOctave + intervals[12 + scaleDegree + chords[chord*2]], 7)] << 5;
			bBasePitch = fix16_mul(coarseTune, expo.convert(chordMultiplier) >> 2) << chordTranspose;

			detuneBase = 0;

            // Calcuate pitch change trigger
			if ((root != lastRoot) || (offset != lastOffset) || (chord != lastChord)) {
				noteChange = 1;
			} else {
				noteChange = 0;
			}
			lastChord = chord;

		}

		lastOffset = offset;
		lastRoot = root;

	}

	void updateBaseFreqsSmooth(void) {

		int32_t cv1Index = __USAT(controls.cv1Value - cv1Calibration, 12);
		// coarse range in octaves is 4
		int32_t knob1Index = (controls.knob1Value * 3) >> 3;

		int32_t root;
		int32_t offset;

		root = knob1Index;
		offset = __USAT((cv1Index - (4 << 5)), 12);

		if (chordMode) {

			int32_t fineTune = 65535 + (controls.knob2Value << 3);
			int32_t coarseTune = expo.convert(root) >> 3;

			int32_t chord = __USAT(((controls.knob3Value << 4)) + (int32_t) -inputs.cv3Samples[0], 16);
			int32_t chordFrac = chord & 0xFFF;
			chord >>= 12;

			cBasePitch = fix16_mul(coarseTune,
					expo.convert(offset) >> 2);
			cBasePitch = fix16_mul(cBasePitch, absoluteTune);
			cBasePitch = fix16_mul(cBasePitch, fineTune);

			int32_t chordMultiplier = scale[64 + intervals[14 + chords[chord*2 + 1]]] << 5;
			int32_t chordMultiplier1 = scale[64 + intervals[14 + chords[(chord + 1)*2 + 1]]] << 5;
			chordMultiplier = chordMultiplier + (((chordMultiplier1 - chordMultiplier) * chordFrac) >> 12);

			aBasePitch = fix16_mul(cBasePitch, expo.convert(chordMultiplier) >> 5);

			chordMultiplier = scale[64 + intervals[12 + chords[chord*2]]] << 5;
			chordMultiplier1 = scale[64 + intervals[12 + chords[(chord + 1)*2]]] << 5;
			chordMultiplier = chordMultiplier + (((chordMultiplier1 - chordMultiplier) * chordFrac) >> 12);

			bBasePitch = fix16_mul(cBasePitch, expo.convert(chordMultiplier) >> 5);

			detuneBase = 0;

		} else {
			cBasePitch = fix16_mul(expo.convert(knob1Index) >> 3,
					expo.convert(offset) >> 2);
			cBasePitch = fix16_mul(cBasePitch, absoluteTune);
			cBasePitch = fix16_mul(cBasePitch, 65535 + (controls.knob2Value << 3));
			detuneBase = clockedBeat + (controls.knob3Value << 4);
		}

		noteChangeCounter ++;

		if (noteChangeCounter > 16) {

			if (abs(offset - lastOffset) > 32) {
				noteChange = 1;
			} else {
				noteChange = 0;
			}

			lastOffset = offset;

			noteChangeCounter = 0;

		}

	}

	inline void beatDetect(uint32_t aPhase, uint32_t bPhase) {
		int32_t aQuadrant = aPhase >> 30;
		int32_t bQuadrant = (bPhase + (1 << 30)) >> 30;
		if (beat) {
			beat = (abs(aQuadrant - bQuadrant) != 2);
		} else {
			beat = (aQuadrant == bQuadrant);
		}
	}

	void linearDetune(int32_t detuneMod) {

		detune = (detuneBase + (detuneMod << 3)) * (detuneBase != 0);

		aFreq = (cBasePitch * octaveMult) + (detune >> 2) * !unity;
		bFreq = (cBasePitch * octaveMult) - (detune >> 2) * !unity;

	}

	void scaledDetune(int32_t detuneMod) {

		detune = (detuneBase + (detuneMod << 3)) * (detuneBase != 0);

		aFreq = fix16_mul(cBasePitch * octaveMult, 65536 - (detune >> 5) * !unity);
		bFreq = fix16_mul(cBasePitch * octaveMult, 65536 + (detune >> 5) * !unity);

	}

	void chordalDetune(int32_t detuneMod) {

		aFreq = (!unity) ? aBasePitch << (octaveRange - octave): cBasePitch * octaveMult << chordTranspose;
		bFreq = (!unity) ? bBasePitch << (octaveRange - octave): cBasePitch * octaveMult << chordTranspose;

	}

	void clockedDetune(int32_t detuneMod) {

		aFreq = (cBasePitch * octaveMult) + (beatTimer >> 1);
		bFreq = (cBasePitch * octaveMult) - (beatTimer >> 1) + beatPhaseLock;

	}

	void (ViaOsc3::*doDetune)(int32_t detuneMod);

	inline void updateFrequencies(void) {

		octaveMult = 1 << (!octave * (octaveRange));

		int32_t detuneMod = -inputs.cv3Samples[0];
		detuneMod += cv3Calibration;

		(this->*doDetune)(detuneMod);

		cFreq = cBasePitch * octaveMult;

		int32_t pmInput = inputs.cv2Samples[0];
		pmInput -= cv2Calibration;

		// quadrature
		int32_t aError = aPhase - (cPhase + pmTracker + (1 << 30));
		int32_t bError = bPhase - (cPhase + pmTracker + (1 << 31));

		aFreq += __SSAT(aError * ((detuneBase == 0)), 12);
		bFreq += __SSAT(bError * ((detuneBase == 0)), 12);

		aFreq += __SSAT(aError * (unity), 14);
		bFreq += __SSAT(bError * (unity), 14);

		pm = (pmInput - lastPM) << 11;

		pmTracker += pm;

		lastPM = pmInput;

	}

	inline void parsePhase(int32_t aPhaseWorker, int32_t bPhaseWorker, int32_t cPhaseWorker) {

		int32_t resample = (cPhase > (0xFFFFFFFF - (1 << 30))) & (cPhaseWorker < (1 << 30));

		beatDetect(aPhaseWorker, bPhaseWorker);

		aPhase = aPhaseWorker;
		bPhase = bPhaseWorker;
		cPhase = cPhaseWorker;

		outputs.logicA[0] = GET_ALOGIC_MASK(beat /* (detuneBase != 0) */ * (unity == 0));
		outputs.auxLogic[0] = 0;
		outputs.shA[0] = GET_SH_A_MASK(resample * shAOn);
		outputs.shB[0] = GET_SH_B_MASK(resample * shBOn);

	}

	inline void showQuant(void) {
		setRedLED(hueSpace[scaleMode * 4].r);
		setGreenLED(hueSpace[scaleMode * 4].g);
		setBlueLED(hueSpace[scaleMode * 4].b);
	}

	/// Instance of the exponential converter class.
	ExpoConverter expo;

	uint32_t buttonPressed = 0;
	uint32_t auxLogicHigh = 0;

	int32_t multipliers[8] = {1, 2, 3, 4, 6, 8, 12, 16};

	int32_t beatMultiplier = 1;

	//@{
	/// Event handlers calling the corresponding methods from the state machine.
	void mainRisingEdgeCallback(void) override {

		octave = 1;

	}
	void mainFallingEdgeCallback(void) override {

		octave = 0;

	}
	void auxRisingEdgeCallback(void) override {

		if (!buttonPressed) {
			unity = !clockedBeat;
		}
		auxLogicHigh = 1;
		#ifdef BUILD_F373
		int32_t beatTime = TIM2->CNT;
		#endif
		#ifdef BUILD_VIRTUAL
		int32_t beatTime = readMeasurementTimer();
		#endif
		pllPileup += 1;

		if (beatTime > (45 * 128)) {

			#ifdef BUILD_F373
			TIM2->CNT = 0;
			#endif
			#ifdef BUILD_VIRTUAL
			resetMeasurementTimer();
			#endif

			int32_t error = aPhase - bPhase;

			// figure out a gradual phase locking function
			beatPhaseLock = ((45 * ((int64_t)(error)))/beatTime);
			beatTime = (45 * (((uint64_t)pllPileup << 32)))/(beatTime);
			beatTimer =	beatMultiplier * beatTime;

			pllPileup = 0;
		}

	}
	void auxFallingEdgeCallback(void) override {

		if (!buttonPressed) {
			unity = 0;
		}
		auxLogicHigh = 0;

	}
	void buttonPressedCallback(void) override {

		if (!auxLogicHigh) {
			unity = 1;
		}
		buttonPressed = 1;

	}
	void buttonReleasedCallback(void) override {

		if (!auxLogicHigh) {
			unity = 0;
		}
		buttonPressed = 0;

	}
	void ioProcessCallback(void) override {}
	void halfTransferCallback(void) override {
		setLogicOutNoA(0, runtimeDisplay);
		(this->*render)(0);
	}
	void transferCompleteCallback(void) override {
		setLogicOutNoA(0, runtimeDisplay);
		(this->*render)(OSC3_BUFFER_SIZE);
	}
	void slowConversionCallback(void) override {

		controls.updateExtra();
		
		(this->*updateBaseFreqs)();

		setAuxLogic(noteChange);
		if (runtimeDisplay) {
			setLEDA(noteChange);
		}

		if (clockedBeat) {
			int32_t multiplierCV = -inputs.cv3Samples[0];
			multiplierCV += cv3Calibration;
			multiplierCV >>= 4;
			beatMultiplier = multipliers[__USAT(controls.knob3Value + multiplierCV, 12) >> 9];
		}

		if (runtimeDisplay) {
			showQuant();
		}

	}
	void auxTimer1InterruptCallback(void) override {

	}
	void auxTimer2InterruptCallback(void) override {

	}

	int32_t numButton1Modes = 6;
	int32_t numButton2Modes = 4;
	int32_t numButton3Modes = 2;
	int32_t numButton4Modes = 5;
	int32_t numButton5Modes = 4;
	int32_t numButton6Modes = 4;

	void handleButton1ModeChange(int32_t);
	void handleButton2ModeChange(int32_t);
	void handleButton3ModeChange(int32_t);
	void handleButton4ModeChange(int32_t);
	void handleButton5ModeChange(int32_t);
	void handleButton6ModeChange(int32_t);

	void readCalibrationPacket(void) {
		calibrationPacket = osc3UI.loadFromMemory(7);
		decodeCalibrationPacket();
	}

	/// On construction, call subclass constructors and pass each a pointer to the module class.
#ifdef BUILD_F373
	ViaOsc3() : osc3UI(*this) {
#endif
#ifdef BUILD_VIRTUAL
    ViaOsc3(std::string binPath) : osc3UI(*this) {
#endif

        #ifdef BUILD_VIRTUAL
        scales = (Osc3Scale *) malloc(8 * sizeof(Osc3Scale));
        readScalesFromFile(binPath);
        #endif

		/// Link the module GPIO registers.
		initializeAuxOutputs();

		/// Initialize the input stream buffers.
		inputs.init(OSC3_BUFFER_SIZE);
		/// Initialize the output stream buffers.
		outputs.init(OSC3_BUFFER_SIZE);
		/// Set the data members that will be used to determine DMA stream initialization in the hardware executable.
		outputBufferSize = OSC3_BUFFER_SIZE;
		inputBufferSize = 1;

		render = &ViaOsc3::renderTri;
		doDetune = &ViaOsc3::linearDetune;

		/// Call the UI initialization that needs to happen after outer class construction.
		osc3UI.initialize();

		uint32_t optionBytes = readOptionBytes();
		uint32_t ob1Data = optionBytes & 0xFFFF;
		uint32_t ob2Data = optionBytes >> 16;

		if (ob1Data == 254 && ob2Data == 255) {
			readCalibrationPacket();
			osc3UI.writeStockPresets();
			writeOptionBytes(6, 4);
		} else if (ob1Data == 6) {
			readCalibrationPacket();
			if (ob2Data != 4) {
				writeOptionBytes(6, 4);
			}
		} else if (ob1Data != 0) {
			writeOptionBytes(0, 0);
		}

	}

};

#endif /* INC_Calib_HPP_ */
