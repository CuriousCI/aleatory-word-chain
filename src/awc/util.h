#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>
#include <stdio.h>

#include "../collections/map.h"

typedef struct flags_t {
    bool help, version, csv, parallel, text;
    int number;
    char *locale, *word, *file, *output;
} flags_t;

/* I'm feeling Rust today. Log error and exit(EXIT_FAILURE).  */
void expect(char *message);

/* I'm feeling Rust today. Log perror if present and exit(EXIT_FAILURE). It's useful for errors which are system related, set errno and don't have anything to do with the user input. */
void panic();

/* Read flags */
void getflags(int argc, char *argv[], flags_t *flags);

/* Print usage information and exit. */
void help();

/* Print version information and exit. */
void version();

#endif /* util.h */
