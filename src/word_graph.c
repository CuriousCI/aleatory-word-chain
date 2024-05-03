#include "word_graph.h"
#include "hashmap.h"
#include "rbtree.h"
#include "string.h"
#include "vector.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

// int wordcmp(void *w1, void *w2) {
//   return wcscmp(((Word *)w1)->slice, ((Word *)w2)->slice);
// }

// Word *word_from_string(String *string) {
//   Word *word = malloc(sizeof(Word));
//   word->slice = string->str;
//   word->links = vec();
//
//   return word;
// }

// TODO: const pointer and const parameters
// TODO: graph, table are alla synonyms
// TODO: if read utf8 non italian wchar_tacter, spit out "invalid
// wchar_tacter,

int is_special(wchar_t c) { return c == '?' || c == '!' || c == '.'; }

typedef enum { SPECIAL, SEPARATOR, LETTER } Character;

/* */
static Vector *parse_words(wchar_t *slice) {
  Vector *words = vec();
  Character state = SPECIAL;
  String *token;

  push(words, wchar_t_to_str(L'.'));

  do {
    wchar_t c = tolower(*slice);

    if (c == '\'') {
      if (state == LETTER) {
        append(token, c);
        push(words, token->str);
      }

      state = SEPARATOR;
    }
    if (is_special(c)) {
      if (state == LETTER)
        push(words, token->str);

      push(words, wchar_t_to_str(c));
      state = SPECIAL;
      // } else if (is_letter(c)) {
    } else if (iswalpha(c)) {
      if (state == LETTER)
        append(token, c);
      else
        token = wchar_t_to_string(c);

      state = LETTER;
    } else {
      if (state == LETTER)
        push(words, token->str);

      state = SEPARATOR;
    }

    slice++;
  } while (*slice);

  // TODO: check if actually needed -  push(words, slice_from_wchar_t('.'));
  return words;
}

typedef struct Record {
  size_t count;
  RBTree *map;
} Record;

RBTree *table(Vector *words) {
  RBTree *map = tree();

  for (size_t i = 0; i < words->len - 1; i++) {
    wchar_t *word = get(words, i);
    wchar_t *next_word = get(words, i + 1);

    Record *record = value(map, word, sizeof(Record));
    if (record->map == NULL)
      record->map = tree();
    record->count++;

    size_t *count = value(record->map, next_word, sizeof(size_t));
    (*count) += 1;
  }

  // visit(map);

  return map;
}

void print_table(wchar_t *str) {
  RBTree *map = table(parse_words(str));

  for (int i = 0; i < map->nodes->len; i++) {
    Node *node = get(map->nodes, i);
    printf("%ls", node->key);

    Record *record = node->value;
    RBTree *children_map = record->map;
    for (int j = 0; j < children_map->nodes->len; j++) {
      Node *child = get(children_map->nodes, j);
      printf(",%ls,%g", child->key,
             (float)*((int *)child->value) / (float)record->count);
    }
    printf("\n");
  }
}

// - [ ] nel generare il testo casuale si può specificare una prima parola,
// altrimenti il programma seleziona una punteggiatura a caso tra `.`, `?`,
// `!` _(secondo la tabella di input)_ e genera la parola successiva
// TODO: if there is an argument word, start from it

/* Possibile states of the FSM (finite state machine) that reads CSV's */
typedef enum { WORD, KEY, VALUE } Csv;

RBTree *parse_csv(wchar_t *csv) {
  // HashMap *graph = hash_map(1000);

  RBTree *graph = tree();

  Csv state = WORD;

  String *token = str();

  RBTree *current_word_map;
  Node *current_key_node;
  Link *link;

  do {
    wchar_t c = *csv;

    if (c == ',' || c == '\n') {
      switch (state) {
      case WORD: {
        Node *current_word_node = node(graph, token->str);
        if (current_word_node->value == NULL)
          current_word_node->value = tree();
        current_word_map = current_word_node->value;
      }

        state = KEY;
        break;

      case KEY:
        current_key_node = node(graph, token->str);

        state = VALUE;
        break;

      case VALUE:
        link = value(current_word_map, current_key_node->key, sizeof(Link));
        link->node = current_key_node;
        link->frequency = wcstof(token->str, 0);

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

// HashMap *parse_csv(wchar_t *csv) {
// HashMap *graph = hash_map(1000);
// HashMap *graph = hash_map(1);
// Csv state = WORD;
//
// String *token = str();
//
// HashMap *current_word_map;
// Entry *current_key_entry;
// Link *link;
//
// do {
//   wchar_t c = *csv;
//
//   if (c == ',' || c == '\n') {
//     switch (state) {
//     case WORD: {
//       Entry *current_word_entry = entry(graph, token->str);
//       if (current_word_entry->value == NULL)
//         current_word_entry->value = hash_map(0);
//       current_word_map = current_word_entry->value;
//     }
//
//       state = KEY;
//       break;
//
//     case KEY:
//       current_key_entry = entry(graph, token->str);
//
//       state = VALUE;
//       break;
//
//     case VALUE:
//       link = value(current_word_map, current_key_entry->key, sizeof(Link));
//       link->entry = current_key_entry;
//       link->frequency = wcstof(token->str, 0);
//
//       state = c == ',' ? KEY : WORD; // either ',' or '\n'
//       break;
//     }
//
//     token = str(); // reset token
//   } else
//     append(token, c);
//
//   csv++;
// } while (*csv);
// return graph;
// }

wchar_t *generate_text(RBTree *graph, size_t n) {
  srand(time(NULL));

  RBTree *links = value(graph, L".", sizeof(HashMap));
  Link *link;

  int capitalize = 0;
  for (int i = 0; i < n; i++) {
    float rng = rand() / (float)RAND_MAX;
    float total = 0;

    for (int j = 0; j < links->nodes->len; j++) {
      // Node* link
    }

    // for (int j = 0; links != NULL && j < links->size; j++) {
    //   Vector *bucket = at(links, j);
    //
    //   for (int k = 0; bucket != NULL && k < bucket->len; k++) {
    //     Entry *e = get(bucket, k);
    //
    //     Link *l = e->value;
    //
    //     if (total >= rng)
    //       break;
    //
    //     total += l->frequency;
    //     link = l;
    //   }
    // }

    // fprintf(stderr, "%ld\n", bucket->len);
    // fprintf(stderr, "BEFORE\n");
    // fprintf(stderr, "AFTER\n");
    // if (capitalize) {
    //   ((wchar_t *)link->entry->key)[0] =
    //       toupper(((wchar_t *)link->entry->key)[0]);
    //   printf("%ls ", (wchar_t *)link->entry->key);
    //   ((wchar_t *)link->entry->key)[0] =
    //       tolower(((wchar_t *)link->entry->key)[0]);
    //   capitalize = 0;
    // }
    // printf("%ls ", (wchar_t *)link->entry->key);
    // TODO: fast concatenation and print at the end
    // if (is_special(((wchar_t *)link->entry->key)[0]))
    //   capitalize = 1;

    // if (link != NULL)
    //   links = link->entry->value;
  }

  return NULL;
}

// wchar_t *generate_text(HashMap *graph, size_t n) {
//   srand(time(NULL));
//
//   // HashMap *links = value(graph, L".", sizeof(HashMap));
//   // Link *link;
//   //
//   // int capitalize = 0;
//   // for (int i = 0; i < n; i++) {
//   //   float rng = rand() / (float)RAND_MAX;
//   //   float total = 0;
//   //
//   //   for (int j = 0; links != NULL && j < links->size; j++) {
//   //     Vector *bucket = at(links, j);
//   //
//   //     for (int k = 0; bucket != NULL && k < bucket->len; k++) {
//   //       Entry *e = get(bucket, k);
//   //
//   //       Link *l = e->value;
//   //
//   //       if (total >= rng)
//   //         break;
//   //
//   //       total += l->frequency;
//   //       link = l;
//   //     }
//   //   }
//   //
//   //   // fprintf(stderr, "%ld\n", bucket->len);
//   //   // fprintf(stderr, "BEFORE\n");
//   //   // fprintf(stderr, "AFTER\n");
//   //   // if (capitalize) {
//   //   //   ((wchar_t *)link->entry->key)[0] =
//   //   //       toupper(((wchar_t *)link->entry->key)[0]);
//   //   //   printf("%ls ", (wchar_t *)link->entry->key);
//   //   //   ((wchar_t *)link->entry->key)[0] =
//   //   //       tolower(((wchar_t *)link->entry->key)[0]);
//   //   //   capitalize = 0;
//   //   // }
//   //   // printf("%ls ", (wchar_t *)link->entry->key);
//   //   // TODO: fast concatenation and print at the end
//   //   // if (is_special(((wchar_t *)link->entry->key)[0]))
//   //   //   capitalize = 1;
//   //
//   //   if (link != NULL)
//   //     links = link->entry->value;
//   // }
//
//   return NULL;
// }

/* Generates a random text from a specified word */
wchar_t *generate_text_from_word(Vector *graph, size_t n, wchar_t *word) {
  return NULL;
}
