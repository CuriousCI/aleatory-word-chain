#include "word_graph.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
// #include <time.h>
#include <unistd.h>
#include <wchar.h>

int main(int argc, char **argv) {
  // TODO: getopt

  // char x;
  // while ((x = getopt(argc, argv, "po")) != -1) {
  //   fprintf(stderr, "%c\n", x);
  // }

  setlocale(LC_ALL, "");
  size_t len = 1;
  wchar_t *slice = calloc(len, sizeof(wchar_t));
  wchar_t c;

  while ((c = getwc(stdin)) != WEOF && !ferror(stdin)) {
    slice = realloc(slice, ++len * sizeof(wchar_t));
    slice[len - 2] = c;
    slice[len - 1] = 0;
  }

  if (argv[1][0] == '1')
    print_table(slice);
  else
    generate_text(graph_from_csv(slice), atoi(argv[2]));

  exit(EXIT_SUCCESS);
}

// Vector *graph = graph_from_csv(slice);
// srand(time(NULL));
// TODO: if there is an argument word, start from it
// srand(time(NULL));

// float start_time = (float)clock() / CLOCKS_PER_SEC;
// float end_time = (float)clock() / CLOCKS_PER_SEC;
// printf("%f\n", end_time - start_time);

// setlocale(LC_CTYPE, "it_IT.UTF-8"); // put your locale here

// key:
//  - left
//  - right
// value:
//  - count
//
//  sentence ? . !
//  word ' '
// Quello delle parole non è altro che un grafo pesato con la frequenza?
