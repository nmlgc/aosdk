/*
 * Audio Overload SDK
 *
 * Wave dumping
 *
 * Author: Nmlgc
 */

#include <assert.h>
#include <stdio.h>
#include "ao.h"
#include "utils.h"
#include "wavedump.h"

typedef struct {
	uint32 FOURCC;
	uint32 Size;
} RIFFCHUNK;

// This is pretty much identical to the WAVEFORMAT+PCMWAVEFORMAT structures
// from <windows.h>, which we can't use because they're stupidly declared in
// such a way as to come with 2 padding bytes before and after
// PCMWAVEFORMAT.wBitsPerSample.

typedef struct {
	uint16 wFormatTag; // format type
	uint16 nChannels; // number of channels (i.e. mono, stereo, etc.)
	uint32 nSamplesPerSec; // sample rate
	uint32 nAvgBytesPerSec; // for buffer estimation
	uint16 nBlockAlign; // block size of data
	uint16 wBitsPerSample;
} WAVEFORMAT_PCM;

// flags for wFormatTag field of WAVEFORMAT
#define WAVE_FORMAT_PCM 1

typedef struct {
	RIFFCHUNK cRIFF;
	uint32 WAVE;
	RIFFCHUNK cfmt;
	WAVEFORMAT_PCM Format;
	RIFFCHUNK cdata;
} WAVEHEADER;

static void wavedump_header_fill(
	WAVEHEADER *h, uint32 data_size,
	uint32 sample_rate, uint16 bits_per_sample, uint16 channels
)
{
	h->cRIFF.FOURCC = LE32(*(uint32*)"RIFF");
	h->cRIFF.Size = LE32(data_size + sizeof(WAVEHEADER) - sizeof(RIFFCHUNK));
	h->WAVE = LE32(*(uint32*)"WAVE");
	h->cfmt.FOURCC = LE32(*(uint32*)"fmt ");
	h->cfmt.Size = LE32(sizeof(h->Format));
	h->Format.wFormatTag = LE16(WAVE_FORMAT_PCM);
	h->Format.nChannels = LE16(channels);
	h->Format.nSamplesPerSec = LE16(sample_rate);
	h->Format.nBlockAlign = LE16((channels * bits_per_sample) / 8);
	h->Format.nAvgBytesPerSec = LE32(
		h->Format.nSamplesPerSec * h->Format.nBlockAlign
	);
	h->Format.wBitsPerSample = LE16(bits_per_sample);
	h->cdata.FOURCC = LE32(*(uint32*)"data");
	h->cdata.Size = LE32(data_size);
}

ao_bool wavedump_open(wavedump_t *wave, const char *fn)
{
	uint32 temp = 0;
	assert(wave);

	wave->data_size = 0;
	wave->file = fopen_derivative(fn, ".wav");
	if(!wave->file) {
		return false;
	}
	// Jump over header, we write that one later
	fwrite(&temp, 1, sizeof(WAVEHEADER), wave->file);
	return true;
}

void wavedump_append(wavedump_t *wave, uint32 len, void *buf)
{
	assert(wave);
	if(wave->file) {
		wave->data_size += len;
		// XXX: Doesn't the data need to be swapped on big-endian platforms?
		// That would mean that we need to know the target wave format on
		// opening time.
		fwrite(buf, len, 1, wave->file);
	}
}

void wavedump_finish(
	wavedump_t *wave, uint32 sample_rate, uint16 bits_per_sample, uint16 channels
)
{
	assert(wave);
	if(wave->file) {
		WAVEHEADER h;
		wavedump_header_fill(
			&h, wave->data_size, sample_rate, bits_per_sample, channels
		);
		fseek(wave->file, 0, SEEK_SET);
		fwrite(&h, sizeof(h), 1, wave->file);
		fclose(wave->file);
		wave->file = NULL;
	}
}
