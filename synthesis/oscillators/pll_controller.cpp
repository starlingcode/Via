/*
 * oscillator_resources.c
 *
 *  Created on: Aug 26, 2018
 *      Author: willmitchell
 */

#include <oscillators.hpp>


/*
 *
 * Pll with scale grid multiplier
 *
 */


void PllController::parseControls(ViaControls * controls, ViaInputStreams * input) {

	int32_t ratioX = controls->knob1Value + controls->cv1Value - cv1Offset - 2048;
	ratioX = __USAT(ratioX, 12);

	ratioX = ratioXHysterisis(ratioX >> 5, ratioX);

	int32_t rootModLocal = -rootMod[0];
	rootModLocal += cv2Offset;
	rootModLocal = rootModLocal >> 4;
	rootModLocal += controls->knob2Value;
	int32_t	ratioY = __USAT(rootModLocal, 12);

	ratioY = ratioYHysterisis(ratioY, scale->t2Bitshift);

	if (ratioY != lastYIndex) {
		yIndexChange = 1;
	}

	lastYIndex = ratioY;

	fracMultiplier = scale->grid[ratioY][ratioX]->fractionalPart;
	intMultiplier = scale->grid[ratioY][ratioX]->integerPart;
	gcd = scale->grid[ratioY][ratioX]->fundamentalDivision;

}
void PllController::doPLL(void) {

	pllCounter++;
	pllCounter = (pllCounter >= gcd) ? 0 : pllCounter;


	pllCounter *= pllReset;
	pllReset = 1;
	phaseReset = 1;

	int32_t localPhaseOffset = fix48_mul(phaseOffset, fracMultiplier) + fix16_mul(phaseOffset, intMultiplier);

	uint32_t phase = phaseSignal - phaseModSignal + (localPhaseOffset << 7);

	uint32_t span = ((int64_t)intMultiplier << 16) | (fracMultiplier >> 16);

	uint32_t target = (int64_t)pllCounter * span;

	int32_t error = (phase - target);

	error *= -1;

//	errorSig = __USAT((error >> 20) + 2048, 12);
//	errorSig = __USAT(((periodCount - lastIncrement) >> 8) + 2048, 12);
//	lastIncrement = periodCount;

#define IGNORE 0
#define SLOW_PLL 1
#define FAST_PLL 2
#define WILD_PLL 3
#define HARD_SYNC 4

	int32_t pTerm;
	int32_t iTerm;
	int32_t dTerm;

	switch (syncMode) {
		case IGNORE:
			break;
		case SLOW_PLL:

			nudgeSum = error + nudgeSum - readBuffer(&nudgeBuffer, 31);
			writeBuffer(&nudgeBuffer, error);
			pTerm = error;
			iTerm = nudgeSum >> 5;
			dTerm = (error - readBuffer(&nudgeBuffer, 3));

			pllNudge = (pTerm + iTerm + dTerm) >> 4;

			break;
		case FAST_PLL:

			nudgeSum = error + nudgeSum - readBuffer(&nudgeBuffer, 7);
			writeBuffer(&nudgeBuffer, error);
			pTerm = error;
			iTerm = nudgeSum >> 3;

			pllNudge = (pTerm + iTerm) >> 2;

			break;
		case WILD_PLL:

			pTerm = error;
			nudgeSum = error + nudgeSum - readBuffer(&nudgeBuffer, 7);
			writeBuffer(&nudgeBuffer, error);
			iTerm = nudgeSum >> 3;
			pllNudge = pTerm;

			break;
		case HARD_SYNC:

			pllNudge = 0;
			nudgeSum = 0;
			phaseSignal = (localPhaseOffset << 7) + (phaseModSignal << 7) + target;

			break;

		default:
			break;

	}

	int32_t multKey = fracMultiplier + intMultiplier;

	ratioChange = (lastMultiplier != multKey);

	lastMultiplier = multKey;

}

void PllController::generateFrequency(void) {

#ifdef BUILD_F373

	int64_t incrementCalc = ((int64_t)intMultiplier << 16) | (fracMultiplier >> 16);
	incrementCalc = ((uint64_t)(incrementCalc + pllNudge) * 1440) / (periodCount * 8);
	increment = __USAT(incrementCalc, 31);

#endif

#ifdef BUILD_VIRTUAL

	if (periodCount == 0) {
		periodCount = 1;
	}

	uint64_t incrementCalc = ((uint64_t)intMultiplier << 16) | (fracMultiplier >> 16);
	incrementCalc = ((uint64_t)(incrementCalc + pllNudge)) / (periodCount * 8);
	increment = incrementCalc;

#endif

}
