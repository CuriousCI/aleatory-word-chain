#ifndef CODEX_H_
#define CODEX_H_

#include <stdbool.h>
#include <stdio.h>

#include "map.h"

typedef struct flags_t {
    bool help, version, csv, parallel, text;
    int number;
    char *locale, *word, *file, *output;
} flags_t;

/* I'm feeling Rust today. Log error and exit(EXIT_FAILURE).  */
void panic(char *error);

/* Read flags */
void getflags(int argc, char *argv[], flags_t *flags);

/* Print usage information and exit. */
void help(flags_t *flags);

/*Print version information and exit. */
void version();

#endif /* codex.h */
