/*
 * signal_processors.h
 *
 *  Created on: Aug 18, 2018
 *      Author: willmitchell
 */

#ifndef INC_SIGNAL_PROCESSORS_HPP_
#define INC_SIGNAL_PROCESSORS_HPP_

#include "via_platform_binding.hpp"
#include "tables.hpp"

/*
 *
 * Three axis scanner
 *
 */

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

	int32_t lastDeltaXState = 1;
	int32_t deltaXTransitionSample = 0;
	int32_t deltaXOutputStable = 0;

	inline int32_t deltaXHysterisis(int32_t thisDeltaState, int32_t sample) {

		if (deltaXOutputStable) {
			deltaXOutputStable = ((lastDeltaXState - thisDeltaState) == 0);
			deltaXTransitionSample = sample;
			lastDeltaXState = thisDeltaState;
			return thisDeltaState ^ xReversed;
		} else {
			deltaXOutputStable = (abs(sample - deltaXTransitionSample) > 1);
			lastDeltaXState = deltaXOutputStable ? thisDeltaState : lastDeltaXState;
			return lastDeltaXState;
		}

	}

	int32_t lastDeltaYState = 1;
	int32_t deltaYTransitionSample = 0;
	int32_t deltaYOutputStable = 0;

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

	int32_t lastHemisphereXState = 1;
	int32_t hemisphereXTransitionSample = 0;
	int32_t hemisphereXOutputStable = 0;

	inline int32_t hemisphereXHysterisis(int32_t thisHemisphereState, int32_t sample) {

		if (hemisphereXOutputStable) {
			hemisphereXOutputStable = ((lastHemisphereXState - thisHemisphereState) == 0);
			hemisphereXTransitionSample = sample;
			lastHemisphereXState = thisHemisphereState;
			return thisHemisphereState;
		} else {
			hemisphereXOutputStable = (abs(sample - hemisphereXTransitionSample) > 1);
			lastHemisphereXState = hemisphereXOutputStable ? thisHemisphereState : lastHemisphereXState;
			return lastHemisphereXState;
		}

	}

	int32_t lastHemisphereYState = 1;
	int32_t hemisphereYTransitionSample = 0;
	int32_t hemisphereYOutputStable = 0;

	inline int32_t hemisphereYHysterisis(int32_t thisHemisphereState, int32_t sample) {

		if (hemisphereYOutputStable) {
			hemisphereYOutputStable = ((lastHemisphereYState - thisHemisphereState) == 0);
			hemisphereYTransitionSample = sample;
			lastHemisphereYState = thisHemisphereState;
			return thisHemisphereState;
		} else {
			hemisphereYOutputStable = (abs(sample - hemisphereYTransitionSample) > 1);
			lastHemisphereYState = hemisphereYOutputStable ? thisHemisphereState : lastHemisphereYState;
			return lastHemisphereYState;
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

	// mode parameters
	uint32_t terrainType = 0;
	uint32_t syncMode = 0;
	uint32_t xTableSize = 0;
	uint32_t yTableSize = 0;
	uint32_t xInterpolateOn = 1;
	uint32_t yInterpolateOn = 1;

	int32_t * locationBlend;
	int32_t * altitude;

	int32_t hemisphereBlend = 0;
	int32_t deltaBlend = 0;

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


#endif /* INC_SIGNAL_PROCESSORS_HPP_ */
