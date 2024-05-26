#include "codex.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void panic(char *error) {
    if (errno == 0)
        fprintf(stderr, "%s", error);
    else
        perror(error);

    exit(EXIT_FAILURE);
}

void expect(int value, char *error) {
    if (value == -1)
        panic(error);
}

void help(flags_t *flags) {
    if (flags == NULL)
        fprintf(
            stderr,
            "Usage: codex ...\n"
            "OPTIONS\n"
            " -h, --help       \t\tPrint usage information and exit\n"
            " -v, --version    \t\tPrint version information and exit\n"
            " -c, --csv        \t\tconvert input to csv\n"
            " -t, --text=NUMBER\t\tgenerate text of NUMBER words\n"
            " -f, --file=FILE  \t\tinput file\n"
            " -o, --output=FILE\t\toutput file\n"
            " -w, --word=WORD  \t\tgenerate text starting from WORD word\n"
            " -l, --locale     \t\tset locale of input file (Ex. it_IT.UTF-8)\n"
            " -p, --parallel   \t\trun this command using multiple processes\n"
        );
}

void version() {
    fprintf(
        stderr,
        "codex (Sapienza coreutils)\n 1.0"
        "Copyright (C) 2024 Free Software Foundation, Inc.\n"
        "License GPLv3+: GNU GPL version 3 ...\n\n"
        "Written by Cicio Ionut.\n"
    );
}

void getflags(int argc, char *argv[], flags_t *flags) {
    int option;
    struct option options[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"csv", no_argument, NULL, 'c'},
        {"text", no_argument, NULL, 't'},
        {"number", required_argument, NULL, 'n'},
        {"word", required_argument, NULL, 'w'},
        {"parallel", no_argument, NULL, 'p'},
        {"file", required_argument, NULL, 'f'},
        {"output", required_argument, NULL, 'o'},
        {"locale", required_argument, NULL, 'l'},
        {NULL, no_argument, NULL, '-'},
    };

    flags->locale = "";
    flags->word = NULL;
    flags->file = NULL;
    flags->output = NULL;

    while ((option = getopt_long(argc, argv, "hvctn:w:pf:o:l:", options, NULL)) != -1) {
        switch (option) {
            case 'v':
                flags->version = true;
                break;
            case 'c':
                flags->csv = true;
                break;
            case 't':
                flags->text = true;
                break;
            case 'n':
                flags->number = atoi(optarg);
                if (flags->number < 0)
                    flags->help = true;
                break;
            case 'w':
                flags->word = optarg;
                break;
            case 'p':
                flags->parallel = true;
                break;
            case 'f':
                flags->file = optarg;
                break;
            case 'o':
                flags->output = optarg;
                break;
            case 'l':
                flags->locale = optarg;
                break;
            default:
                flags->help = true;
        }
    }
}
