/*
 * tables.h
 *
 *  Created on: Aug 18, 2018
 *      Author: willmitchell
 */

#ifndef INC_TABLES_H_
#define INC_TABLES_H_

/// Wavetable data struct storing a wavetable as two halves, an attack and release, generated with (link)
class Wavetable {
public:
    /// Array of attack slopes, each an array of 16 bit sample values.
	const uint16_t (*attackSlope)[257];
	/// Array of release slopes, each an array of 16 bit sample values stored in reverse time order.
	const uint16_t (*releaseSlope)[257];
	/// Length in samples of each slope.
	uint32_t slopeLength;
	/// Number of slopes in the arrays.
	uint32_t numWaveforms;
};


class WavetableSet {

public:

	/// Load each sample from flash to ram with bitshift by 3 and store in bottom halfword.
	/// Store difference between sample and corresponding sample in adjacent waveform in top halfword

	void loadWavetableWithDiff(const Wavetable * table,
			uint32_t * tableRead) {

		uint32_t numSamples = table->slopeLength;

		//for each table in the table
		for (uint32_t i = 0; i < table->numWaveforms; i++) {
			//include the "last two" samples from release
			*((tableRead + 517 * i) + 0) = *(*(table->releaseSlope + i) + 0) >> 3;
			*((tableRead + 517 * i) + 1) = *(*(table->releaseSlope + i) + 0) >> 3;
			//fill in a full cycle's worth of samples
			//the release gets reversed
			//we drop the last sample from attack and the first from releas
			for (uint32_t j = 0; j < numSamples; j++) {
				*((tableRead + 517 * i) + 2 + j) = *(*(table->attackSlope + i) + j)
						>> 3;
				*((tableRead + 517 * i) + 2 + numSamples + j) =
						*(*(table->releaseSlope + i) + numSamples - j) >> 3;
			}
			//pad out the "first two" samples from attack
			*((tableRead + 517 * i) + (numSamples << 1) + 2) =
					*(*(table->attackSlope + i) + 0) >> 3;
			*((tableRead + 517 * i) + (numSamples << 1) + 3) =
					*(*(table->attackSlope + i) + 0) >> 3;
			*((tableRead + 517 * i) + (numSamples << 1) + 4) =
					*(*(table->attackSlope + i) + 0) >> 3;
		}

		for (uint32_t i = 0; i < table->numWaveforms - 1; i++) {
			for (uint32_t j = 0; j < (numSamples * 2 + 5); j++) {
				*((tableRead + 517 * i) + j) |= (*((tableRead + 517 * (i + 1)) + j)
						- *((tableRead + 517 * i) + j)) << 16;
			}
		}

	}

	/// Load each sample from flash to ram at full precision and store in bottom halfword.
	/// Store difference between sample and corresponding sample in adjacent waveform in top halfword

	void loadWavetableWithDiff15Bit(const Wavetable * table,
			uint32_t * tableRead) {

		uint32_t numSamples = table->slopeLength;

		//for each table in the table
		for (uint32_t i = 0; i < table->numWaveforms; i++) {
			//pad with duplicate samples
			*((tableRead + 517 * i) + 0) = *(*(table->releaseSlope + i) + 0);
			*((tableRead + 517 * i) + 1) = *(*(table->releaseSlope + i) + 0);
			//fill in a full cycle's worth of samples
			//the release gets reversed
			//we drop the last sample from attack and the first from releas
			for (uint32_t j = 0; j < numSamples; j++) {
				*((tableRead + 517 * i) + 2 + j) = *(*(table->attackSlope + i) + j);
				*((tableRead + 517 * i) + 2 + numSamples + j) =
						*(*(table->releaseSlope + i) + numSamples - j);
			}
			//pad with duplicate samples
			*((tableRead + 517 * i) + (numSamples << 1) + 2) =
					*(*(table->attackSlope + i) + 0);
			*((tableRead + 517 * i) + (numSamples << 1) + 3) =
					*(*(table->attackSlope + i) + 0);
			*((tableRead + 517 * i) + (numSamples << 1) + 4) =
					*(*(table->attackSlope + i) + 0);
		}

		for (uint32_t i = 0; i < table->numWaveforms - 1; i++) {
			for (uint32_t j = 0; j < (numSamples * 2 + 5); j++) {
				*((tableRead + 517 * i) + j) |= (*((tableRead + 517 * (i + 1)) + j)
						- *((tableRead + 517 * i) + j)) << 16;
			}
		}

	}

	/// Load each sample from flash to ram at full precision and store in bottom halfword.
	/// Store difference between sample and corresponding sample in adjacent waveform in top halfword

	void loadWavetableWithDiff15BitSlope(const Wavetable * table,
			uint32_t * tableRead) {

		uint32_t numSamples = table->slopeLength;

		//for each table in the table
		for (uint32_t i = 0; i < table->numWaveforms; i++) {
			//pad with duplicate samples
			*((tableRead + 517 * i) + 0) = *(*(table->releaseSlope + i) + 0);
			*((tableRead + 517 * i) + 1) = *(*(table->releaseSlope + i) + 0);
			//fill in a full cycle's worth of samples
			//the release gets reversed
			//we drop the last sample from attack and the first from releas
			for (uint32_t j = 0; j < numSamples; j++) {
				*((tableRead + 517 * i) + 2 + j) = *(*(table->attackSlope + i) + j);
				*((tableRead + 517 * i) + 2 + numSamples + j) =
						*(*(table->releaseSlope + i) + numSamples - j);
			}
			//pad with duplicate samples
			*((tableRead + 517 * i) + (numSamples << 1) + 2) =
					*((tableRead + 517 * i) + (numSamples << 1) + 1);
			*((tableRead + 517 * i) + (numSamples << 1) + 3) =
					*((tableRead + 517 * i) + (numSamples << 1) + 1);
			*((tableRead + 517 * i) + (numSamples << 1) + 4) =
					*((tableRead + 517 * i) + (numSamples << 1) + 1);
		}

		for (uint32_t i = 0; i < table->numWaveforms - 1; i++) {
			for (uint32_t j = 0; j < (numSamples * 2 + 5); j++) {
				*((tableRead + 517 * i) + j) |= (*((tableRead + 517 * (i + 1)) + j)
						- *((tableRead + 517 * i) + j)) << 16;
			}
		}

	}



	void loadSingleTable15Bit(const Wavetable * table,
			uint32_t * tableRead) {

		uint32_t numSamples = table->slopeLength;

		//for each table in the table
		for (uint32_t i = 0; i < 1; i++) {
			//pad with duplicate samples
			*((tableRead + 517 * i) + 0) = *(*(table->releaseSlope + i) + 0);
			*((tableRead + 517 * i) + 1) = *(*(table->releaseSlope + i) + 0);
			//fill in a full cycle's worth of samples
			//the release gets reversed
			//we drop the last sample from attack and the first from releas
			for (uint32_t j = 0; j < numSamples; j++) {
				*((tableRead + 517 * i) + 2 + j) = *(*(table->attackSlope + i) + j);
				*((tableRead + 517 * i) + 2 + numSamples + j) =
						*(*(table->releaseSlope + i) + numSamples - j);
			}
			//pad with duplicate samples
			*((tableRead + 517 * i) + (numSamples << 1) + 2) =
					*(*(table->attackSlope + i) + 0);
			*((tableRead + 517 * i) + (numSamples << 1) + 3) =
					*(*(table->attackSlope + i) + 0);
			*((tableRead + 517 * i) + (numSamples << 1) + 4) =
					*(*(table->attackSlope + i) + 0);
		}

	}

};

#endif /* INC_TABLES_H_ */
