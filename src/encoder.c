#include "encoder.h"
#include "string.h"
#include <stdlib.h>
#include <wctype.h>

// #include <uchar.h>
// int is_alpha = UCHAR_ISALPHA(c) || iswdigit(c),

static wchar_t *wctowcs(wchar_t c) {
  wchar_t *str = calloc(2, sizeof(wchar_t));
  *str = c;

  return str;
}

Vector *parse(wchar_t *str) {
  Vector *words = vec();
  // String *token;

  wchar_t *text = str;
  int token = 0;
  int counter = 0;

  int is_word = 0;
  wchar_t *question = wctowcs(L'?');
  wchar_t *esclamation = wctowcs(L'!');
  wchar_t *dot = wctowcs(L'.');

  push(words, wctowcs(L'.'));

  do {
    wchar_t c = *str;

    int is_alpha = iswalpha(c) || iswdigit(c),
        is_special = c == L'?' || c == L'!' || c == L'.';

    if (is_alpha)
      *str = towlower(*str);

    if (is_word) {
      if (!is_alpha) {
        push(words, text + token);
        *str = 0;
      }
    } else if (is_alpha)
      token = counter;

    if (is_special) {
      switch (c) {
      case L'?':
        push(words, question);
        break;
      case L'!':
        push(words, esclamation);
        break;
      case L'.':
        push(words, dot);
        break;
      }
    }

    is_word = is_alpha;
    counter++;
  } while (*++str);

  return words;
}

// Vector *parse(wchar_t *str) {
//   Vector *words = vec();
//   String *token;
//
//   int is_word = 0;
//
//   push(words, wctowcs(L'.'));
//
//   do {
//     wchar_t c = towlower(*str);
//
//     int is_alpha = iswalpha(c) || iswdigit(c),
//         is_special = c == L'?' || c == L'!' || c == L'.';
//
//     if (is_word) {
//       if (is_alpha || c == '\'')
//         append(token, c);
//       if (!is_alpha)
//         push(words, token->str);
//     } else if (is_alpha)
//       token = wctostr(c);
//
//     if (is_special)
//       push(words, wctowcs(c));
//
//     is_word = is_alpha;
//   } while (*++str);
//
//   return words;
// }

typedef struct Word {
  size_t count;
  RBTree *adjacent;
} Word;

/* */
RBTree *count(Vector *words) {
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
    // size_t count = (size_t)value(word->adjacent, next, sizeof(size_t));
    // (*count)++;
    // size_t *count = value(word->adjacent, next, sizeof(size_t));
    // (*count)++;
  }

  return counter;
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
      // wprintf(L",%ls,%g", next->key,
      //         (float)*((size_t *)next->value) / (float)word->count);
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
