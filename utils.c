/*
 * Audio Overload SDK
 *
 * Random utility functions
 *
 * Author: Nmlgc
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

FILE* fopen_derivative(const char *fn, const char *suffix)
{
	size_t full_fn_len = strlen(fn) + strlen(suffix) + 1;
	char *full_fn = malloc(full_fn_len);
	FILE *ret = NULL;

	if(!full_fn) {
		fprintf(stderr, "Out of memory\n");
		return NULL;
	}

	sprintf(full_fn, "%s%s", fn, suffix);
	ret = fopen(full_fn, "wb");
	free(full_fn);
	if(!ret) {
		fprintf(stderr, "Error opening %s for writing: %s\n", fn, strerror(errno));
	}
	return ret;
}
