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

ao_bool wavedump_stream_open(wavedump_t *wave, const char *fn);
void wavedump_stream_append(wavedump_t *wave, unsigned long sample_count, stereo_sample_t *buffer);
void wavedump_stream_finish(wavedump_t *wave, uint32 sample_rate);
