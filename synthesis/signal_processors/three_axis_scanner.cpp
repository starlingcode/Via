/*
 * three_axis_scanner.c
 *
 *  Created on: Aug 22, 2018
 *      Author: willmitchell
 */

#include "signal_processors.hpp"


void ThreeAxisScanner::parseControls(ViaControls * controls) {

	xOffset = (controls->knob2Value - 2048) << 13;
	yOffset = (controls->knob3Value - 2048) << 13;

	int32_t zKnobCV = controls->knob1Value - 2200 + controls->cv1Value;
	zIndex = __USAT(zKnobCV, 12) << 6;

//	scannerXHysterisis();
//	scannerYHysterisis();
//	scannerZHysterisis();

}

void ThreeAxisScanner::scanSetup() {

	int32_t xIncrement = (xInput - lastXInput) * reverse;
	int32_t yIncrement = (yInput - lastYInput) * reverse;

	if (int32_abs(xIncrement) > 512 || int32_abs(yIncrement) > 512) {
		oversample = 1;
	} else {
		oversample = 0;
	}

	// store last value
	lastXInput = xInput;
	lastYInput = yInput;

	int32_t xIndex = lastXIndex * hardSync;
	int32_t yIndex = lastYIndex * hardSync;

	int32_t samplesRemaining = bufferSize;
	int32_t writeIndex = 0;

	while (samplesRemaining) {

		xIndex += xIncrement;
		yIndex += yIncrement;

		xIndexBuffer[writeIndex] = foldSignal25Bit((xIndex << 8) + xOffset);
		yIndexBuffer[writeIndex] = foldSignal25Bit((yIndex << 8) + yOffset);

		samplesRemaining --;
		writeIndex ++;

	}

	// store
	lastXIndex = xIndex;
	lastYIndex = yIndex;

	xReversed = (xIncrement == 0) ? lastXReversed : (uint32_t) xIncrement >> 31;
	yReversed = (yIncrement == 0) ? lastYReversed : (uint32_t) yIncrement >> 31;

	lastXReversed = xReversed;
	lastYReversed = yReversed;

}

void ThreeAxisScanner::fillBufferSum() {

	scanSetup();

	scanTerrainSum();

}

void ThreeAxisScanner::fillBufferMultiply() {

	scanSetup();

	scanTerrainMultiply();

}

void ThreeAxisScanner::fillBufferDifference() {

	scanSetup();

	scanTerrainDifference();

}

void ThreeAxisScanner::fillBufferLighten() {

	scanSetup();

	scanTerrainLighten();

}

inline void ThreeAxisScanner::scanTerrainSum(void) {

	uint32_t writeIndex = 0;

	int32_t xDelta = 0;
	int32_t yDelta = 0;

	int32_t xSample;
	int32_t ySample;

	int32_t * xTableRead = (int32_t *) xTable + (517 * (zIndex >> 16)) + 2;
	int32_t * yTableRead = (int32_t *) yTable + (517 * (zIndex >> 16)) + 2;

	int32_t leftSample;
	int32_t morphFrac;
	int32_t phaseFrac;

	int32_t samplesRemaining = bufferSize - 1;

	int32_t xIndexAtLogic;
	int32_t yIndexAtLogic;

	if (oversample == 0) {
		if (!xInterpolateOff) {
			xSample = getSampleQuinticSplineDeltaValue(xIndexBuffer[writeIndex], zIndex,
				(uint32_t *) xTable, &xDelta, xInterpolateOff);
		} else {
			phaseFrac = xIndexBuffer[writeIndex] >> 16;
			morphFrac = zIndex & 0xFFFF;
			leftSample = fast_15_16_lerp_prediff(xTableRead[phaseFrac], morphFrac);
			xSample = xValueHysterisis(leftSample, phaseFrac);
			xDelta = fast_15_16_lerp_prediff(xTableRead[phaseFrac + 1], morphFrac) - leftSample;
		}
		if (!yInterpolateOff) {
			ySample = getSampleQuinticSplineDeltaValue(yIndexBuffer[writeIndex], zIndex,
				(uint32_t *) yTable, &yDelta, yInterpolateOff);
		} else {
			phaseFrac = yIndexBuffer[writeIndex] >> 16;
			morphFrac = zIndex & 0xFFFF;
			leftSample = fast_15_16_lerp_prediff(yTableRead[phaseFrac], morphFrac);
			ySample = yValueHysterisis(leftSample, phaseFrac);
			yDelta = fast_15_16_lerp_prediff(yTableRead[phaseFrac + 1], morphFrac) - leftSample;
		}

		xIndexAtLogic = xIndexBuffer[writeIndex] >> 16;
		yIndexAtLogic = yIndexBuffer[writeIndex] >> 16;

		int32_t mainScan = (xSample + ySample) >> 4;

		int32_t samplesRemaining = bufferSize;

		while (samplesRemaining) {
			altitude[writeIndex] = mainScan;
			locationBlend[writeIndex] = (xIndexBuffer[writeIndex] + yIndexBuffer[writeIndex]) >> 14;

			writeIndex ++;
			samplesRemaining --;

		}

	} else {

		while (samplesRemaining) {

			leftSample = xIndexBuffer[writeIndex] >> 16;
			phaseFrac = (xIndexBuffer[writeIndex]) & 0xFFFF;
			morphFrac =  zIndex & 0xFFFF;

			xSample = fast_15_16_bilerp_prediff(xTableRead[leftSample], xTableRead[leftSample + 1], morphFrac, phaseFrac);

			leftSample = yIndexBuffer[writeIndex] >> 16;
			phaseFrac = (yIndexBuffer[writeIndex]) & 0xFFFF;

			ySample = fast_15_16_bilerp_prediff(yTableRead[leftSample], yTableRead[leftSample + 1], morphFrac, phaseFrac);

			altitude[writeIndex] = (xSample + ySample) >> 4;
			locationBlend[writeIndex] = (xIndexBuffer[writeIndex] + yIndexBuffer[writeIndex]) >> 14;
			writeIndex ++;
			samplesRemaining --;

		}

		leftSample = xIndexBuffer[writeIndex] >> 16;
		xIndexAtLogic = leftSample;
		phaseFrac = (xIndexBuffer[writeIndex]) & 0xFFFF;
		morphFrac =  zIndex & 0xFFFF;

		xSample = fast_15_16_bilerp_prediff_deltaValue(xTableRead[leftSample], xTableRead[leftSample + 1], morphFrac, phaseFrac, &xDelta);

		leftSample = yIndexBuffer[writeIndex]>> 16;
		yIndexAtLogic = leftSample;
		phaseFrac = (yIndexBuffer[writeIndex]) & 0xFFFF;

		ySample = fast_15_16_bilerp_prediff_deltaValue(yTableRead[leftSample], yTableRead[leftSample + 1], morphFrac, phaseFrac, &yDelta);

		altitude[writeIndex] = (xSample + ySample) >> 4;
		locationBlend[writeIndex] = (xIndexBuffer[writeIndex] + yIndexBuffer[writeIndex]) >> 14;
	}

	hemisphereXHysterisis(xSample);
	hemisphereYHysterisis(ySample);

	xDelta = (xDelta == 0) ? lastDeltaXState : ((uint32_t) xDelta >> 31);
	yDelta = (yDelta == 0) ? lastDeltaYState : ((uint32_t) yDelta >> 31);

	xDelta = deltaXHysterisis(xDelta, xIndexAtLogic);
	yDelta = deltaYHysterisis(yDelta, yIndexAtLogic);

	hemisphereBlend = xHemisphere | yHemisphere;
	deltaBlend = xDelta | yDelta;


}

inline void ThreeAxisScanner::scanTerrainMultiply(void) {

	uint32_t writeIndex = 0;

	int32_t xDelta;
	int32_t yDelta;

	int32_t xSample;
	int32_t ySample;

	int32_t samplesRemaining = bufferSize - 1;

	int32_t * xTableRead = (int32_t *) xTable + (517 * (zIndex >> 16)) + 2;
	int32_t * yTableRead = (int32_t *) yTable + (517 * (zIndex >> 16)) + 2;

	int32_t leftSample;
	int32_t morphFrac;
	int32_t phaseFrac;

	int32_t xIndexAtLogic;
	int32_t yIndexAtLogic;

	if (oversample == 0) {

		if (!xInterpolateOff) {
			xSample = getSampleQuinticSplineDeltaValue(xIndexBuffer[writeIndex], zIndex,
				(uint32_t *) xTable, &xDelta, xInterpolateOff);
		} else {
			phaseFrac = xIndexBuffer[writeIndex] >> 16;
			morphFrac = zIndex & 0xFFFF;
			leftSample = fast_15_16_lerp_prediff(xTableRead[phaseFrac], morphFrac);
			xSample = xValueHysterisis(leftSample, phaseFrac);
			xDelta = fast_15_16_lerp_prediff(xTableRead[phaseFrac + 1], morphFrac) - leftSample;
		}
		if (!yInterpolateOff) {
			ySample = getSampleQuinticSplineDeltaValue(yIndexBuffer[writeIndex], zIndex,
				(uint32_t *) yTable, &yDelta, yInterpolateOff);
		} else {
			phaseFrac = yIndexBuffer[writeIndex] >> 16;
			morphFrac = zIndex & 0xFFFF;
			leftSample = fast_15_16_lerp_prediff(yTableRead[phaseFrac], morphFrac);
			ySample = yValueHysterisis(leftSample, phaseFrac);
			yDelta = fast_15_16_lerp_prediff(yTableRead[phaseFrac + 1], morphFrac) - leftSample;
		}

		xIndexAtLogic = xIndexBuffer[writeIndex] >> 16;
		yIndexAtLogic = yIndexBuffer[writeIndex] >> 16;

		int32_t mainScan = (((xSample - (1<<14)) * (ySample - (1<<14))) >> 17) + 2048; //15 bit fixed point multiply and right shift by 3

		int32_t samplesRemaining = bufferSize;

		while (samplesRemaining) {
			altitude[writeIndex] = mainScan;
			locationBlend[writeIndex] = ((((xIndexBuffer[writeIndex] >> 13) - 2048) * ((yIndexBuffer[writeIndex] >> 13) - 2048)) >> 12) + 2048;

			writeIndex ++;
			samplesRemaining --;
		}

	} else {

		while (samplesRemaining) {

			leftSample = xIndexBuffer[writeIndex]>> 16;
			phaseFrac = (xIndexBuffer[writeIndex]) & 0xFFFF;
			morphFrac =  zIndex & 0xFFFF;

			xSample = fast_15_16_bilerp_prediff(xTableRead[leftSample], xTableRead[leftSample + 1], morphFrac, phaseFrac);

			leftSample = yIndexBuffer[writeIndex]>> 16;
			phaseFrac = (yIndexBuffer[writeIndex]) & 0xFFFF;

			ySample = fast_15_16_bilerp_prediff(yTableRead[leftSample], yTableRead[leftSample + 1], morphFrac, phaseFrac);

			altitude[writeIndex] = (((xSample - (1<<14)) * (ySample - (1<<14))) >> 17) + 2048;
			locationBlend[writeIndex] = ((((xIndexBuffer[writeIndex] >> 13) - 2048) * ((yIndexBuffer[writeIndex] >> 13) - 2048)) >> 12) + 2048;

			writeIndex ++;
			samplesRemaining --;

		}

		leftSample = xIndexBuffer[writeIndex]>> 16;
		xIndexAtLogic = leftSample;
		phaseFrac = (xIndexBuffer[writeIndex]) & 0xFFFF;
		morphFrac =  zIndex & 0xFFFF;

		xSample = fast_15_16_bilerp_prediff_deltaValue(xTableRead[leftSample], xTableRead[leftSample + 1], morphFrac, phaseFrac, &xDelta);

		leftSample = yIndexBuffer[writeIndex]>> 16;
		yIndexAtLogic = leftSample;
		phaseFrac = (yIndexBuffer[writeIndex]) & 0xFFFF;

		ySample = fast_15_16_bilerp_prediff_deltaValue(yTableRead[leftSample], yTableRead[leftSample + 1], morphFrac, phaseFrac, &yDelta);

		altitude[writeIndex] = (((xSample - (1<<14)) * (ySample - (1<<14))) >> 17) + 2048;
		locationBlend[writeIndex] = ((((xIndexBuffer[writeIndex] >> 13) - 2048) * ((yIndexBuffer[writeIndex] >> 13) - 2048)) >> 12) + 2048;

	}

	hemisphereXHysterisis(xSample);
	hemisphereYHysterisis(ySample);

	xDelta = (xDelta == 0) ? lastDeltaXState : ((uint32_t) xDelta >> 31);
	yDelta = (yDelta == 0) ? lastDeltaYState : ((uint32_t) yDelta >> 31);

	xDelta = deltaXHysterisis(xDelta, xIndexAtLogic);
	yDelta = deltaYHysterisis(yDelta, yIndexAtLogic);

	hemisphereBlend = xHemisphere & yHemisphere;
	deltaBlend = xDelta & yDelta;


}

inline void ThreeAxisScanner::scanTerrainDifference(void) {

	uint32_t writeIndex = 0;

	int32_t xDelta;
	int32_t yDelta;

	int32_t xSample;
	int32_t ySample;

	int32_t samplesRemaining = bufferSize - 1;

	int32_t * xTableRead = (int32_t *) xTable + (517 * (zIndex >> 16)) + 2;
	int32_t * yTableRead = (int32_t *) yTable + (517 * (zIndex >> 16)) + 2;

	int32_t leftSample;
	int32_t morphFrac;
	int32_t phaseFrac;

	int32_t xIndexAtLogic;
	int32_t yIndexAtLogic;

	if (oversample == 0) {

		if (!xInterpolateOff) {
			xSample = getSampleQuinticSplineDeltaValue(xIndexBuffer[writeIndex], zIndex,
				(uint32_t *) xTable, &xDelta, xInterpolateOff);
		} else {
			phaseFrac = xIndexBuffer[writeIndex] >> 16;
			morphFrac = zIndex & 0xFFFF;
			leftSample = fast_15_16_lerp_prediff(xTableRead[phaseFrac], morphFrac);
			xSample = xValueHysterisis(leftSample, phaseFrac);
			xDelta = fast_15_16_lerp_prediff(xTableRead[phaseFrac + 1], morphFrac) - leftSample;
		}
		if (!yInterpolateOff) {
			ySample = getSampleQuinticSplineDeltaValue(yIndexBuffer[writeIndex], zIndex,
				(uint32_t *) yTable, &yDelta, yInterpolateOff);
		} else {
			phaseFrac = yIndexBuffer[writeIndex] >> 16;
			morphFrac = zIndex & 0xFFFF;
			leftSample = fast_15_16_lerp_prediff(yTableRead[phaseFrac], morphFrac);
			ySample = yValueHysterisis(leftSample, phaseFrac);
			yDelta = fast_15_16_lerp_prediff(yTableRead[phaseFrac + 1], morphFrac) - leftSample;
		}

		xIndexAtLogic = xIndexBuffer[writeIndex] >> 16;
		yIndexAtLogic = yIndexBuffer[writeIndex] >> 16;

		int32_t mainScan = int32_abs(xSample - ySample) >> 3; //15 bit fixed point multiply and right shift by 3

		int32_t samplesRemaining = bufferSize;

		while (samplesRemaining) {
			altitude[writeIndex] = mainScan;
			locationBlend[writeIndex] = int32_abs((xIndexBuffer[writeIndex] - yIndexBuffer[writeIndex]) >> 13);

			writeIndex ++;
			samplesRemaining --;

		}

	} else {

		while (samplesRemaining) {

			leftSample = xIndexBuffer[writeIndex]>> 16;
			phaseFrac = (xIndexBuffer[writeIndex]) & 0xFFFF;
			morphFrac =  zIndex & 0xFFFF;

			xSample = fast_15_16_bilerp_prediff(xTableRead[leftSample], xTableRead[leftSample + 1], morphFrac, phaseFrac);

			leftSample = yIndexBuffer[writeIndex]>> 16;
			phaseFrac = (yIndexBuffer[writeIndex]) & 0xFFFF;

			ySample = fast_15_16_bilerp_prediff(yTableRead[leftSample], yTableRead[leftSample + 1], morphFrac, phaseFrac);

			altitude[writeIndex] = int32_abs(xSample - ySample) >> 3;
			locationBlend[writeIndex] = int32_abs((xIndexBuffer[writeIndex] - yIndexBuffer[writeIndex]) >> 13);

			writeIndex ++;
			samplesRemaining --;

		}

		leftSample = xIndexBuffer[writeIndex]>> 16;
		xIndexAtLogic = leftSample;
		phaseFrac = (xIndexBuffer[writeIndex]) & 0xFFFF;
		morphFrac =  zIndex & 0xFFFF;

		xSample = fast_15_16_bilerp_prediff_deltaValue(xTableRead[leftSample], xTableRead[leftSample + 1], morphFrac, phaseFrac, &xDelta);

		leftSample = yIndexBuffer[writeIndex]>> 16;
		yIndexAtLogic = leftSample;
		phaseFrac = (yIndexBuffer[writeIndex]) & 0xFFFF;

		ySample = fast_15_16_bilerp_prediff_deltaValue(yTableRead[leftSample], yTableRead[leftSample + 1], morphFrac, phaseFrac, &yDelta);

		altitude[writeIndex] = int32_abs(xSample - ySample) >> 3;
		locationBlend[writeIndex] = int32_abs((xIndexBuffer[writeIndex] - yIndexBuffer[writeIndex]) >> 13);

	}

	hemisphereXHysterisis(xSample);
	hemisphereYHysterisis(ySample);

	xDelta = (xDelta == 0) ? lastDeltaXState : ((uint32_t) xDelta >> 31);
	yDelta = (yDelta == 0) ? lastDeltaYState : ((uint32_t) yDelta >> 31);

	xDelta = deltaXHysterisis(xDelta, xIndexAtLogic);
	yDelta = deltaYHysterisis(yDelta, yIndexAtLogic);

	hemisphereBlend = xHemisphere ^ yHemisphere;
	deltaBlend = xDelta ^ yDelta;

}

inline void ThreeAxisScanner::scanTerrainLighten(void) {

	uint32_t writeIndex = 0;

	int32_t xDelta;
	int32_t yDelta;

	int32_t xSample;
	int32_t ySample;

	int32_t samplesRemaining = bufferSize - 1;

	int32_t * xTableRead = (int32_t *) xTable + (517 * (zIndex >> 16)) + 2;
	int32_t * yTableRead = (int32_t *) yTable + (517 * (zIndex >> 16)) + 2;

	int32_t leftSample;
	int32_t morphFrac;
	int32_t phaseFrac;

	int32_t xIndexAtLogic;
	int32_t yIndexAtLogic;

	if (oversample == 0) {

		if (!xInterpolateOff) {
			xSample = getSampleQuinticSplineDeltaValue(xIndexBuffer[writeIndex], zIndex,
				(uint32_t *) xTable, &xDelta, xInterpolateOff);
		} else {
			phaseFrac = xIndexBuffer[writeIndex] >> 16;
			morphFrac = zIndex & 0xFFFF;
			leftSample = fast_15_16_lerp_prediff(xTableRead[phaseFrac], morphFrac);
			xSample = xValueHysterisis(leftSample, phaseFrac);
			xDelta = fast_15_16_lerp_prediff(xTableRead[phaseFrac + 1], morphFrac) - leftSample;
		}
		if (!yInterpolateOff) {
			ySample = getSampleQuinticSplineDeltaValue(yIndexBuffer[writeIndex], zIndex,
				(uint32_t *) yTable, &yDelta, yInterpolateOff);
		} else {
			phaseFrac = yIndexBuffer[writeIndex] >> 16;
			morphFrac = zIndex & 0xFFFF;
			leftSample = fast_15_16_lerp_prediff(yTableRead[phaseFrac], morphFrac);
			ySample = yValueHysterisis(leftSample, phaseFrac);
			yDelta = fast_15_16_lerp_prediff(yTableRead[phaseFrac + 1], morphFrac) - leftSample;
		}

		xIndexAtLogic = xIndexBuffer[writeIndex] >> 16;
		yIndexAtLogic = yIndexBuffer[writeIndex] >> 16;

		int32_t mainScan = (ySample > xSample) ? ySample >> 3 : xSample >> 3; //15 bit fixed point multiply and right shift by 3

		int32_t samplesRemaining = bufferSize;

		while (samplesRemaining) {
			altitude[writeIndex] = mainScan;
			locationBlend[writeIndex] = ((yIndexBuffer[writeIndex] > xIndexBuffer[writeIndex]) ? yIndexBuffer[writeIndex] : xIndexBuffer[writeIndex]) >> 13;

			writeIndex ++;
			samplesRemaining --;

		}

	} else {

		while (samplesRemaining) {

			leftSample = xIndexBuffer[writeIndex]>> 16;
			phaseFrac = (xIndexBuffer[writeIndex]) & 0xFFFF;
			morphFrac =  zIndex & 0xFFFF;

			xSample = fast_15_16_bilerp_prediff(xTableRead[leftSample], xTableRead[leftSample + 1], morphFrac, phaseFrac);

			leftSample = yIndexBuffer[writeIndex]>> 16;
			phaseFrac = (yIndexBuffer[writeIndex]) & 0xFFFF;

			ySample = fast_15_16_bilerp_prediff(yTableRead[leftSample], yTableRead[leftSample + 1], morphFrac, phaseFrac);

			altitude[writeIndex] = (ySample > xSample) ? ySample >> 3 : xSample >> 3;
			locationBlend[writeIndex] = ((yIndexBuffer[writeIndex] > xIndexBuffer[writeIndex]) ? yIndexBuffer[writeIndex] : xIndexBuffer[writeIndex]) >> 13;

			writeIndex ++;
			samplesRemaining --;

		}

		leftSample = xIndexBuffer[writeIndex] >> 16;
		xIndexAtLogic = leftSample;
		phaseFrac = (xIndexBuffer[writeIndex]) & 0xFFFF;
		morphFrac =  zIndex & 0xFFFF;

		xSample = fast_15_16_bilerp_prediff_deltaValue(xTableRead[leftSample], xTableRead[leftSample + 1], morphFrac, phaseFrac, &xDelta);

		leftSample = yIndexBuffer[writeIndex] >> 16;
		yIndexAtLogic = leftSample;
		phaseFrac = (yIndexBuffer[writeIndex]) & 0xFFFF;

		ySample = fast_15_16_bilerp_prediff_deltaValue(yTableRead[leftSample], yTableRead[leftSample + 1], morphFrac, phaseFrac, &yDelta);

		altitude[writeIndex] = (ySample > xSample) ? ySample >> 3 : xSample >> 3;
		locationBlend[writeIndex] = ((yIndexBuffer[writeIndex] > xIndexBuffer[writeIndex]) ? yIndexBuffer[writeIndex] : xIndexBuffer[writeIndex]) >> 13;

	}

	hemisphereXHysterisis(xSample);
	hemisphereYHysterisis(ySample);

	xDelta = (xDelta == 0) ? lastDeltaXState : ((uint32_t) xDelta >> 31);
	yDelta = (yDelta == 0) ? lastDeltaYState : ((uint32_t) yDelta >> 31);

	xDelta = deltaXHysterisis(xDelta, xIndexAtLogic);
	yDelta = deltaYHysterisis(yDelta, yIndexAtLogic);

	hemisphereBlend = (yHemisphere > xHemisphere);
	deltaBlend = (yDelta > xDelta);

}
