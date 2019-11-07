/*
 * oscillator_resources.c
 *
 *  Created on: Aug 26, 2018
 *      Author: willmitchell
 */

#include <sync.hpp>


/*
 *
 * Pll with scale grid multiplier
 *
 */


void ViaSync::parseControls(ViaControls * controls, ViaInputStreams * input) {

	int32_t ratioX = controls->knob1Value + controls->cv1Value - cv1Offset - 2048;
	ratioX = __USAT(ratioX, 12);

	ratioX = ratioXHysterisis(ratioX >> 5, ratioX);

	int32_t rootModLocal = -rootMod[0];
	rootModLocal += cv2Offset;
	rootModLocal = rootModLocal >> 4;
	rootModLocal += controls->knob2Value;
	int32_t	ratioY = __USAT(rootModLocal, 12);

	ratioY = ratioYHysterisis(ratioY, selectedScale->t2Bitshift);

	if (ratioY != lastYIndex) {
		yIndexChange = 1;
	}

	lastYIndex = ratioY;

	fracMultiplier = selectedScale->grid[ratioY][ratioX]->fractionalPart;
	intMultiplier = selectedScale->grid[ratioY][ratioX]->integerPart;
	gcd = selectedScale->grid[ratioY][ratioX]->fundamentalDivision;

}
void ViaSync::doPLL(void) {

	pllCounter += clockDiv;
	pllCounter = (pllCounter >= gcd) ? 0 : pllCounter;


	pllCounter *= pllReset;
	pllReset = 1;
	phaseReset = 1;

	int32_t localPhaseOffset = fix48_mul(phaseOffset, fracMultiplier) + fix16_mul(phaseOffset, intMultiplier);

	uint32_t phase = phaseSignal - syncWavetable.phaseMod + (localPhaseOffset << 7);

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

	if (syncMode == 0) {

		nudgeSum = error + nudgeSum - readBuffer(&nudgeBuffer, 31);
		writeBuffer(&nudgeBuffer, error);
		pTerm = error;
		iTerm = nudgeSum >> 5;
		dTerm = (error - readBuffer(&nudgeBuffer, 3));

		pllNudge = (pTerm + iTerm + dTerm) >> 4;

	} else if (syncMode == 1) {

		nudgeSum = error + nudgeSum - readBuffer(&nudgeBuffer, 7);
		writeBuffer(&nudgeBuffer, error);
		pTerm = error;
		iTerm = nudgeSum >> 3;

		pllNudge = (pTerm + iTerm) >> 2;

	} else if (syncMode == 2) {

		pTerm = error;
		nudgeSum = error + nudgeSum - readBuffer(&nudgeBuffer, 7);
		writeBuffer(&nudgeBuffer, error);
		iTerm = nudgeSum >> 3;
		pllNudge = pTerm;

	} else if (syncMode == 3) {

		pllNudge = 0;
		nudgeSum = 0;
		syncWavetable.phase = (localPhaseOffset << 7) + (phaseModSignal << 7) + target;

	}

}

void ViaSync::generateFrequency(void) {

#ifdef BUILD_VIRTUAL

	if (periodCount == 0) {
		periodCount = 48000;
	}

#endif


	int64_t incrementCalc = ((int64_t)intMultiplier << 16) | (fracMultiplier >> 16);
	incrementCalc = ((int64_t) (incrementCalc * (int64_t)clockDiv + (int64_t)pllNudge) * 1440) / (periodCount * 8);
	increment = __USAT(incrementCalc, 31);

}
