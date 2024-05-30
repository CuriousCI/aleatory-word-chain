#include "util.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void expect(char *message) {
    if (errno)
        perror(message);
    else
        fprintf(stderr, "%s\n", message);

    exit(EXIT_FAILURE);
}

void panic() {
    if (errno)
        perror("");

    exit(EXIT_FAILURE);
}

void help() {
    fprintf(
        stderr,
        "Usage: SRC | codex --csv > DST (job 1)\n"
        "   or: codex --csv -f SRC -o DST (job 1)\n"
        "   or: codex --text -n 1000 -f SRC -p > DST (job 2)\n"
        "   or: codex --csv -f SRC -p --locale it_IT.UTF-8 > DST (job 2)\n"
        "   or: codex --text -n 1000 -f SRC -p -w ciao > DST (job 2)\n"
        "\n"
    );

    fprintf(
        stderr,
        "OPTIONS\n"
        " -h, --help        print usage information and exit\n"
        " -v, --version     print version information and exit\n"
        "\n"
        " CSV\n"
        " -c, --csv         generate CSV from SRC into DST\n"
        "\n"
        " TEXT\n"
        " -t, --text        generate TEXT from SRC into DST\n"
        " -n, --number=NUM  generate TEXT with NUM words (NUM >= 0)\n"
        " -w, --word=WORD   generate TEXT starting from WORD\n"
        "\n"
    );

    fprintf(
        stderr,
        " GENERAL\n"
        " -f, --file=FILE   input FILE\n"
        " -o, --output=FILE output FILE\n"
        " -p, --parallel    run using multiple processes\n"
        " -l, --locale      set locale of input file (Ex. it_IT.UTF-8)\n"
    );
}

void version() {
    fprintf(
        stderr,
        "codex (SO2) 1.0\n"
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

    flags->help = false;
    flags->version = false;
    flags->csv = false;
    flags->parallel = false;
    flags->text = false;
    flags->number = 0;
    flags->locale = "";
    flags->word = NULL;
    flags->file = NULL;
    flags->output = NULL;

    while ((option = getopt_long(argc, argv, "hvctn:w:pf:o:l:", options, NULL)) != -1) {
        switch (option) {
            case 'h':
                flags->help = true;
                break;
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

    /* If no action is requested, print usage information and exit. */
    if (!flags->csv && !flags->text && !flags->version)
        flags->help = true;

    if (flags->csv && flags->text)
        expect("Can't run both --csv and --text");

    if (flags->text && flags->number <= 0)
        expect("--number=INTEGER>0 required with --text");
}
