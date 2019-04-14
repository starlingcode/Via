#include "meta.hpp"


void ViaMeta::fillWavetableArray(void) {

	wavetableArray[0][0] = &wavetableSet.impevens;
	wavetableArray[0][1] = &wavetableSet.additive_pairs;
	wavetableArray[0][2] = &wavetableSet.linwavefold_257;
	wavetableArray[0][3] = &wavetableSet.skipSaw;
	wavetableArray[0][4] = &wavetableSet.new_perlin;
	wavetableArray[0][5] = &wavetableSet.csound_formants;
	wavetableArray[0][6] = &wavetableSet.vox;
	wavetableArray[0][7] = &wavetableSet.trains;
	wavetableArray[1][0] = &wavetableSet.gammaAsym;
	wavetableArray[1][1] = &wavetableSet.sharpLinSym;
	wavetableArray[1][2] = &wavetableSet.circular_257;
	wavetableArray[1][3] = &wavetableSet.quintic_outinAsym;
	wavetableArray[1][4] = &wavetableSet.fixed_lump;
	wavetableArray[1][5] = &wavetableSet.moving_lump;
	wavetableArray[1][6] = &wavetableSet.compressor;
	wavetableArray[1][7] = &wavetableSet.fake_adsr;
	wavetableArray[2][0] = &wavetableSet.new_waves;
	wavetableArray[2][1] = &wavetableSet.euc_odd_sharp;
	wavetableArray[2][2] = &wavetableSet.rubberband;
	wavetableArray[2][3] = &wavetableSet.newest_bounce;
	wavetableArray[2][4] = &wavetableSet.mountains;
	wavetableArray[2][5] = &wavetableSet.half_sines;
	wavetableArray[2][6] = &wavetableSet.newest_steps;
	wavetableArray[2][7] = &wavetableSet.block_test;
}

constexpr Wavetable MetaWavetableSet::trains;
constexpr Wavetable MetaWavetableSet::new_perlin;
constexpr Wavetable MetaWavetableSet::moving_lump;
constexpr Wavetable MetaWavetableSet::drum;
constexpr Wavetable MetaWavetableSet::quintic_outinAsym;
constexpr Wavetable MetaWavetableSet::skipSaw;
constexpr Wavetable MetaWavetableSet::linwavefold_257;
constexpr Wavetable MetaWavetableSet::compressor;
constexpr Wavetable MetaWavetableSet::additive_pairs;
constexpr Wavetable MetaWavetableSet::newest_steps;
constexpr Wavetable MetaWavetableSet::circular_257;
constexpr Wavetable MetaWavetableSet::new_waves;
constexpr Wavetable MetaWavetableSet::newest_bounce;
constexpr Wavetable MetaWavetableSet::gammaAsym;
constexpr Wavetable MetaWavetableSet::sharpLinSym;
constexpr Wavetable MetaWavetableSet::rubberband;
constexpr Wavetable MetaWavetableSet::fake_adsr;
constexpr Wavetable MetaWavetableSet::euc_odd_sharp;
constexpr Wavetable MetaWavetableSet::mountains;
constexpr Wavetable MetaWavetableSet::fixed_lump;
constexpr Wavetable MetaWavetableSet::csound_formants;
constexpr Wavetable MetaWavetableSet::vox;
constexpr Wavetable MetaWavetableSet::block_test;
constexpr Wavetable MetaWavetableSet::impevens;
constexpr Wavetable MetaWavetableSet::half_sines;

constexpr uint16_t MetaWavetableSet::gammaAsymAttackFamily[9][257];

constexpr uint16_t MetaWavetableSet::gammaAsymReleaseFamily[9][257];

constexpr uint16_t MetaWavetableSet::sharpLinSymAttackFamily[9][257];

constexpr uint16_t MetaWavetableSet::sharpLinSymReleaseFamily[9][257];

constexpr uint16_t MetaWavetableSet::impshort[9][257];

constexpr uint16_t MetaWavetableSet::skipsaw[5][257];

constexpr uint16_t MetaWavetableSet::linwavefold_257_Family[5][257];

constexpr uint16_t MetaWavetableSet::circular_257_slopes[4][257];

constexpr uint16_t MetaWavetableSet::quintic_outin2quintic_inout257_slopes[9][257];

constexpr uint16_t MetaWavetableSet::quintic_inout2quintic_outin257_slopes[9][257];

constexpr uint16_t MetaWavetableSet::trains_attack[9][257];

constexpr uint16_t MetaWavetableSet::trains_release[9][257];

constexpr uint16_t MetaWavetableSet::vox_attack[9][257];

constexpr uint16_t MetaWavetableSet::vox_release[9][257];

constexpr uint16_t MetaWavetableSet::csound_formants_attack[9][257];

constexpr uint16_t MetaWavetableSet::csound_formants_release[9][257];

constexpr uint16_t MetaWavetableSet::new_perlin_slope[9][257];

constexpr uint16_t MetaWavetableSet::new_perlin_slope_2[9][257];

constexpr uint16_t MetaWavetableSet::additive_pairs_slopes[9][257];

constexpr uint16_t MetaWavetableSet::block_test_attack[9][257];

constexpr uint16_t MetaWavetableSet::block_test_release[9][257];

constexpr uint16_t MetaWavetableSet::newest_steps_attack[9][257];

constexpr uint16_t MetaWavetableSet::newest_steps_release[9][257];

constexpr uint16_t MetaWavetableSet::allLinear[1][257];

constexpr uint16_t MetaWavetableSet::allExpo[1][257];

constexpr uint16_t MetaWavetableSet::compressor_attack[5][257];

constexpr uint16_t MetaWavetableSet::compressor_release[5][257];

constexpr uint16_t MetaWavetableSet::allLinear129_5[5][257];

constexpr uint16_t MetaWavetableSet::fixed_lump_slopes[5][257];

constexpr uint16_t MetaWavetableSet::moving_lump_slopes[5][257];

constexpr uint16_t MetaWavetableSet::allLinear129_9[9][257];

constexpr uint16_t MetaWavetableSet::fake_adsr_slopes[9][257];

constexpr uint16_t MetaWavetableSet::half_sines_attack[9][257];

constexpr uint16_t MetaWavetableSet::half_sines_release[9][257];

constexpr uint16_t MetaWavetableSet::mountains_attack[4][257];

constexpr uint16_t MetaWavetableSet::mountains_release[4][257];

constexpr uint16_t MetaWavetableSet::rubberband_attacks[9][257];

constexpr uint16_t MetaWavetableSet::newest_bounce_attack[5][257];

constexpr uint16_t MetaWavetableSet::newest_bounce_release[5][257];

constexpr uint16_t MetaWavetableSet::euc_odd_sharp_attacks[5][257];

constexpr uint16_t MetaWavetableSet::euc_odd_sharp_releases[5][257];

constexpr uint16_t MetaWavetableSet::new_waves_attack[5][257];

constexpr uint16_t MetaWavetableSet::new_waves_release[5][257];


// declare functions to set the currently active tables
void ViaMeta::switchWavetable(const Wavetable * table) {
	wavetableSet.loadWavetableWithDiff15Bit(table, (uint32_t *) wavetableRead);
	metaWavetable.tableSize = table->numWaveforms - 1;
}

// declare functions to set the currently active tables
void ViaMeta::initDrum(void) {
	wavetableSet.loadSingleTable15Bit(&wavetableSet.drum, (uint32_t *) wavetableReadDrum);
	for (int32_t i = 0; i < 4; i++) {
		drumFullScale[i] = 32767;
	}
}