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

int corlett_decode(uint8 *input, uint32 input_len, uint8 **output, uint64 *size, corlett_t *c);
void corlett_free(corlett_t *c);
int corlett_tag_recognize(corlett_t *c, const char **target_value, int tag_num, const char *key);
void corlett_length_set(uint32 length_ms, int32 fade_ms);
uint32 corlett_sample_count(void);
void corlett_sample_fade(int16 *l, int16 *r);
uint32 psfTimeToMS(const char *str);
