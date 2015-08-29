/*
 * Audio Overload SDK
 *
 * MIDI dumping
 *
 * Author: Nmlgc
 */

extern ao_bool nomidi;

void mididump_vchan_note_on(int vchan, char note, char velocity);
void mididump_vchan_note_off(int vchan, char note, char velocity);
ao_bool mididump_write(const char *fn);
void mididump_free(void);
