#include "word_graph.h"
#include "hashmap.h"
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
  word->slice = string->slice;
  word->links = vec();

  return word;
}

// TODO: const pointer and const parameters
// TODO: graph, table are alla synonyms
// TODO: if read utf8 non italian wchar_tacter, spit out "invalid wchar_tacter,
// not italian text"

/* Possibile states of the FSM (finite state machine) that reads CSV's */
typedef enum { WORD, KEY, VALUE } Csv;

void *new_map(size_t size) { return hash_map(size); }

HashMap *parse_csv(wchar_t *csv) {
  HashMap *graph = hash_map(1000);
  Csv state = WORD;

  String *token = str();

  HashMap *current_word_map;
  Entry *current_key_entry;
  Link *link;

  do {
    wchar_t c = *csv;

    if (c == ',' || c == '\n') {
      switch (state) {
      case WORD: {
        Entry *current_word_entry = entry(graph, token->slice);
        if (current_word_entry->value == NULL)
          current_word_entry->value = hash_map(100);
        current_word_map = current_word_entry->value;
      }

        state = KEY;
        break;

      case KEY:
        current_key_entry = entry(graph, token->slice);

        state = VALUE;
        break;

      case VALUE:
        link = value(current_word_map, current_key_entry->key, sizeof(Link));
        link->entry = current_key_entry;
        link->frequency = wcstof(token->slice, 0);

        state = c == ',' ? KEY : WORD; // either ',' or '\n'
        break;
      }

      token = str(); // reset token
    } else
      append(token, c);

    csv++;
  } while (*csv);

  // fprintf(stderr, "ciao\n");
  return graph;
}

// Vector *graph_from_csv(wchar_t *csv) {
//   Vector *graph = vec();
//   Csv state = WORD;
//
//   String *token = str();
//
//   Word *current_word;
//   Word *current_key;
//   Link *link;
//
//   do {
//     wchar_t c = *csv;
//
//     if (c == ',' || c == '\n') {
//       switch (state) {
//       case WORD:
//         current_word = vec_insert(graph, word_from_string(token), wordcmp);
//         state = KEY;
//         break;
//
//       case KEY:
//         current_key = vec_insert(graph, word_from_string(token), wordcmp);
//         state = VALUE;
//         break;
//
//       case VALUE:
//         link = malloc(sizeof(Link));
//         link->word = current_key;
//         link->frequency = wcstof(token->slice, 0);
//
//         push(current_word->links, link);
//
//         state = c == ',' ? KEY : WORD; // either ',' or '\n'
//         break;
//       }
//
//       token = str(); // reset token
//     } else
//       append(token, c);
//
//     csv++;
//   } while (*csv);
//
//   return graph;
// }

int is_special(wchar_t c) { return c == '?' || c == '!' || c == '.'; }

// TODO: lettere accentate italiane se possibile / utf-8 letters?
// int is_letter(wchar_t c) {
//   return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
//
//          || c == L'è' || c == L'È' || c == L'é' || c == L'à' || c == L'ù' ||
//          c == L'ò';
// }

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
        push(words, token->slice);
      }

      state = SEPARATOR;
    }
    if (is_special(c)) {
      if (state == LETTER)
        push(words, token->slice);

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
        push(words, token->slice);

      state = SEPARATOR;
    }

    slice++;
  } while (*slice);

  // TODO: check if actually needed -  push(words, slice_from_wchar_t('.'));

  return words;
}

typedef struct Record {
  size_t count;
  HashMap *map;
} Record;

HashMap *table(Vector *words) {
  HashMap *map = hash_map(1000);

  for (int i = 0; i < words->len - 1; i++) {
    wchar_t *word = get(words, i);
    wchar_t *next_word = get(words, i + 1);

    Record *record = value(map, word, sizeof(Record));
    if (record->map == NULL)
      record->map = hash_map(4);
    record->count++;

    size_t *count = value(record->map, next_word, sizeof(size_t));
    *count += 1;
  }

  return map;
}

void print_table(wchar_t *slice) {
  HashMap *map = table(parse_words(slice));

  for (int i = 0; i < map->size; i++) {
    Vector *bucket = at(map, i);

    for (int j = 0; j < bucket->len; j++) {
      Entry *entry = get(bucket, j);
      Record *record = entry->value;
      size_t count = record->count;
      printf("%ls", (wchar_t *)entry->key);

      HashMap *links = record->map;

      for (int k = 0; k < links->size; k++) {
        Vector *links_buckets = at(links, k);

        for (int n = 0; n < links_buckets->len; n++) {
          Entry *e = get(links_buckets, n);
          size_t *link_count = e->value;

          printf(",%ls,%g", (wchar_t *)e->key,
                 (float)*link_count / (float)count);
        }
      }

      printf("\n");
    }
  }
}

// - [ ] nel generare il testo casuale si può specificare una prima parola,
// altrimenti il programma seleziona una punteggiatura a caso tra `.`, `?`,
// `!` _(secondo la tabella di input)_ e genera la parola successiva
// TODO: if there is an argument word, start from it

wchar_t *generate_text(HashMap *graph, size_t n) {
  srand(time(NULL));

  HashMap *links = value(graph, L".", sizeof(HashMap));
  Link *link;

  int capitalize = 0;
  for (int i = 0; i < n; i++) {
    float rng = rand() / (float)RAND_MAX;
    float total = 0;

    for (int j = 0; links != NULL && j < links->size; j++) {
      Vector *bucket = at(links, j);
      for (int k = 0; k < bucket->len; k++) {
        Entry *e = get(bucket, k);

        Link *l = e->value;

        if (total >= rng)
          break;

        total += l->frequency;
        link = l;
      }
    }

    if (capitalize) {
      ((wchar_t *)link->entry->key)[0] =
          toupper(((wchar_t *)link->entry->key)[0]);
      capitalize = 0;
    }

    printf("%ls ", (wchar_t *)link->entry->key);

    // if (is_special(((wchar_t *)link->entry->key)[0]))
    //   capitalize = 1;

    links = link->entry->value;
  }

  return NULL;
}

// wchar_t *generate_text(Vector *graph, size_t n) {
//   srand(time(NULL));
//
//   Word *word = get(graph, 0);
//
//   int capitalize = 0;
//   for (int i = 0; i < n; i++) {
//     float rng = rand() / (float)RAND_MAX;
//     float total = 0;
//
//     Link *link = get(word->links, 0);
//     for (int j = 0; j < word->links->len; j++) {
//       Link *l = get(word->links, j);
//
//       if (total >= rng)
//         break;
//
//       total += l->frequency;
//       link = l;
//     }
//
//     if (capitalize) {
//       link->word->slice[0] = toupper(link->word->slice[0]);
//       capitalize = 0;
//     }
//
//     printf("%ls ", link->word->slice);
//     if (is_special(link->word->slice[0]))
//       capitalize = 1;
//
//     word = link->word;
//   }
//
//   return NULL;
// }

/* Generates a random text from a specified word */
wchar_t *generate_text_from_word(Vector *graph, size_t n, wchar_t *word) {
  return NULL;
}

// for (size_t i = 0; i < graph->len; i++) {
//   Word *word = get(graph, i);
//   printf("%ls\n\t", word->slice);
//   for (size_t j = 0; j < word->links->len; j++) {
//     Link *link = get(word->links, j);
//     printf("%ls: %g; ", link->word->slice, link->frequency);
//   }
//   printf("\n");
// }

// Vector *table(Vector *words) {
//   Vector *table = vec();
//
//   for (int i = 0; i < words->len - 1; i++) {
//     Record *r = calloc(1, sizeof(Record));
//     r->word = get(words, i);
//     r->entries = vec();
//     r = vec_insert(table, r, recordcmp);
//     r->count++;
//
//     Entry *e = calloc(1, sizeof(Entry));
//     e->word = get(words, i + 1);
//     e = vec_insert(r->entries, e, entrycmp);
//     e->count++;
//   }
//
//   return table;
// }

// void print_table(wchar_t *slice) {
//   Vector *t = table(parse_words(slice));
//
//   for (int i = 0; i < t->len; i++) {
//     Record *r = get(t, i);
//     printf("%ls", r->word);
//     for (int j = 0; j < r->entries->len; j++) {
//       Entry *e = get(r->entries, j);
//       printf(",%ls,%g", e->word, (float)e->count / (float)r->count);
//     }
//     printf("\n");
//   }
// }

// int *freq = calloc(256, sizeof(int));
// for (int i = 0; i < words->len; i++) {
//   Word *word = get(words, i);
//   int index = word->slice[0] % 256;
//   if (index < 256)
//     freq[index]++;
// }
//
// for (int i = 0; i < 256; i++)
//   fprintf(stderr, "%d - %d\n", i, freq[i]);
//

// typedef struct Entry {
//   wchar_t *word;
//   size_t count;
// } Entry;
//
// int entrycmp(void *a, void *b) {
//   return wcscmp(((Entry *)a)->word, ((Entry *)b)->word);
// }

// typedef struct Record {
//   wchar_t *word;
//   Vector *entries;
//   size_t count;
// } Record;
//
// int recordcmp(void *a, void *b) {
//   return wcscmp(((Record *)a)->word, ((Record *)b)->word);
// }
// size_t wcshash_v(const void *w) { return wcshash(w); }
//
// int wcscmp_v(const void *w1, const void *w2) { return wcscmp(w1, w2); }

//   Entry *next_entry = find(record->map, next, wcshash_v, wcscmp_v);
//
//   if (next_entry->value == NULL)
//     next_entry->value = calloc(1, sizeof(size_t));
//
//   size_t *count = next_entry->value;
//   *count += 1;
// }

// HashMap *table(Vector *words) {
//   HashMap *map = hash_map(1000);
//
//   for (int i = 0; i < words->len - 1; i++) {
//     wchar_t *word = get(words, i);
//
//     Entry *entry = find(map, word, wcshash_v, wcscmp_v);
//     Record *record;
//
//     if (entry->value == NULL) {
//       record = calloc(1, sizeof(Record));
//       record->map = hash_map(4);
//
//       entry->value = record;
//     } else
//       record = entry->value;
//
//     record->count++;
//
//     wchar_t *next = get(words, i + 1);
//     Entry *next_entry = find(record->map, next, wcshash_v, wcscmp_v);
//
//     if (next_entry->value == NULL)
//       next_entry->value = calloc(1, sizeof(size_t));
//
//     size_t *count = next_entry->value;
//     *count += 1;
//   }
//
//   return map;
// }

// fprintf(stderr, "BEFORE!\n");
// fprintf(stderr, "%d current: %ls\n", i, word);
// fprintf(stderr, "%d next: %ls\n", i, next_word);
// fprintf(stderr, "%p maybe here? \n", record->map);
// fprintf(stderr, "%d or later? \n", i);
// fprintf(stderr, "HERE!\n");

// If dicto doesn't have word, insert record, else
// if (*v == NULL) {
//   *v = calloc(1, sizeof(Record));
//   ((Record *)*v)->map = hash_map(4);
// }

// ((Record *)*v)->count++;

// void **sub_count = value(((Record *)*v)->map, next_word);
// if (*sub_count == NULL)
//   *sub_count = calloc(1, sizeof(size_t));
// *((size_t *)*sub_count) += 1;
