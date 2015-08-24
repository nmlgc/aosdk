/*
 * Audio Overload SDK
 *
 * Wave dumping
 *
 * Author: Nmlgc
 */

typedef struct {
	FILE *file;
	uint32 data_size;
} wavedump_t;

ao_bool wavedump_open(wavedump_t *wave, const char *fn);
void wavedump_append(wavedump_t *wave, uint32 len, void *buffer);
void wavedump_finish(wavedump_t *wave, uint32 sample_rate, uint16 bits_per_sample, uint16 channels);
