#include <gateseq.hpp>

void ViaGateseq::initializePatterns() {
   seq1Banks[0] = &sequencer1banknoclock_bank;
   seq1Banks[1] = &sequencer1banktriplet_bank;
   seq1Banks[2] = &sequencer1bankshuffle_bank;
   seq1Banks[3] = &sequencer1bankmultiplier_bank;
   seq2Banks[0] = &sequencer2bank1_bank;
   seq2Banks[1] = &sequencer2bank2_bank;
   seq2Banks[2] = &sequencer2bank3_bank;
   seq2Banks[3] = &sequencer2bank4_bank;
}
