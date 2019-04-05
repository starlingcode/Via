#ifndef SEQUENCERS_INC_BOOLEAN_SEQUENCES_H_
#define SEQUENCERS_INC_BOOLEAN_SEQUENCES_H_

/// Sequence grid data type, generated using (link).
typedef struct {
    /// Array of sequences, each an array of values
	const uint32_t **patternBank;
	/// Array of lengths for each sequence
	const uint32_t *lengths;
	/// number of sequences in the array
	const uint32_t numPatterns;
} booleanSequenceBank;


static const uint32_t sequencer2bank2_7_16[16] = {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer1banktriplet_24_24[24] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 

static const uint32_t sequencer1banktriplet_7_24[24] = {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0}; 

static const uint32_t sequencer1banktriplet_19_24[24] = {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer2bank2_5_8[8] = {1, 0, 1, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer1banktriplet_16_24[24] = {1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer1banknoclock_16_16[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 

static const uint32_t sequencer2bank1_4_16[16] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}; 

static const uint32_t sequencer2bank3_3_17[17] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}; 

static const uint32_t sequencer1banknoclock_15_16[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer1banktriplet_10_24[24] = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer1banknoclock_10_16[16] = {1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer1bankshuffle_29_32[32] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer1bankmultiplier_1_16[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank3_3_13[13] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank3_2_2[2] = {1, 1}; 

static const uint32_t sequencer1banknoclock_8_16[16] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}; 

static const uint32_t sequencer1banknoclock_12_16[16] = {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0}; 

static const uint32_t sequencer1bankshuffle_15_32[32] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer1banktriplet_13_24[24] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer1bankshuffle_27_32[32] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer1banktriplet_2_24[24] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1bankshuffle_23_32[32] = {1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1}; 

static const uint32_t sequencer2bank3_3_7[7] = {1, 0, 1, 0, 1, 0, 0}; 

static const uint32_t sequencer1bankshuffle_19_32[32] = {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer1banknoclock_7_16[16] = {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer1bankshuffle_9_32[32] = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0}; 

static const uint32_t sequencer1banknoclock_9_16[16] = {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer2bank1_8_16[16] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}; 

static const uint32_t sequencer2bank1_9_16[16] = {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer2bank2_1_12[12] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banknoclock_11_16[16] = {1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1}; 

static const uint32_t sequencer2bank2_1_1[1] = {1};

static const uint32_t sequencer2bank3_2_13[13] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banktriplet_8_24[24] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}; 

static const uint32_t sequencer2bank2_1_4[4] = {1, 0, 0, 0}; 

static const uint32_t sequencer2bank2_7_8[8] = {1, 1, 1, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer2bank1_13_16[16] = {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1}; 

static const uint32_t sequencer2bank3_2_9[9] = {1, 0, 0, 0, 1, 0, 0, 0, 0}; 

static const uint32_t sequencer1bankshuffle_32_32[32] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 

static const uint32_t sequencer1bankmultiplier_1_3[3] = {1, 0, 0}; 

static const uint32_t sequencer2bank4_1_6[6] = {1, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank2_3_8[8] = {1, 0, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer1bankshuffle_11_32[32] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer2bank2_5_16[16] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0}; 

static const uint32_t sequencer1bankmultiplier_1_12[12] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banknoclock_2_16[16] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banknoclock_3_16[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank1_5_16[16] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0}; 

static const uint32_t sequencer2bank1_1_16[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank1_0_16[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banknoclock_6_16[16] = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer1banktriplet_14_24[24] = {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer2bank1_16_16[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 

static const uint32_t sequencer2bank3_3_11[11] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}; 

static const uint32_t sequencer1bankmultiplier_1_6[6] = {1, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank2_1_16[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1bankmultiplier_1_4[4] = {1, 0, 0, 0}; 

static const uint32_t sequencer1bankmultiplier_1_2[2] = {1, 0}; 

static const uint32_t sequencer2bank3_3_9[9] = {1, 0, 0, 1, 0, 0, 1, 0, 0}; 

static const uint32_t sequencer1bankshuffle_13_32[32] = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer2bank4_1_2[2] = {1, 0}; 

static const uint32_t sequencer1banknoclock_14_16[16] = {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer2bank1_11_16[16] = {1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1}; 

static const uint32_t sequencer2bank3_2_5[5] = {1, 0, 1, 0, 0}; 

static const uint32_t sequencer2bank2_7_12[12] = {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer1bankmultiplier_1_8[8] = {1, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banknoclock_4_16[16] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}; 

static const uint32_t sequencer2bank1_7_16[16] = {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer2bank1_10_16[16] = {1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer2bank3_2_4[4] = {1, 0, 1, 0}; 

static const uint32_t sequencer1bankshuffle_17_32[32] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer1bankmultiplier_2_2[2] = {1, 1}; 

static const uint32_t sequencer1banknoclock_0_16[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank3_3_5[5] = {1, 0, 1, 0, 1}; 

static const uint32_t sequencer2bank2_3_4[4] = {1, 1, 1, 0}; 

static const uint32_t sequencer1banknoclock_13_16[16] = {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1}; 

static const uint32_t sequencer1bankshuffle_0_32[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank4_2_2[2] = {1, 1}; 

static const uint32_t sequencer2bank4_1_4[4] = {1, 0, 0, 0}; 

static const uint32_t sequencer2bank2_0_2[2] = {0, 0}; 

static const uint32_t sequencer2bank4_1_12[12] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank4_1_3[3] = {1, 0, 0}; 

static const uint32_t sequencer2bank1_14_16[16] = {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer1banktriplet_17_24[24] = {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0}; 

static const uint32_t sequencer2bank3_3_4[4] = {1, 1, 1, 0}; 

static const uint32_t sequencer1bankmultiplier_0_2[2] = {0, 0}; 

static const uint32_t sequencer2bank3_0_2[2] = {0, 0}; 

static const uint32_t sequencer1bankshuffle_7_32[32] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank4_0_2[2] = {0, 0}; 

static const uint32_t sequencer2bank3_2_17[17] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banktriplet_20_24[24] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer2bank2_1_2[2] = {1, 0}; 

static const uint32_t sequencer1bankshuffle_5_32[32] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banktriplet_4_24[24] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank4_1_8[8] = {1, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank2_5_12[12] = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer2bank1_2_16[16] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banknoclock_5_16[16] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0}; 

static const uint32_t sequencer2bank1_12_16[16] = {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0}; 

static const uint32_t sequencer2bank3_2_7[7] = {1, 0, 0, 1, 0, 0, 0}; 

static const uint32_t sequencer1banktriplet_22_24[24] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}; 

static const uint32_t sequencer1banktriplet_0_24[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1banktriplet_5_24[24] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0}; 

static const uint32_t sequencer1bankshuffle_21_32[32] = {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0}; 

static const uint32_t sequencer1banktriplet_11_24[24] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer2bank3_2_11[11] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank2_3_16[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank2_1_8[8] = {1, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank1_3_16[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer2bank1_6_16[16] = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0}; 

static const uint32_t sequencer1bankshuffle_3_32[32] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

static const uint32_t sequencer1bankshuffle_25_32[32] = {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1}; 

static const uint32_t sequencer2bank4_1_16[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 




//////////////////////////////////////////////////////// 

// Banks 
//////////////////////////////////////////////////////// 

static const uint32_t *sequencer1banknoclock[16] = {sequencer1banknoclock_0_16, sequencer1banknoclock_2_16, sequencer1banknoclock_3_16, sequencer1banknoclock_4_16, sequencer1banknoclock_5_16, sequencer1banknoclock_6_16, sequencer1banknoclock_7_16, sequencer1banknoclock_8_16, sequencer1banknoclock_9_16, sequencer1banknoclock_10_16, sequencer1banknoclock_11_16, sequencer1banknoclock_12_16, sequencer1banknoclock_13_16, sequencer1banknoclock_14_16, sequencer1banknoclock_15_16, sequencer1banknoclock_16_16}; 

static const uint32_t sequencer1banknoclockLengths[16] = {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}; 

static const booleanSequenceBank sequencer1banknoclock_bank = {
   .patternBank = sequencer1banknoclock,
   .lengths = sequencer1banknoclockLengths,
   .numPatterns = 16};
//////////////////////////////////////////////////////// 

static const uint32_t *sequencer1banktriplet[16] = {sequencer1banktriplet_0_24, sequencer1banktriplet_2_24, sequencer1banktriplet_4_24, sequencer1banktriplet_5_24, sequencer1banktriplet_7_24, sequencer1banktriplet_8_24, sequencer1banktriplet_10_24, sequencer1banktriplet_11_24, sequencer1banktriplet_13_24, sequencer1banktriplet_14_24, sequencer1banktriplet_16_24, sequencer1banktriplet_17_24, sequencer1banktriplet_19_24, sequencer1banktriplet_20_24, sequencer1banktriplet_22_24, sequencer1banktriplet_24_24}; 

static const uint32_t sequencer1banktripletLengths[16] = {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24}; 

static const booleanSequenceBank sequencer1banktriplet_bank = {
   .patternBank = sequencer1banktriplet,
   .lengths = sequencer1banktripletLengths,
   .numPatterns = 16};
//////////////////////////////////////////////////////// 

static const uint32_t *sequencer1bankshuffle[16] = {sequencer1bankshuffle_0_32, sequencer1bankshuffle_3_32, sequencer1bankshuffle_5_32, sequencer1bankshuffle_7_32, sequencer1bankshuffle_9_32, sequencer1bankshuffle_11_32, sequencer1bankshuffle_13_32, sequencer1bankshuffle_15_32, sequencer1bankshuffle_17_32, sequencer1bankshuffle_19_32, sequencer1bankshuffle_21_32, sequencer1bankshuffle_23_32, sequencer1bankshuffle_25_32, sequencer1bankshuffle_27_32, sequencer1bankshuffle_29_32, sequencer1bankshuffle_32_32}; 

static const uint32_t sequencer1bankshuffleLengths[16] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}; 

static const booleanSequenceBank sequencer1bankshuffle_bank = {
   .patternBank = sequencer1bankshuffle,
   .lengths = sequencer1bankshuffleLengths,
   .numPatterns = 16};
//////////////////////////////////////////////////////// 

static const uint32_t *sequencer1bankmultiplier[16] = {sequencer1bankmultiplier_0_2, sequencer1bankmultiplier_1_16, sequencer1bankmultiplier_1_16, sequencer1bankmultiplier_1_12, sequencer1bankmultiplier_1_12, sequencer1bankmultiplier_1_8, sequencer1bankmultiplier_1_8, sequencer1bankmultiplier_1_6, sequencer1bankmultiplier_1_6, sequencer1bankmultiplier_1_4, sequencer1bankmultiplier_1_4, sequencer1bankmultiplier_1_3, sequencer1bankmultiplier_1_3, sequencer1bankmultiplier_1_2, sequencer1bankmultiplier_1_2, sequencer1bankmultiplier_2_2}; 

static const uint32_t sequencer1bankmultiplierLengths[16] = {2, 16, 16, 12, 12, 8, 8, 6, 6, 4, 4, 3, 3, 2, 2, 2}; 

static const booleanSequenceBank sequencer1bankmultiplier_bank = {
   .patternBank = sequencer1bankmultiplier,
   .lengths = sequencer1bankmultiplierLengths,
   .numPatterns = 16};
//////////////////////////////////////////////////////// 

static const uint32_t *sequencer2bank1[16] = {sequencer2bank1_0_16, sequencer2bank1_1_16, sequencer2bank1_2_16, sequencer2bank1_3_16, sequencer2bank1_4_16, sequencer2bank1_5_16, sequencer2bank1_6_16, sequencer2bank1_7_16, sequencer2bank1_8_16, sequencer2bank1_9_16, sequencer2bank1_10_16, sequencer2bank1_11_16, sequencer2bank1_12_16, sequencer2bank1_13_16, sequencer2bank1_14_16, sequencer2bank1_16_16}; 

static const uint32_t sequencer2bank1Lengths[16] = {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}; 

static const booleanSequenceBank sequencer2bank1_bank = {
   .patternBank = sequencer2bank1,
   .lengths = sequencer2bank1Lengths,
   .numPatterns = 16};
//////////////////////////////////////////////////////// 

static const uint32_t *sequencer2bank2[16] = {sequencer2bank2_0_2, sequencer2bank2_1_16, sequencer2bank2_1_12, sequencer2bank2_1_8, sequencer2bank2_3_16, sequencer2bank2_1_4, sequencer2bank2_5_16, sequencer2bank2_3_8, sequencer2bank2_5_12, sequencer2bank2_7_16, sequencer2bank2_1_2, sequencer2bank2_7_12, sequencer2bank2_5_8, sequencer2bank2_3_4, sequencer2bank2_7_8, sequencer2bank2_1_1}; 

static const uint32_t sequencer2bank2Lengths[16] = {2, 16, 12, 8, 16, 4, 16, 8, 12, 16, 2, 12, 8, 4, 8, 1}; 

static const booleanSequenceBank sequencer2bank2_bank = {
   .patternBank = sequencer2bank2,
   .lengths = sequencer2bank2Lengths,
   .numPatterns = 16};
//////////////////////////////////////////////////////// 

static const uint32_t *sequencer2bank3[16] = {sequencer2bank3_0_2, sequencer2bank3_2_17, sequencer2bank3_2_13, sequencer2bank3_3_17, sequencer2bank3_2_11, sequencer2bank3_2_9, sequencer2bank3_3_13, sequencer2bank3_3_11, sequencer2bank3_2_7, sequencer2bank3_3_9, sequencer2bank3_2_5, sequencer2bank3_3_7, sequencer2bank3_2_4, sequencer2bank3_3_5, sequencer2bank3_3_4, sequencer2bank3_2_2}; 

static const uint32_t sequencer2bank3Lengths[16] = {2, 17, 13, 17, 11, 9, 13, 11, 7, 9, 5, 7, 4, 5, 4, 2}; 

static const booleanSequenceBank sequencer2bank3_bank = {
   .patternBank = sequencer2bank3,
   .lengths = sequencer2bank3Lengths,
   .numPatterns = 16};
//////////////////////////////////////////////////////// 

static const uint32_t *sequencer2bank4[16] = {sequencer2bank4_0_2, sequencer2bank4_1_16, sequencer2bank4_1_16, sequencer2bank4_1_12, sequencer2bank4_1_12, sequencer2bank4_1_8, sequencer2bank4_1_8, sequencer2bank4_1_6, sequencer2bank4_1_6, sequencer2bank4_1_4, sequencer2bank4_1_4, sequencer2bank4_1_3, sequencer2bank4_1_3, sequencer2bank4_1_2, sequencer2bank4_1_2, sequencer2bank4_2_2}; 

static const uint32_t sequencer2bank4Lengths[16] = {2, 16, 16, 12, 12, 8, 8, 6, 6, 4, 4, 3, 3, 2, 2, 2}; 

static const booleanSequenceBank sequencer2bank4_bank = {
   .patternBank = sequencer2bank4,
   .lengths = sequencer2bank4Lengths,
   .numPatterns = 16};
//////////////////////////////////////////////////////// 

#endif