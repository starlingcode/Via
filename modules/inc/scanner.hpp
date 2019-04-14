/*
 * scanner.hpp
 *
 *  Created on: Sep 17, 2018
 *      Author: willmitchell
 */

#ifndef INC_SCANNER_HPP_
#define INC_SCANNER_HPP_



#ifdef __cplusplus
extern "C" {
#endif

#include "user_interface.hpp"
#include <via_platform_binding.hpp>
#include <signal_processors.hpp>
#include <scanner_tables.hpp>

#ifdef __cplusplus
}
#endif

#define SCANNER_BUFFER_SIZE 8

/*
 *
 * Modes
 *
 */

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

class ViaScanner : public ViaModule {

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
