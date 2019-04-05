#include "scanner.hpp"


void ViaScanner::fillWavetableArray(void) {

	wavetableArray[0][0] = &wavetableSet.hyperbolic_shapers;
	wavetableArray[0][1] = &wavetableSet.bessel;
	wavetableArray[0][2] = &wavetableSet.newest_bounce;
	wavetableArray[0][3] = &wavetableSet.mountains;
	wavetableArray[0][4] = &wavetableSet.new_waves;
	wavetableArray[0][5] = &wavetableSet.test_fm;
	wavetableArray[0][6] = &wavetableSet.staircase;
	wavetableArray[0][7] = &wavetableSet.block_slope;
	wavetableArray[1][0] = &wavetableSet.circular_257;
	wavetableArray[1][1] = &wavetableSet.half_sines;
	wavetableArray[1][2] = &wavetableSet.rubberband;
	wavetableArray[1][3] = &wavetableSet.exciteBike;
	wavetableArray[1][4] = &wavetableSet.skipSaw;
	wavetableArray[1][5] = &wavetableSet.moog5Square;
	wavetableArray[1][6] = &wavetableSet.bitcrush;
	wavetableArray[1][7] = &wavetableSet.block_test;
}

constexpr Wavetable ScannerWavetableSet::skipSaw;
constexpr Wavetable ScannerWavetableSet::moog5Square;
constexpr Wavetable ScannerWavetableSet::hyperbolic_shapers;
constexpr Wavetable ScannerWavetableSet::newest_bounce;
constexpr Wavetable ScannerWavetableSet::test_fm;
constexpr Wavetable ScannerWavetableSet::staircase;
constexpr Wavetable ScannerWavetableSet::mountains;
constexpr Wavetable ScannerWavetableSet::block_slope;
constexpr Wavetable ScannerWavetableSet::block_test;
constexpr Wavetable ScannerWavetableSet::bessel;
constexpr Wavetable ScannerWavetableSet::rubberband;
constexpr Wavetable ScannerWavetableSet::bitcrush;
constexpr Wavetable ScannerWavetableSet::circular_257;
constexpr Wavetable ScannerWavetableSet::exciteBike;
constexpr Wavetable ScannerWavetableSet::new_waves;
constexpr Wavetable ScannerWavetableSet::half_sines;

constexpr uint16_t ScannerWavetableSet::moog5SquareShiftAttackFamily[5][257];

constexpr uint16_t ScannerWavetableSet::moog5SquareShiftReleaseFamily[5][257];

constexpr uint16_t ScannerWavetableSet::exciteBikeAttackFamily[5][257];

constexpr uint16_t ScannerWavetableSet::exciteBikeReleaseFamily[5][257];

constexpr uint16_t ScannerWavetableSet::skipsaw[5][257];

constexpr uint16_t ScannerWavetableSet::circular_257_slopes[4][257];

constexpr uint16_t ScannerWavetableSet::test_fm_attack[5][257];

constexpr uint16_t ScannerWavetableSet::test_fm_release[5][257];

constexpr uint16_t ScannerWavetableSet::hyperbolic_shapers_attack[5][257];

constexpr uint16_t ScannerWavetableSet::hyperbolic_shapers_release[5][257];

constexpr uint16_t ScannerWavetableSet::bitcrush_attack[5][257];

constexpr uint16_t ScannerWavetableSet::bitcrush_release[5][257];

constexpr uint16_t ScannerWavetableSet::bessel_attack[5][257];

constexpr uint16_t ScannerWavetableSet::bessel_release[5][257];

constexpr uint16_t ScannerWavetableSet::block_test_attack[5][257];

constexpr uint16_t ScannerWavetableSet::block_test_release[5][257];

constexpr uint16_t ScannerWavetableSet::half_sines_attack[5][257];

constexpr uint16_t ScannerWavetableSet::half_sines_release[5][257];

constexpr uint16_t ScannerWavetableSet::mountains_attack[4][257];

constexpr uint16_t ScannerWavetableSet::mountains_release[4][257];

constexpr uint16_t ScannerWavetableSet::rubberband_attacks[5][257];

constexpr uint16_t ScannerWavetableSet::newest_bounce_attack[5][257];

constexpr uint16_t ScannerWavetableSet::newest_bounce_release[5][257];

constexpr uint16_t ScannerWavetableSet::new_waves_attack[5][257];

constexpr uint16_t ScannerWavetableSet::new_waves_release[5][257];

constexpr uint16_t ScannerWavetableSet::block_slope_attack[5][257];

constexpr uint16_t ScannerWavetableSet::block_slope_release[5][257];

constexpr uint16_t ScannerWavetableSet::staircase_attack[5][257];

constexpr uint16_t ScannerWavetableSet::staircase_release[5][257];


// declare functions to set the currently active tables
void ViaScanner::switchWavetableX(const Wavetable * table) {
	wavetableSet.loadWavetableWithDiff15BitSlope(table, (uint32_t *) wavetableXRead);
	scanner.xTableSize = table->numWaveforms - 1;
}

// declare functions to set the currently active tables
void ViaScanner::switchWavetableY(const Wavetable * table) {
	wavetableSet.loadWavetableWithDiff15BitSlope(table, (uint32_t *) wavetableYRead);
	scanner.yTableSize = table->numWaveforms - 1;
}