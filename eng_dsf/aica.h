/*

	Sega/Yamaha AICA emulation
*/

#ifndef _AICA_H_
#define _AICA_H_

#define MAX_AICA	(2)

#define COMBINE_DATA(varptr)	(*(varptr) = (*(varptr) & mem_mask) | (data & ~mem_mask))

// convert AO types
typedef int8 data8_t;
typedef int16 data16_t;
typedef int32 data32_t;
typedef int offs_t;

// AICA sample types (value of PCMS)
typedef enum
{
	ST_PCM_16 = 0,
	ST_PCM_8 = 1,
	ST_ADPCM = 2,
	ST_ADPCM_LONG = 3
} AICA_SAMPLE_TYPE;

struct AICAinterface
{
	int num;
	void *region[MAX_AICA];
	int mixing_level[MAX_AICA];			/* volume */
	void (*irq_callback[MAX_AICA])(int state);	/* irq callback */
};

void *aica_start(const void *config);
void AICA_Update(void *param, INT16 **inputs, stereo_sample_t *sample);
int AICA_DumpSample(const char *ram, uint32 SA, uint16 LSA, uint16 LEA, AICA_SAMPLE_TYPE PCMS);

#define READ16_HANDLER(name)	data16_t name(offs_t offset, data16_t mem_mask)
#define WRITE16_HANDLER(name)	void     name(offs_t offset, data16_t data, data16_t mem_mask)

// AICA register access
READ16_HANDLER( AICA_0_r );
WRITE16_HANDLER( AICA_0_w );
READ16_HANDLER( AICA_1_r );
WRITE16_HANDLER( AICA_1_w );

// MIDI I/O access (used for comms on Model 2/3)
WRITE16_HANDLER( AICA_MidiIn );
READ16_HANDLER( AICA_MidiOutR );

#endif
