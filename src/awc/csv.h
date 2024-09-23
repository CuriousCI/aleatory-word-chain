#ifndef CSV_H_
#define CSV_H_

/* Functions to generate CSV from TEXT. */

#include <stdio.h>

#include "../collections/map.h"

/* Splits STREAM into WORDS which collects into a COLLECTION. Returns -1 on ERROR. */
int split_words(FILE *stream, int (*collect)(void *, char *), void *collection);

/* Consumes a COLLECTION of WORDS into COUNTERS. Returns -1 on ERROR. */
int count_words(struct map_t *counters, char *(*consume)(void *), void *collection);

/* Maps each COUNTER in COUNTERS into CSV lines which are collected into COLLECTION. Returns -1 on ERROR. */
int csv(struct map_t *counters, int (*collect)(void *, char *), void *collection);

#endif /* csv.h */
