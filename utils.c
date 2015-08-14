/*
 * Audio Overload SDK
 *
 * Random utility functions
 *
 * Author: Nmlgc
 */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ao.h"
#include "utils.h"

/// Hash table
/// ----------
// From http://www.cse.yorku.ca/~oz/hash.html
static unsigned long hash(const char *key_buf, size_t key_len)
{
	unsigned long hash = 5381;
	int i;
	for(i = 0; i < key_len; i++) {
		hash = ((hash << 5) + hash) ^ key_buf[i]; // hash * 33 ^ c
	}
	return hash;
}

// MSVC has memicmp(), but GCC does not, so this has to have a different name.
static int memcasecmp(const void *s1, const void *s2, size_t n)
{
	const char *sc1 = (const char *)s1;
	const char *sc2 = (const char *)s2;
	const char *sc2end = sc2 + n;

	while (sc2 < sc2end && toupper(*sc1) == toupper(*sc2)) {
		sc1++, sc2++;
	}
	if (sc2 == sc2end) {
		return 0;
	}
	return (int) (toupper(*sc1) - toupper(*sc2));
}

static int bucket_match(const hashtable_bucket_t *bucket, const void *key_buf, size_t key_len, hashtable_flags_t flags)
{
	if(bucket->key_len != key_len) {
		return 0;
	}
	return flags & HT_CASE_INSENSITIVE
		? !memcasecmp(bucket->key_buf, key_buf, key_len)
		: !memcmp(bucket->key_buf, key_buf, key_len);
}

void* hashtable_get(hashtable_t *table, const void *key_buf, size_t key_len, hashtable_flags_t flags)
{
	hashtable_bucket_t *bucket;
	ao_bool create = flags & HT_CREATE;

	assert(table);
	assert(table->data_size > 0);
	assert(key_buf);
	assert(key_len);

	bucket = &table->buckets[hash(key_buf, key_len) % BUCKETS];

	while(bucket->data && !bucket_match(bucket, key_buf, key_len, flags)) {
		if(!bucket->next && create) {
			bucket->next = calloc(1, sizeof(hashtable_bucket_t));
		}
		bucket = bucket->next;
	}
	if(!bucket->data && create) {
		bucket->data = calloc(1, table->data_size);
		bucket->key_len = key_len;
		bucket->key_buf = malloc(key_len);
		memcpy(bucket->key_buf, key_buf, key_len);
	}
	return bucket->data;
}

void* hashtable_iterate(hashtable_t *table, hashtable_iterator_t *iter)
{
	hashtable_t *ret = NULL;
	assert(table);
	assert(iter);
	while(iter->i < BUCKETS) {
		if(!iter->bucket) {
			iter->bucket = &table->buckets[iter->i];
		}
		if(iter->bucket->data) {
			ret = iter->bucket->data;
		}
		if(iter->bucket->next) {
			iter->bucket = iter->bucket->next;
		} else {
			iter->i++;
			iter->bucket = NULL;
		}
		if(ret) {
			return ret;
		}
	}
	return ret;
}

unsigned int hashtable_length(hashtable_t *table)
{
	unsigned int ret = 0;
	hashtable_iterator_t iter = {0};
	assert(table);
	while(hashtable_iterate(table, &iter)) {
		ret++;
	}
	return ret;
}

void hashtable_free(hashtable_t *table)
{
	int i;
	assert(table);
	if(!table->buckets) {
		return;
	}
	for(i = 0; i < BUCKETS; i++) {
		// We only have to free the key of the first entry in each
		// bucket's list, as it was allocated as part of a single array
		// in hashtable_init().
		hashtable_bucket_t *bucket = &table->buckets[i];
		if(bucket->key_buf) {
			free(bucket->key_buf);
		}
		bucket = bucket->next;
		while(bucket) {
			hashtable_bucket_t *bucket_cur = bucket;
			bucket = bucket->next;
			if(bucket_cur->data) {
				free(bucket_cur->data);
				free(bucket_cur->key_buf);
			}
			free(bucket_cur);
		}
	}
}
/// ----------

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
