#ifndef CSV_H_
#define CSV_H_

#include <stdio.h>

#include "map.h"

/* Splits STREAM into TOKENS which collects into a COLLECTION. Returns -1 on ERROR. */
int split(FILE *stream, int (*collect)(void *, char *), void *collection);

/* Consumes a COLLECTION of TOKENS into COUNTERS. Returns -1 on ERROR. */
int count(struct map_t *counters, char *(*consume)(void *), void *collection);

/* Maps COUNTERS into CSV lines which sends into STREAM. It flushes the STREAM at the end. */
void csv(struct map_t *counters, FILE *stream);

#endif /* csv.h */
