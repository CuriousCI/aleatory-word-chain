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
#include <wchar.h>
#include <wctype.h>

int wordcmp(void *w1, void *w2) {
  return wcscmp(((Word *)w1)->slice, ((Word *)w2)->slice);
}

Word *word_from_string(String *string) {
  Word *word = malloc(sizeof(Word));
  word->slice = string->str;
  word->links = vec();

  return word;
}

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

  push(words, slice_from_wchar_t('.'));

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

      push(words, slice_from_wchar_t(c));
      state = SPECIAL;
      // } else if (is_letter(c)) {
    } else if (iswalpha(c)) {
      if (state == LETTER)
        append(token, c);
      else
        token = string_from_wchar_t(c);

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

// typedef struct Record {
//   size_t count;
//   HashMap *map;
// } Record;

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
    *count += 1;
  }

  return map;
}

// HashMap *table(Vector *words) {
//   HashMap *map = hash_map(words->len / 100);
//
//   for (int i = 0; i < words->len - 1; i++) {
//     wchar_t *word = get(words, i);
//     wchar_t *next_word = get(words, i + 1);
//
//     Record *record = value(map, word, sizeof(Record));
//     if (record->map == NULL)
//       record->map = hash_map(100);
//     record->count++;
//
//     size_t *count = value(record->map, next_word, sizeof(size_t));
//     *count += 1;
//   }
//
//   return map;
// }

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

  // for (int i = 0; i < map->size; i++) {
  //   Vector *bucket = at(map, i);
  //
  //   for (int j = 0; bucket != NULL && j < bucket->len; j++) {
  //     Entry *entry = get(bucket, j);
  //     Record *record = entry->value;
  //     size_t count = record->count;
  //     printf("%ls", (wchar_t *)entry->key);
  //
  //     HashMap *links = record->map;
  //
  //     for (int k = 0; links != NULL && k < links->size; k++) {
  //       Vector *links_buckets = at(links, k);
  //
  //       for (int n = 0; links_buckets != NULL && n < links_buckets->len; n++)
  //       {
  //         Entry *e = get(links_buckets, n);
  //         size_t *link_count = e->value;
  //
  //         printf(",%ls,%g", (wchar_t *)e->key,
  //                (float)*link_count / (float)count);
  //       }
  //     }
  //
  //     printf("\n");
  //   }
  // }
}

// void print_table(wchar_t *str) {
//   HashMap *map = table(parse_words(str));
//
//   for (int i = 0; i < map->size; i++) {
//     Vector *bucket = at(map, i);
//
//     for (int j = 0; bucket != NULL && j < bucket->len; j++) {
//       Entry *entry = get(bucket, j);
//       Record *record = entry->value;
//       size_t count = record->count;
//       printf("%ls", (wchar_t *)entry->key);
//
//       HashMap *links = record->map;
//
//       for (int k = 0; links != NULL && k < links->size; k++) {
//         Vector *links_buckets = at(links, k);
//
//         for (int n = 0; links_buckets != NULL && n < links_buckets->len; n++)
//         {
//           Entry *e = get(links_buckets, n);
//           size_t *link_count = e->value;
//
//           printf(",%ls,%g", (wchar_t *)e->key,
//                  (float)*link_count / (float)count);
//         }
//       }
//
//       printf("\n");
//     }
//   }
// }

// - [ ] nel generare il testo casuale si può specificare una prima parola,
// altrimenti il programma seleziona una punteggiatura a caso tra `.`, `?`,
// `!` _(secondo la tabella di input)_ e genera la parola successiva
// TODO: if there is an argument word, start from it

/* Possibile states of the FSM (finite state machine) that reads CSV's */
typedef enum { WORD, KEY, VALUE } Csv;

HashMap *parse_csv(wchar_t *csv) {
  // HashMap *graph = hash_map(1000);
  HashMap *graph = hash_map(1);
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

  return graph;
}

wchar_t *generate_text(HashMap *graph, size_t n) {
  srand(time(NULL));

  // HashMap *links = value(graph, L".", sizeof(HashMap));
  // Link *link;
  //
  // int capitalize = 0;
  // for (int i = 0; i < n; i++) {
  //   float rng = rand() / (float)RAND_MAX;
  //   float total = 0;
  //
  //   for (int j = 0; links != NULL && j < links->size; j++) {
  //     Vector *bucket = at(links, j);
  //
  //     for (int k = 0; bucket != NULL && k < bucket->len; k++) {
  //       Entry *e = get(bucket, k);
  //
  //       Link *l = e->value;
  //
  //       if (total >= rng)
  //         break;
  //
  //       total += l->frequency;
  //       link = l;
  //     }
  //   }
  //
  //   // fprintf(stderr, "%ld\n", bucket->len);
  //   // fprintf(stderr, "BEFORE\n");
  //   // fprintf(stderr, "AFTER\n");
  //   // if (capitalize) {
  //   //   ((wchar_t *)link->entry->key)[0] =
  //   //       toupper(((wchar_t *)link->entry->key)[0]);
  //   //   printf("%ls ", (wchar_t *)link->entry->key);
  //   //   ((wchar_t *)link->entry->key)[0] =
  //   //       tolower(((wchar_t *)link->entry->key)[0]);
  //   //   capitalize = 0;
  //   // }
  //   // printf("%ls ", (wchar_t *)link->entry->key);
  //   // TODO: fast concatenation and print at the end
  //   // if (is_special(((wchar_t *)link->entry->key)[0]))
  //   //   capitalize = 1;
  //
  //   if (link != NULL)
  //     links = link->entry->value;
  // }

  return NULL;
}

/* Generates a random text from a specified word */
wchar_t *generate_text_from_word(Vector *graph, size_t n, wchar_t *word) {
  return NULL;
}
