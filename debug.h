/*
 * Audio Overload SDK
 *
 * Debug GUI
 *
 * Author: Nmlgc
 */

#ifdef __cplusplus
extern "C" {
#endif

// All functions return false when debugging should be stopped.
ao_bool debug_start(void);
ao_bool debug_frame(void);
ao_bool debug_stop(void);

#ifdef __cplusplus
}
#endif
