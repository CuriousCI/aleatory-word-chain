#include "decoder.h"
#include "rbtree.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { WORD, KEY, VALUE } Csv;

// TODO: double instead of float? Maybe long double?
typedef struct Link {
  Node *node;
  float frequency;
} Link;

RBTree *parse_csv_(wchar_t *str) {
  RBTree *graph = tree();
  String *token = string();

  Csv state = WORD;

  // 1 / 100_000
  // 1 / 1_000_000

  // word:
  //    - next: frequency
  //    - next: frequency
  //    - next: frequency

  do {
    wchar_t c = *str;
    wchar_t *state;

    // wchar_t *line_state;
    // wchar_t *line = wcstok(str, L"\n", &line_state);

    // word:
    wchar_t *word = wcstok(str, L",\n", &state);
    fprintf(stderr, "%c", state[-1]);

    wchar_t *next;
    while ((next = wcstok(NULL, L"\n,", &state)) != NULL) {
      wchar_t *frequency = wcstok(NULL, L"\n,", &state);
    }

    // it should work... but i parse the line twice...

    //  - next: frequency, how many?????

    // until \n

    // if (c == ',' || c == '\n') {
    //   switch (state) {
    //   case WORD: {
    //     Node *current_word_node = node(graph, token->str);
    //     if (current_word_node->value == NULL)
    //       current_word_node->value = tree();
    //     current_word = current_word_node->value;
    //   }
    //
    //     state = KEY;
    //     break;
    //
    //   case KEY:
    //     next_word = node(graph, token->str);
    //
    //     state = VALUE;
    //     break;
    //
    //   case VALUE:
    //     link = value(current_word, next_word->key, sizeof(Link));
    //     link->node = next_word;
    //     link->frequency = wcstof(token->str, 0);
    //
    //     state = c == ',' ? KEY : WORD; // either ',' or '\n'
    //     break;
    //   }
    //
    //   token = string(); // reset token
    // } else
    //   append(token, c);

    str++;
  } while (*str);

  return graph;
}

/* */
RBTree *parse_csv(wchar_t *str) {
  parse_csv_(str);
  RBTree *graph = tree();
  String *token = string();

  Csv state = WORD;

  RBTree *current_word;
  Node *next_word;
  Link *link;

  do {
    wchar_t c = *str;

    if (c == ',' || c == '\n') {
      switch (state) {
      case WORD: {
        Node *current_word_node = node(graph, token->str);
        if (current_word_node->value == NULL)
          current_word_node->value = tree();
        current_word = current_word_node->value;
      }

        state = KEY;
        break;

      case KEY:
        next_word = node(graph, token->str);

        state = VALUE;
        break;

      case VALUE:
        link = value(current_word, next_word->key, sizeof(Link));
        link->node = next_word;
        link->frequency = wcstof(token->str, 0);

        state = c == ',' ? KEY : WORD; // either ',' or '\n'
        break;
      }

      token = string(); // reset token
    } else
      append(token, c);

    str++;
  } while (*str);

  return graph;
}

/* */
wchar_t *generate(RBTree *csv, size_t words) {
  srand(time(NULL));

  RBTree *links = value(csv, L".", sizeof(RBTree));

  int capitalize = 0;
  for (int i = 0; i < words; i++) {
    float rng = rand() / (float)RAND_MAX;
    float total = 0;

    for (int j = 0; j < links->nodes->len; j++) {
      Node *l = get(links->nodes, j);

      if (total >= rng - 10e-6)
        break;

      total += *(float *)l->value;
    }
  }

  return NULL;
}

/* */
wchar_t *generate_from(RBTree *csv, size_t words, wchar_t *word) {
  return NULL;
}
