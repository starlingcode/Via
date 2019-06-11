#include "sync.hpp"


void ViaSync::fillWavetableArray(void) {

	wavetableArray[	0][0] = &wavetableSet.additive_evens;
	wavetableArray[	0][1] = &wavetableSet.new_waves;
	wavetableArray[	0][2] = &wavetableSet.newest_bounce;
	wavetableArray[	0][3] = &wavetableSet.circular_257;
	wavetableArray[	1][0] = &wavetableSet.impevens;
	wavetableArray[	1][1] = &wavetableSet.additive_tri_to_pulse;
	wavetableArray[	1][2] = &wavetableSet.new_perlin;
	wavetableArray[	1][3] = &wavetableSet.csound_formants;
	wavetableArray[	2][0] = &wavetableSet.additive_pairs;
	wavetableArray[	2][1] = &wavetableSet.moogSquare;
	wavetableArray[	2][2] = &wavetableSet.test_fm;
	wavetableArray[	2][3] = &wavetableSet.trains_slim;
	wavetableArray[	3][0] = &wavetableSet.sharpExpoSymSlim;
	wavetableArray[	3][1] = &wavetableSet.gammaAsymSlim;
	wavetableArray[	3][2] = &wavetableSet.rhythm_steps;
	wavetableArray[	3][3] = &wavetableSet.block_test;
	wavetableArrayGlobal[0] = &wavetableSet.triOdd;
	wavetableArrayGlobal[1] = &wavetableSet.sinwavefold_257;
	wavetableArrayGlobal[2] = &wavetableSet.euc_odd_sharp;
	wavetableArrayGlobal[3] = &wavetableSet.skipSaw;
}

constexpr Wavetable SyncWavetableSet::csound_formants;
constexpr Wavetable SyncWavetableSet::moogSquare;
constexpr Wavetable SyncWavetableSet::impevens;
constexpr Wavetable SyncWavetableSet::circular_257;
constexpr Wavetable SyncWavetableSet::new_perlin;
constexpr Wavetable SyncWavetableSet::additive_evens;
constexpr Wavetable SyncWavetableSet::trains_slim;
constexpr Wavetable SyncWavetableSet::sharpExpoSymSlim;
constexpr Wavetable SyncWavetableSet::rhythm_steps;
constexpr Wavetable SyncWavetableSet::euc_odd_sharp;
constexpr Wavetable SyncWavetableSet::gammaAsymSlim;
constexpr Wavetable SyncWavetableSet::skipSaw;
constexpr Wavetable SyncWavetableSet::triOdd;
constexpr Wavetable SyncWavetableSet::newest_bounce;
constexpr Wavetable SyncWavetableSet::block_test;
constexpr Wavetable SyncWavetableSet::additive_pairs;
constexpr Wavetable SyncWavetableSet::new_waves;
constexpr Wavetable SyncWavetableSet::test_fm;
constexpr Wavetable SyncWavetableSet::sinwavefold_257;
constexpr Wavetable SyncWavetableSet::additive_tri_to_pulse;

constexpr uint16_t SyncWavetableSet::moogSquareShiftAttackFamily[9][257];

constexpr uint16_t SyncWavetableSet::moogSquareShiftReleaseFamily[9][257];

constexpr uint16_t SyncWavetableSet::trioddAttackFamily[9][257];

constexpr uint16_t SyncWavetableSet::impshort[9][257];

constexpr uint16_t SyncWavetableSet::skipsaw[5][257];

constexpr uint16_t SyncWavetableSet::sinwavefold_257_Family[9][257];

constexpr uint16_t SyncWavetableSet::additive_tri_to_pulseFamily[5][257];

constexpr uint16_t SyncWavetableSet::circular_257_slopes[4][257];

constexpr uint16_t SyncWavetableSet::test_fm_attack[5][257];

constexpr uint16_t SyncWavetableSet::test_fm_release[5][257];

constexpr uint16_t SyncWavetableSet::csound_formants_attack[9][257];

constexpr uint16_t SyncWavetableSet::csound_formants_release[9][257];

constexpr uint16_t SyncWavetableSet::new_perlin_slope[9][257];

constexpr uint16_t SyncWavetableSet::new_perlin_slope_2[9][257];

constexpr uint16_t SyncWavetableSet::additive_pairs_slopes[9][257];

constexpr uint16_t SyncWavetableSet::block_test_attack[9][257];

constexpr uint16_t SyncWavetableSet::block_test_release[9][257];

constexpr uint16_t SyncWavetableSet::additive_evens_attack[9][257];

constexpr uint16_t SyncWavetableSet::newest_bounce_attack[5][257];

constexpr uint16_t SyncWavetableSet::newest_bounce_release[5][257];

constexpr uint16_t SyncWavetableSet::euc_odd_sharp_attacks[5][257];

constexpr uint16_t SyncWavetableSet::euc_odd_sharp_releases[5][257];

constexpr uint16_t SyncWavetableSet::new_waves_attack[5][257];

constexpr uint16_t SyncWavetableSet::new_waves_release[5][257];

constexpr uint16_t SyncWavetableSet::rhythm_steps_attack[6][257];

constexpr uint16_t SyncWavetableSet::rhythm_steps_release[6][257];

constexpr uint16_t SyncWavetableSet::gammaAsymSlimAttackFamily[8][257];

constexpr uint16_t SyncWavetableSet::gammaAsymSlimReleaseFamily[8][257];

constexpr uint16_t SyncWavetableSet::sharpExpoSymSlimAttackFamily[8][257];

constexpr uint16_t SyncWavetableSet::trains_slim_attack[8][257];

constexpr uint16_t SyncWavetableSet::trains_slim_release[8][257];


// declare functions to set the currently active tables
void ViaSync::switchWavetable(const Wavetable * table) {
	wavetableSet.loadWavetableWithDiff(table, (uint32_t *) wavetableRead);
	syncWavetable.tableSize = table->numWaveforms - 1;
}

// declare functions to set the currently active tables
void ViaSync::switchWavetableGlobal(const Wavetable * table) {
	wavetableSet.loadWavetableWithDiff(table, (uint32_t *) wavetableRead);
	syncWavetable.tableSize = table->numWaveforms - 1;
}

//// declare functions to set the currently active tables
//void ViaSync::initPhaseDistTable(void) {
//	loadPhaseDistTable(&phaseDistPWM, phaseDistRead);
//}

