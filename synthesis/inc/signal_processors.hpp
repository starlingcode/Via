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
	int32_t deltaXRising = 0;

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
	int32_t deltaYRising = 0;

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
