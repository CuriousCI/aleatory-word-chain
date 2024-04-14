#ifndef WORD_GRAPH_H_
#define WORD_GRAPH_H_

#include "vector.h"

// typedef Vector *Table;
// TODO: parse
// TODO: structs
// TODO: rename file to graph?

/* */
typedef struct Word {
  char *slice;
  Vector *links;
} Word;

/* */
typedef struct Link {
  Word *word;
  float frequency;
} Link;

/* */
Vector *graph_from_string(char *string);

/* */
int wordcmp(void *w1, void *w2);

#endif // !TABLE_H_
