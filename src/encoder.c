#include "encoder.h"
#include "string.h"
#include <stdbool.h>
#include <stdlib.h>
#include <wctype.h>

// #include <uchar.h>
// int is_alpha = UCHAR_ISALPHA(c) || iswdigit(c),

static wchar_t *wctowcs(wchar_t c) {
  wchar_t *str = calloc(2, sizeof(wchar_t));
  *str = c;

  return str;
}

// get input from file, save to file
// parser struct, with is_word, token and index, L'?', L'!' and L'.'
//    - returns a single word, the next word
//    - while not null { parse(&str, &context) }
//    - it doesn't even need the token

static struct parser {
  wchar_t *base;
  bool is_word;
  wchar_t *question, *exclamation, *dot;
} parser;

struct parser *new_parser();

wchar_t *generic_parse_2(wchar_t *str, struct parser *p) {
  if (p == NULL) {
    p = calloc(1, sizeof(struct parser));
    *p = (struct parser){str, false, wctowcs(L'?'), wctowcs(L'!'),
                         wctowcs(L'.')};
  }

  wchar_t *token = str;
  // size_t token = 0, index = 0;
  // int is_word = 0;

  // wchar_t *q = wctowcs(L'?'), *e = wctowcs(L'!'), *d = wctowcs(L'.');

  // save(object, d);

  do {
    wchar_t c = *str;
    bool is_alpha = iswalpha(c) || iswdigit(c);
    bool save = false;

    if (is_alpha)
      *str = towlower(*str);

    if (p->is_word) {
      if (!is_alpha)
        save = true;
    } else if (is_alpha)
      token = p->base;

    // if (c == L'?')
    //   save(object, q);
    // if (c == L'!')
    //   save(object, e);
    // if (c == L'.')
    //   save(object, d);

    if (save) {
      *str = 0;
      return token;
    }

    p->is_word = is_alpha;
  } while (*++str);

  return NULL;
}
void generic_parse(wchar_t *str, void *object,
                   void (*save)(void *, wchar_t *)) {
  wchar_t *text = str;
  size_t token = 0, index = 0;
  int is_word = 0;

  wchar_t *q = wctowcs(L'?'), *e = wctowcs(L'!'), *d = wctowcs(L'.');

  save(object, d);

  do {
    wchar_t c = *str;
    int is_alpha = iswalpha(c) || iswdigit(c);

    if (is_alpha)
      *str = towlower(*str);

    if (is_word) {
      if (!is_alpha) {
        save(object, text + token);
        *str = 0;
      }
    } else if (is_alpha)
      token = index;

    if (c == L'?')
      save(object, q);
    if (c == L'!')
      save(object, e);
    if (c == L'.')
      save(object, d);

    is_word = is_alpha;
    index++;
  } while (*++str);
  // TODO: apostrofi
}

Vector *parse(wchar_t *str) {
  Vector *words = vec();

  wchar_t *text = str;
  size_t token = 0, counter = 0;
  int is_word = 0;

  wchar_t *q = wctowcs(L'?'), *e = wctowcs(L'!'), *d = wctowcs(L'.');

  push(words, d);

  do {
    wchar_t c = *str;
    int is_alpha = iswalpha(c) || iswdigit(c);

    if (is_alpha)
      *str = towlower(*str);

    if (is_word) {
      if (!is_alpha) {
        push(words, text + token);
        *str = 0;
      }
    } else if (is_alpha)
      token = counter;

    if (c == L'?')
      push(words, q);
    if (c == L'!')
      push(words, e);
    if (c == L'.')
      push(words, d);

    is_word = is_alpha;
    counter++;
  } while (*++str);

  // TODO: apostrofi

  return words;
}

typedef struct Word {
  size_t count;
  RBTree *adjacent;
} Word;

// How to get input? Output should be (void *object, void(*save)(void *, wchar_t
// *))
// count parola per parola, oppure count dal file?
// in single process case read pointers
// in multi-process case read words
// in both read words

// RBTree *generic_count(FILE *file) {
//   // TODO: l'ultima parola punta alla prima
//   RBTree *words_counter = tree();
//   wchar_t *current = NULL, *next = NULL;
//
//   while (!current)
//     fwscanf(file, L"%ls", &current);
//
//   while (1) {
//     fwscanf(file, L"%ls", &next);
//
//     Word *counter = value(words_counter, current, sizeof(Word));
//     counter->count++;
//
//     if (counter->adjacent == NULL)
//       counter->adjacent = tree();
//
//     Node *count = node(counter->adjacent, next);
//     count->value++;
//
//     current = next;
//   }
//
//   return words_counter;
// }
//

static struct line {
  size_t count;
  RBTree *adj;
} line;

void generic_count(wchar_t *current, wchar_t *next, RBTree *counter) {
  struct line *l = value(counter, current, sizeof(line));
  l->count++;

  if (l->adj == NULL)
    l->adj = tree();

  Node *count = node(l->adj, next);
  count->value++;
}

// Instead of getting the words, gets' two words,
// Then gets the RBTreeMap struct (maybe rbtree_map?)
// adds the item to the rbtree_map
/* */
RBTree *count(Vector *words) {
  // TODO: l'ultima parola punta alla prima
  RBTree *counter = tree();
  size_t index = 0;

  while (index < words->len - 1) {
    wchar_t *current = get(words, index);
    wchar_t *next = get(words, ++index);

    Word *word = value(counter, current, sizeof(Word));
    word->count++;

    if (word->adjacent == NULL)
      word->adjacent = tree();

    Node *count = node(word->adjacent, next);
    count->value++;
  }

  return counter;
}

// gets a line, or maybe just the actual values:
//     - a word
//     - it's count
//         - the next words (maybe an array ? )
//         - the count for each
void generic_save(RBTree *counter, FILE *file) {
  for (size_t i = 0; i < counter->nodes->len; i++) {
    Node *node = get(counter->nodes, i);
    fwprintf(file, L"%ls", node->key);

    Word *word = node->value;
    RBTree *adjacent = word->adjacent;

    for (size_t j = 0; j < adjacent->nodes->len; j++) {
      Node *next = get(adjacent->nodes, j);
      // TODO: %f instead of %g? I already account for scientific notation now
      // that I think about it!
      fwprintf(file, L",%ls,%g", next->key,
               (float)((size_t)next->value) / (float)word->count);
    }

    fwprintf(file, L"\n");
  }
}

/* */
void save(RBTree *counter) {
  for (size_t i = 0; i < counter->nodes->len; i++) {
    Node *node = get(counter->nodes, i);
    wprintf(L"%ls", node->key);

    Word *word = node->value;
    RBTree *adjacent = word->adjacent;

    for (size_t j = 0; j < adjacent->nodes->len; j++) {
      Node *next = get(adjacent->nodes, j);
      wprintf(L",%ls,%g", next->key,
              (float)((size_t)next->value) / (float)word->count);
    }

    wprintf(L"\n");
  }
}

// void save_(Map *counter, FILE *file) {
//   Entry *entry;
//   while (next(&entry, counter) != NULL) {
//     fwprintf(file, L"%ls", key(entry));
//     Map *adjacent = value(entry).adjacent;
//
//     Entry *word;
//     while (next(&word, adjacent) != NULL) {
//       fwprintf(file, L",%ls,%g", key(word),
//                (float)value(word) / (float)value(word).count);
//       word = next(word)
//     }
//
//     fwprintf(file, L"\n");
//     entry = next(entry);
//   }
// }
