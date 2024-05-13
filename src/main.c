#define _GNU_SOURCE
#include "decoder.h"
#include "encoder.h"
#include "string.h"
#include <fcntl.h>
#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");

  int help = 0, csv = 0, text = 0, parallel = 0;
  int number = 0;
  char *file = NULL, *output = NULL, *word = NULL;

  struct option options[] = {
      {"help", no_argument, 0, 0},         {"csv", no_argument, 0, 0},
      {"text", required_argument, 0, 0},   {"word", required_argument, 0, 0},
      {"parallel", no_argument, 0, 0},     {"file", required_argument, 0, 0},
      {"output", required_argument, 0, 0}, {0, 0, 0, 0},
  };

  // words count!

  int option, index;
  while ((option = getopt_long(argc, argv, "hcg:w:p", options, &index)) != -1) {
    if (option == 0 && index > 0 && index < 5)
      option = "hctwpfo"[index];

    switch (option) {
    case 'h':
      help = 1;
      break;
    case 'c':
      csv = 1;
      break;
    case 't':
      text = 1;
      number = atoi(optarg);
      if (number < 0)
        fprintf(stderr, "--text %d :text must have at least 0 words\n", number);
      break;
    case 'w':
      word = optarg;
      break;
    case 'p':
      parallel = 1;
      break;
    case 'f':
      file = optarg;
      break;
    case 'o':
      output = optarg;
      break;
    default:
      help = 1;
    }
  }

  if (!(csv || text))
    help = 1;

  if (help)
    fprintf(stderr,
            "Usage: codex ...\n"
            "OPTIONS\n"
            " -h, --help            \t\tshow this menu\n"
            " -c, --csv             \t\tconvert input to csv\n"
            " -t, --text=NUMBER     \t\tgenerate NUMBER words from stdin\n"
            " -f, --file=FILE       \t\tinput file\n"
            " -o, --output=FILE     \t\toutput file\n"
            " -w, --word=WORD       \t\tgenerate starting from WORD word\n"
            " -p, --parallel        \t\trun this command using multiple "
            "processes\n");
  else if (parallel) {
    if (csv) {
    } else if (text) {
      if (word) {
      } else {
      }
    }
  } else {

    int start = clock();
    wchar_t c;
    String *input = string();

    if (file) {
      // INPUT FILE
    }

    if (output) {
      // OUTPUT FILE
    }

    while ((c = getwc(stdin)) != WEOF && !ferror(stdin))
      append(input, c);
    wchar_t *str = input->str;
    int end = clock();

    fprintf(stderr, "READING file - %g\n",
            (float)(end - start) / (float)CLOCKS_PER_SEC);

    if (csv) {
      start = clock();
      Vector *words = parse(str);
      end = clock();
      fprintf(stderr, "SEPARATING words - %g\n",
              (float)(end - start) / (float)CLOCKS_PER_SEC);

      start = clock();
      RBTree *counter = count(words);
      end = clock();
      fprintf(stderr, "COUNTING words - %g\n",
              (float)(end - start) / (float)CLOCKS_PER_SEC);

      start = clock();
      save(counter);
      end = clock();
      fprintf(stderr, "SAVING words - %g\n",
              (float)(end - start) / (float)CLOCKS_PER_SEC);

      // save(count(parse(str)));
    } else if (text) {
      if (word) {
        size_t length = strlen(word);
        wchar_t *start = calloc(length, sizeof(wchar_t));
        mbstowcs(start, word, length);
        generate_from(parse_csv(str), number, start);
      } else
        generate(parse_csv(str), number);
    }
  }

  exit(EXIT_SUCCESS);
}

// pipe2(NULL, O_DIRECT);
