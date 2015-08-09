//
// Audio Overload
// Emulated music player
//
// (C) 2000-2008 Richard F. Bannister
//

// corlett.h

#define MAX_UNKNOWN_TAGS			32

typedef struct
{
	char *tag_buffer;

	// All tags are pointers into [tag_buffer].
	const char *lib;
	const char *libaux[8];

	const char *inf_title;
	const char *inf_copy;
	const char *inf_artist;
	const char *inf_game;
	const char *inf_year;
	const char *inf_length;
	const char *inf_fade;

	const char *inf_refresh;

	const char *tag_name[MAX_UNKNOWN_TAGS];
	const char *tag_data[MAX_UNKNOWN_TAGS];

	uint32 *res_section;
	uint32 res_size;
} corlett_t;

// Engine-specific callback function called during corlett_decode() for all
// the libraries referenced in a Corlett file, as well as the main file
// itself. The function will be called in this order, and with these values
// for [libnum]:
//
//	• 1 ("_lib")
//	• 2 ("_lib2"), 3 ("_lib3"), 4, …, 9 ("_lib9")
// 	• 0 (the main file itself)
//
// All pointers passed for [lib] and [c] stay valid until after the final
// callback for library #0. This allows the callback function to change the
// order in which to process the libraries by caching the pointers 
// applying their data in later calls.
//
// The function should return either AO_SUCCESS, or AO_FAIL if an error
// occurred and corlett_decode() should be aborted.
typedef int corlett_lib_callback_t(int libnum, uint8 *lib, uint64 size, corlett_t *c);

int corlett_decode(uint8 *input, uint32 input_len, corlett_t *c, corlett_lib_callback_t *lib_callback);
void corlett_free(corlett_t *c);
int corlett_tag_recognize(corlett_t *c, const char **target_value, int tag_num, const char *key);
void corlett_length_set(uint32 length_ms, int32 fade_ms);
uint32 corlett_sample_count(void);
void corlett_sample_fade(stereo_sample_t *sample);
uint32 psfTimeToMS(const char *str);
