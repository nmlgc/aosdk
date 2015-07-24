/*
 * Audio Overload SDK
 *
 * Audio backend function declarations
 */

#ifndef M1SDR_H
#define M1SDR_H

typedef void m1sdr_callback_t(
	unsigned long sample_count, stereo_sample_t *buffer
);
extern m1sdr_callback_t *m1sdr_Callback;

#endif /* M1SDR_H */
