#ifndef TEXT_H_
#define TEXT_H_

#include <stdbool.h>
#include <stdio.h>

#include "map.h"

/* Collects LINES from STREAM into COLLECTION. Returns -1 on ERROR. */
int lines(FILE *stream, int (*collect)(void *, char *), void *collection);

/* Consumes LINES from a COLLECTION and inserts them into a MAP. Returns -1 on ERROR. */
int insert(struct map_t *csv, char *(*consume)(void *), void *collection);

/* Generate WORDS from CSV and sends into STREAM. It flushes the STREAM at the END. */
void text(struct map_t *csv, FILE *stream, size_t n, char *start);

#endif /* text.h */
