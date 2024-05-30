#ifndef TEXT_H_
#define TEXT_H_

#include <stdio.h>

#include "../collections/map.h"

/* Collects LINES from STREAM into COLLECTION. Returns -1 on ERROR. */
int split_lines(FILE *stream, int (*collect)(void *, char *), void *collection);

/* Consumes LINES from a COLLECTION and inserts them into a MAP. Returns -1 on ERROR. */
int insert_lines(struct map_t *csv, char *(*consume)(void *), void *collection);

/* Generate WORDS from CSV and collects then into COLLECTION. Returns -1 on ERROR. */
int text(struct map_t *csv, int (*collect)(void *, char *), void *collection, size_t n, char *start);

#endif /* text.h */
