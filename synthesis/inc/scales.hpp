/*
 * scales.h
 *
 *  Created on: Aug 18, 2018
 *      Author: willmitchell
 */

#ifndef INC_SCALES_HPP_
#define INC_SCALES_HPP_


/// Data structure for a note in a scale as represented by a precalculated ratio of integers. Generated with (link).
typedef struct {
    /// Top 32 bits of a 64 bit precalculated ratio.
	const uint32_t integerPart;
	/// Bottom 32 bits of a 64 bit precalculated ratio.
	const uint32_t fractionalPart;
	/// GCD of the numerator and denominator.
	const uint32_t fundamentalDivision;
} ScaleNote;


/// Data structure for a grid of scale notes. Generated with (link).
typedef struct {
    /// An array of scales (an array of pointers to notes).
	const ScaleNote* const** grid;
	/// 12 - log2(number of scales) used to scale a control to an lookup index.
	const uint32_t t2Bitshift;
	/// Use 1v_oct control parsing if 1.
	const uint32_t oneVoltOct;
} Scale;


#endif /* INC_SCALES_HPP_ */
