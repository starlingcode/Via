/*
 * scanner.hpp
 *
 *  Created on: Sep 17, 2018
 *      Author: willmitchell
 */

#ifndef INC_SCANNER_HPP_
#define INC_SCANNER_HPP_

#include "user_interface.hpp"
#include <via_platform_binding.hpp>
#include <scanner_tables.hpp>

class ThreeAxisScanner {

private:

	int32_t oversample = 0;

	int32_t lastXInput = 0;
	int32_t lastYInput = 0;
	int32_t lastXIndex = 0;
	int32_t lastYIndex = 0;

	int32_t * xIndexBuffer;
	int32_t * yIndexBuffer;

	inline void scanTerrainMultiply(void);

	inline void scanTerrainLighten(void);

	inline void scanTerrainSum(void);

	inline void scanTerrainDifference(void);

	int32_t xReversed = 0;
	int32_t yReversed = 0;

	int32_t lastXReversed = 0;
	int32_t lastYReversed = 0;

	int32_t xOffset = 0;
	int32_t yOffset = 0;
	int32_t xOffsetControl = 0;
	int32_t yOffsetControl = 0;

//	uint32_t noisyZIndex = 0;
//	uint32_t lastZIndex = 0;
//
//	void scannerZHysterisis(void) {
//		if (zIndex != lastZIndex) {
//			if (zIndex > lastZIndex && zIndex > noisyZIndex) {
//				noisyZIndex = lastZIndex;
//				lastZIndex = zIndex;
//			} else if (zIndex < lastZIndex && zIndex < noisyZIndex) {
//				noisyZIndex = lastZIndex;
//				lastZIndex = zIndex;
//			} else {
//				zIndex = lastZIndex;
//			}
//		}
//	}

	int32_t lastXValue = 1;
	int32_t xValueTransitionSample = 0;
	int32_t xValueOutputStable = 0;

	inline int32_t xValueHysterisis(int32_t thisValue, int32_t sample) {

		if (xValueOutputStable) {
			xValueOutputStable = ((lastXValue - thisValue) == 0);
			xValueTransitionSample = sample;
			lastXValue = thisValue;
			return thisValue;
		} else {
			xValueOutputStable = (abs(sample - xValueTransitionSample) > 1);
			lastXValue = xValueOutputStable ? thisValue : lastXValue;
			return lastXValue;
		}

	}

	int32_t lastYValue = 1;
	int32_t yValueTransitionSample = 0;
	int32_t yValueOutputStable = 0;

	inline int32_t yValueHysterisis(int32_t thisValue, int32_t sample) {

		if (yValueOutputStable) {
			yValueOutputStable = ((lastYValue - thisValue) == 0);
			yValueTransitionSample = sample;
			lastYValue = thisValue;
			return thisValue;
		} else {
			yValueOutputStable = (abs(sample - yValueTransitionSample) > 1);
			lastYValue = yValueOutputStable ? thisValue : lastYValue;
			return lastYValue;
		}

	}


	int32_t lastDeltaXState = 1;
	int32_t deltaXTransitionSample = 0;
	int32_t deltaXOutputStable = 0;
	// int32_t deltaXRising = 0;

	inline int32_t deltaXHysterisis(int32_t thisDeltaState, int32_t sample) {

		if (deltaXOutputStable) {
			deltaXOutputStable = ((lastDeltaXState - thisDeltaState) == 0);
			deltaXTransitionSample = sample;
			lastDeltaXState = thisDeltaState;
			return thisDeltaState;
		} else {
			deltaXOutputStable = (abs(sample - deltaXTransitionSample) > 1);
			lastDeltaXState = deltaXOutputStable ? thisDeltaState : lastDeltaXState;
			return lastDeltaXState;
		}

	}

	int32_t lastDeltaYState = 1;
	int32_t deltaYTransitionSample = 0;
	int32_t deltaYOutputStable = 0;
	// int32_t deltaYRising = 0;

	inline int32_t deltaYHysterisis(int32_t thisDeltaState, int32_t sample) {

		if (deltaYOutputStable) {
			deltaYOutputStable = ((lastDeltaYState - thisDeltaState) == 0);
			deltaYTransitionSample = sample;
			lastDeltaYState = thisDeltaState;
			return thisDeltaState;
		} else {
			deltaYOutputStable = (abs(sample - deltaYTransitionSample) > 1);
			lastDeltaYState = deltaYOutputStable ? thisDeltaState : lastDeltaYState;
			return lastDeltaYState;
		}

	}

	int32_t xHemisphere = 0;

	inline void hemisphereXHysterisis(uint32_t xValue) {

		if (xHemisphere) {
			xHemisphere = (xValue > (1 << 14) - 256);
		} else {
			xHemisphere = (xValue > (1 << 14) + 256);
		}

	}

	int32_t yHemisphere = 0;

	inline void hemisphereYHysterisis(uint32_t yValue) {

		if (yHemisphere) {
			yHemisphere = (yValue > (1 << 14) - 256);
		} else {
			yHemisphere = (yValue > (1 << 14) + 256);
		}

	}


public:

	uint32_t *xTable;
	uint32_t *yTable;

	int32_t xInput = 0;
	int32_t yInput = 0;
	int32_t hardSync = 0;
	int32_t reverse = 0;

	int32_t cv1Offset = 0;
	int32_t cv2Offset = 0;
	int32_t cv3Offset = 0;

	//control rate input
	uint32_t zIndex = 0;
	uint32_t zIndexControl = 0;

	// mode parameters
	uint32_t terrainType = 0;
	uint32_t syncMode = 0;
	uint32_t xTableSize = 0;
	uint32_t yTableSize = 0;
	uint32_t xInterpolateOff = 0;
	uint32_t yInterpolateOff = 0;

	int32_t * locationBlend;
	int32_t * altitude;

	int32_t hemisphereBlend = 0;
	int32_t deltaBlend = 0;

	// dumb
	int32_t fix32Mul(int32_t x, int32_t y) {
		int64_t out = (int64_t) x * (int64_t) y;
		out >>= 32;
		return (int32_t) out;
	}

#define UPSAMPLE_CUTOFF (1 << 28)

	void updateControlParams(void) {
		xOffset += fix32Mul(xOffsetControl - xOffset, UPSAMPLE_CUTOFF);
		yOffset += fix32Mul(yOffsetControl - yOffset, UPSAMPLE_CUTOFF);
		zIndex += fix32Mul(zIndexControl - zIndex, UPSAMPLE_CUTOFF);
	}


	inline void fillBufferExternal(void) {
		(this->*fillBuffer)();
	}

	void (ThreeAxisScanner::*fillBuffer)(void);

	void fillBufferSum(void);
	void fillBufferMultiply(void);
	void fillBufferDifference(void);
	void fillBufferLighten(void);

	inline void scanSetup(void);

	void parseControls(ViaControls * controls);

	int32_t bufferSize = 8;

	void init(void) {

		xIndexBuffer = (int32_t *) malloc(bufferSize * sizeof(int32_t));
		yIndexBuffer = (int32_t *) malloc(bufferSize * sizeof(int32_t));
		locationBlend = (int32_t *) malloc(bufferSize * sizeof(int32_t));
		altitude = (int32_t *) malloc(bufferSize * sizeof(int32_t));

		for (int32_t i = 0; i < bufferSize; i++) {
			xIndexBuffer[i] = 0;
			yIndexBuffer[i] = 0;
			locationBlend[i] = 0;
			altitude[i] = 0;
		}

	}

};


#define SCANNER_BUFFER_SIZE 8

/*
 *
 * Modes
 *
 */

#define X_TABLE button4Mode
#define Y_TABLE button2Mode
#define SYNC_MODE button1Mode
#define TERRAIN button3Mode

#define numButton1Modes 2
#define numButton2Modes 8
#define numButton3Modes 4
#define numButton4Modes 8
#define numButton5Modes 8
#define numButton6Modes 8
#define numAux1Modes 0
#define numAux2Modes 0
#define numAux3Modes 0
#define numAux4Modes 0

enum button1Modes {
	reverse, hardsync
};
enum button2Modes {
	yTables
};
enum button3Modes {
	sum, multiply, difference, lighten
};
enum button4Modes {
	xTables
};
enum button5Modes {
	pairedWithButton2
};
enum button6Modes {
	pairedWithButton3
};
enum aux1Modes {
	aux1NotUsed
};
enum aux2Modes {
	aux2NotUsed
};
enum aux3Modes {
	aux3NotUsed
};
enum aux4Modes {
	aux4NotUsed
};

void scannerTouchLink (void *);

class ViaScanner : public TARGET_VIA {

	int32_t readIndex = 0;

public:

	class ViaScannerUI: public ViaUI {

	public:

		ViaScanner& this_module;

		void button1TapCallback(void) override;
		void button1HoldCallback(void) override;
		void button2TapCallback(void) override;
		void button2HoldCallback(void) override;
		void button3TapCallback(void) override;
		void button3HoldCallback(void) override;
		void button4TapCallback(void) override;
		void button4HoldCallback(void) override;
		void button5TapCallback(void) override;
		void button5HoldCallback(void) override;
		void button6TapCallback(void) override;
		void button6HoldCallback(void) override;

		void aux1TapCallback(void) override;
		void aux1HoldCallback(void) override;
		void aux2TapCallback(void) override;
		void aux2HoldCallback(void) override;
		void aux2AltTapCallback(void) override;
		void aux2AltHoldCallback(void) override;
		void aux3TapCallback(void) override;
		void aux3HoldCallback(void) override;
		void aux4TapCallback(void) override;
		void aux4HoldCallback(void) override;

		void uiSetLEDs(int) override;

		void recallModuleState(void) override;

		void defaultEnterMenuCallback(void) override;
		void newModeEnterMenuCallback(void) override;
		void newAuxModeEnterMenuCallback(void) override;
		void presetEnterMenuCallback(void) override;

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

		void initialize(void) override;

		void writeStockPresets(void) override;

		// TODO use enums, fix for scanner ....
		// slopes
		uint32_t stockPreset1 = ENCODE_PRESET(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		// physics world
		uint32_t stockPreset2 = ENCODE_PRESET(0, 2, 0, 3, 0, 0, 0, 0, 0, 0);
		// shapeshifting mult
		uint32_t stockPreset3 = ENCODE_PRESET(0, 3, 0, 4, 0, 0, 0, 0, 0, 0);
		// synthville
		uint32_t stockPreset4 = ENCODE_PRESET(0, 5, 0, 5, 0, 0, 0, 0, 0, 0);
		// staircases
		uint32_t stockPreset5 = ENCODE_PRESET(0, 6, 0, 6, 0, 0, 0, 0, 0, 0);
		// blockland
		uint32_t stockPreset6 = ENCODE_PRESET(0, 7, 0, 7, 0, 0, 0, 0, 0, 0);

		ViaScannerUI(ViaScanner& x): this_module(x) {
			linkUI((void *) &scannerTouchLink, (void *) this);
		}

	};

	void handleButton1ModeChange(int32_t);
	void handleButton2ModeChange(int32_t);
	void handleButton3ModeChange(int32_t);
	void handleButton4ModeChange(int32_t);
	void handleButton5ModeChange(int32_t);
	void handleButton6ModeChange(int32_t);
	void handleAux1ModeChange(int32_t);
	void handleAux2ModeChange(int32_t);
	void handleAux3ModeChange(int32_t);
	void handleAux4ModeChange(int32_t);

	/*
	 *
	 * Tables
	 *
	 */

	ScannerWavetableSet wavetableSet;

	// declare an array of pointers to wavetables (stored in flash)
	const Wavetable * wavetableArray[2][8];

	// declare arrays to store the active tables
	uint32_t wavetableXRead[5][517];
	uint32_t wavetableYRead[5][517];

	void fillWavetableArray(void);

	// declare functions to set the currently active tables
	void switchWavetableX(const Wavetable *);
	void switchWavetableY(const Wavetable *);

	// phase distortion table is fixed
	void initPhaseDistTable(void);

	int32_t reverseBuffer[SCANNER_BUFFER_SIZE*2];

	int32_t reverseSignal = 1;

	/*
	 *
	 * Synthesis classes and init
	 *
	 */


	void init(void);

	ViaScannerUI scannerUI;

	int32_t runtimeDisplay;

	ThreeAxisScanner scanner;

	/*
	 *
	 * Event scanner_handlers
	 *
	 */

	ViaScanner() : scannerUI(*this) {
		init();
	}

	void readCalibrationPacket(void) {
		calibrationPacket = scannerUI.loadFromMemory(7);
		decodeCalibrationPacket();
	}

	void mainRisingEdgeCallback(void);
	void mainFallingEdgeCallback(void);

	void auxRisingEdgeCallback(void);
	void auxFallingEdgeCallback(void);

	void buttonPressedCallback(void);
	void buttonReleasedCallback(void);

	void ioProcessCallback(void);
	void halfTransferCallback(void);
	void transferCompleteCallback(void);
	void cv2HalfTransferCallback(void);
	void cv2TransferCompleteCallback(void);
	void cv3HalfTransferCallback(void);
	void cv3TransferCompleteCallback(void);
	void slowConversionCallback(void);

	void ui_dispatch(int32_t sig) {
		scannerUI.dispatch(sig);
	};

};



#endif /* INC_SCANNER_HPP_ */
