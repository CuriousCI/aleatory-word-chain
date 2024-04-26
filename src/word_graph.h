#ifndef WORD_GRAPH_H_
#define WORD_GRAPH_H_

#include "hashmap.h"
#include "vector.h"
#include <wchar.h>

// TODO: parse
// TODO: structs

/* */
typedef struct Word {
  wchar_t *slice;
  Vector *links;
} Word;

/* Compares the slices in two words with with wcscmp. */
int wordcmp(void *w1, void *w2);

/* */
HashMap *parse_csv(wchar_t *csv);

// TODO: double instead of float? Maybe long double?
/* */
typedef struct Link {
  Entry *entry;
  float frequency;
} Link;

/* */
// Vector *graph_from_slice(wchar_t *slice);

// TODO: return -1 on error?
// TODO: generate table
// TODO: then generate string
// TODO: then save string
// TODO: lettere accentate italiane se possibile / utf-8 letters?

/* */
void print_table(wchar_t *slice);

/* Parses a CSV file and generates a graph of words. */
Vector *graph_from_csv(wchar_t *csv);

// TODO: generate text
/* Generates a random text from a random character '.', '?', '!' */
// wchar_t *generate_text(Vector *graph, size_t n);
wchar_t *generate_text(HashMap *graph, size_t n);

// TODO: generate text
/* Generates a random text from a specified word */
wchar_t *generate_text_from_word(Vector *graph, size_t n, wchar_t *word);

#endif // !TABLE_H_
