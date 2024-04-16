#include "word_graph.h"
#include "string.h"
#include "vector.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* */
typedef enum { WORD, KEY, VALUE } Csv;

/* */
int wordcmp(void *w1, void *w2) {
  return strcmp(((Word *)w1)->slice, ((Word *)w2)->slice);
}

Word *word_from_string(String *string) {
  Word *word = malloc(sizeof(Word));
  word->slice = string->slice;
  word->links = vec();

  return word;
}

// TODO: const pointer
// TODO: graph, table are alla synonyms
// TODO: if read utf8 non italian character, spit out "invalid character, not
// italian text"

// Word *word = malloc(sizeof(Word));
// word->slice = token->slice;
// word->successors = vec();
// current_word = insert(graph, word, wordcmp);

Vector *graph_from_csv(char *csv) {
  Vector *graph = vec();
  Csv state = WORD;

  String *token = str();

  Word *current_word;
  Word *current_key;
  Link *link;

  do {
    char c = *csv;
    if (c == ',' || c == '\n') {
      switch (state) {
      case WORD:
        current_word = insert(graph, word_from_string(token), wordcmp);
        state = KEY;
        break;

      case KEY:
        current_key = insert(graph, word_from_string(token), wordcmp);
        state = VALUE;
        break;

      case VALUE:
        link = malloc(sizeof(Link));
        link->word = current_key;
        link->frequency = atof(token->slice);

        push(current_word->links, link);

        state = c == ',' ? KEY : WORD; // either ',' or '\n'
        break;
      }

      token = str(); // reset token
    } else
      append(token, c);

    csv++;
  } while (*csv);

  return graph;
}

int is_special(char c) { return c == '?' || c == '!' || c == '.'; }

int is_letter(char c) {
  // TODO: lettere accentate italiane se possibile / utf-8 letters?
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '\'';
}

typedef enum { SPECIAL, SEPARATOR, LETTER } Character;

Vector *graph_from_string(char *string) {
  Vector *graph = vec();
  Character state = SPECIAL;

  String *token = str();

  // Word *current_word;
  // Word *current_key;
  // Link *link;

  do {
    *string = tolower(*string);

    switch (state) {
    case SPECIAL:
      break;
    case LETTER:
      break;
    case SEPARATOR:
      break;
    }

    string++;
  } while (*string);

  return graph;
}