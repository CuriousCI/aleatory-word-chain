#include "string.h"
#include "vector.h"
#include "word_graph.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include <time.h>

// void to_lowercase(char *string) {
//   while (*string) {
//     if (*string >= 'A' && *string <= 'Z')
//       // tolower(*string);
//       *string = *string - 'A' + 'a';
//     string++;
//   }
// }

int is_special(char c) { return c == '?' || c == '!' || c == '.'; }

int is_letter(char c) {
  // TODO: lettere accentate italiane se possibile / utf-8 letters?
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '\'';
}

typedef enum { SPECIAL, SEPARATOR, LETTER } Character;

Vector *parse(char *string) {
  Vector *words = vec();

  String *word = str();
  append(word, '.');
  push(words, word->slice);

  Character state = SPECIAL;

  do {
    *string = tolower(*string);

    switch (state) {
    case SPECIAL:
      if (is_special(*string)) {
        append(word, *string);
        push(words, word->slice);
      } else if (is_letter(*string)) {
        word = str();
        append(word, *string);
        state = LETTER;
      } else
        state = SEPARATOR;
      break;
    case LETTER:
      if (is_special(*string)) {
        push(words, word->slice);
        word = str();
        append(word, *string);
        push(words, word->slice);
        state = SPECIAL;
      } else if (is_letter(*string)) {
        append(word, *string);
      } else {
        push(words, word->slice);
        state = SEPARATOR;
      }
      break;
    case SEPARATOR:
      if (is_special(*string)) {
        word = str();
        append(word, *string);
        push(words, word->slice);

        state = SPECIAL;
      } else if (is_letter(*string)) {
        word = str();
        append(word, *string);

        state = LETTER;
      }
      break;
    }

    string++;
  } while (*string);

  word = str();
  append(word, '.');
  push(words, word->slice);

  return words;
}

// key:
//  - left
//  - right
// value:
//  - count
//
//  sentence ? . !
//  word ' '

typedef struct Frequency {
  char *key;
  int frequency;
} Frequency;

Vector *frequency(Vector *words) {
  Vector *frequencies = vec();

  for (int i = 0; i < words->len; i++) {
    int found = 0;

    for (int j = 0; j < frequencies->len; j++) {
      Frequency *f = frequencies->data[j];

      if (strcmp(f->key, words->data[i]) == 0) {
        f->frequency++;
        found = 1;
        break;
      }
    }

    if (!found) {
      Frequency *f = malloc(sizeof(Frequency));
      f->key = words->data[i];
      f->frequency = 1;
      push(frequencies, f);
    }
  }

  return frequencies;
}

typedef struct Record {
  char *word;
  Vector *frequencies;
  size_t count;
} Record;

int compare_records(void *a, void *b) {
  return strcmp(((Record *)a)->word, ((Record *)b)->word);
}

int compare_frequencies(void *a, void *b) {
  return strcmp(((Frequency *)a)->key, ((Frequency *)b)->key);
}

Vector *table(Vector *words) {
  Vector *table = vec();

  for (int i = 0; i < words->len - 1; i++) {
    Record *r = malloc(sizeof(Record));
    r->word = get(words, i);
    r->frequencies = vec();
    r->count = 0;
    r = insert(table, r, compare_records);
    r->count++;

    Frequency *f = malloc(sizeof(Frequency));
    f->key = get(words, i + 1);
    f->frequency = 0;
    f = insert(r->frequencies, f, compare_frequencies);
    f->frequency++;
  }

  return table;
}

typedef struct Successor {
  char *key;
  float frequency;
} Successor;

// typedef struct Word {
//   char *key;
//   Vector *successors; // Vector<Edge>
// } Word;
//
// typedef struct Edge {
//   Word *word;
//   float frequency;
// } Edge;

// typedef struct Word {
//   char *key;
//   Vector *successors; // Vector<Edge>
// } Word;

//
// typedef struct Bucket {
//   Vector *items;
// } Bucket;

// hashing function for strings?

typedef enum { WORD, KEY, VALUE } Csv;

// Vector<Record>
// Vector *parse_table(char *string) {
//   Vector *table = vec();
//   Csv csv = WORD;
//
//   String *word = str();
//   Word *i;
//   Successor *s;
//
//   while (*string) {
//     switch (csv) {
//     case WORD:
//       if (*string == ',') {
//         i = malloc(sizeof(Word));
//         i->key = word->slice;
//         i->successors = vec();
//         push(table, i);
//         word = str();
//         csv = KEY;
//       } else
//         append(word, *string);
//       break;
//
//     case KEY:
//       if (*string == ',') {
//         s = malloc(sizeof(Successor));
//         s->key = word->slice;
//         word = str();
//         push(i->successors, s);
//         csv = VALUE;
//       } else
//         append(word, *string);
//       break;
//
//     case VALUE:
//       if (*string == ',') {
//         s->frequency = atof(word->slice);
//         word = str();
//         csv = KEY;
//       } else if (*string == '\n') {
//         s->frequency = atof(word->slice);
//         word = str();
//         csv = WORD;
//       } else
//         append(word, *string);
//       break;
//     }
//     string++;
//   }
//
//   return table;
// }

// Quello delle parole non è altro che un grafo pesato con la frequenza?

int main(int argc, char **argv) {
  // TODO: getopt

  if (argv[1][0] == '1') {
    int length = 584 * 1024 * 1024;
    char *string = malloc(length * sizeof(char));
    memset(string, 0, length);

    FILE *file = fopen("tests/test_5", "r");
    // FILE *file = fopen("tests/test_1", "r");
    fread(string, sizeof(char), length - 1, file);

    Vector *words = parse(string);
    Vector *t = table(words);

    for (int i = 0; i < t->len; i++) {
      Record *r = get(t, i);
      printf("%s", r->word);
      for (int j = 0; j < r->frequencies->len; j++) {
        Frequency *f = get(r->frequencies, j);
        printf(",%s,%g", f->key, (float)f->frequency / (float)r->count);
      }
      printf("\n");
    }
  } else {
    int len2 = 980 * 1024 * 1024;
    char *outfile = calloc(len2, sizeof(char));
    FILE *fout = fopen("output", "r");
    fread(outfile, sizeof(char), len2 - 2, fout);
    outfile[len2 - 1] = 0;

    Vector *graph = graph_from_string(outfile);

    // for (size_t i = 0; i < graph->len; i++) {
    //   Word *word = get(graph, i);
    //   printf("%s\n\t", word->slice);
    //   for (size_t j = 0; j < word->links->len; j++) {
    //     Link *link = get(word->links, j);
    //     printf("%s: %g; ", link->word->slice, link->frequency);
    //   }
    //   printf("\n");
    // }

    // if there is an argument word, start from it
    srand(time(NULL));

    Word *word = get(graph, 0);
    for (int i = 0; i < graph->len; i++) {
      Word *w = get(graph, i);
      if (strcmp(w->slice, "la") == 0) {
        word = w;
        break;
      }
    }

    int capitalize = 0;
    for (int i = 0; i < 100; i++) {
      float rng = rand() / (float)RAND_MAX;
      float total = 0;

      Link *link = get(word->links, 0);
      for (int j = 0; j < word->links->len; j++) {
        Link *l = get(word->links, j);

        if (total + l->frequency > rng)
          break;

        total += l->frequency;
        link = l;
      }

      if (capitalize) {
        link->word->slice[0] = toupper(link->word->slice[0]);
        capitalize = 0;
      }

      printf("%s ", link->word->slice);

      if (is_special(link->word->slice[0]))
        capitalize = 1;

      word = link->word;
    }

    // Vector *t = parse_table(outfile);
    // Word *word = get(t, rand() % t->len);
    //
    // for (int i = 0; i < 1000; i++) {
    //   Successor *s = get(word->successors, rand() % word->successors->len);
    //   for (int j = 0; j < t->len; j++) {
    //     Word *i = get(t, j);
    //     if (strcmp(i->key, s->key) == 0) {
    //       word = i;
    //       break;
    //     }
    //   }
    //   printf("%s ", word->key);
    // }
  }

  exit(EXIT_SUCCESS);
}

// float start_time = (float)clock() / CLOCKS_PER_SEC;
// float end_time = (float)clock() / CLOCKS_PER_SEC;
// printf("%f\n", end_time - start_time);
// to_lowercase(string);
