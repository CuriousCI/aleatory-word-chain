#define _GNU_SOURCE
#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "codex.h"
#include "csv.h"
#include "map.h"
#include "text.h"
#include "vec.h"

/* EOC = End Of Communication */

/* Redefined to match 'int (*collect)(void *, char *)' API. */
int _push(void *vec, char *item) {
    return push(vec, item);
}

/* Redefined to match 'int (*collect)(void *, char *)' API. */
int _fprintf(void *stream, char *buf) {
    return fprintf(stream, "%s\n", buf) < 0 ? -1 : 0;
}

/* Redefined to match 'char *(*consume)(void *)' API. */
char *_getstr(void *stream) {
    char *lineptr = NULL;
    size_t n = 0, read;

    read = getline(&lineptr, &n, stream);
    /* EMPTY byte means EOC. */
    if (*lineptr == '\n')
        return NULL;

    lineptr[read - 1] = 0;
    return lineptr;
}

/* Redefined to match 'char *(*consume)(void *)' API. */
char *_next(void *iter) {
    return next(iter);
}

/* Quality of life improvement. */
void pipe3(FILE **out, FILE **in) {
    int pipedes[2];

    if (pipe(pipedes) == -1)
        panic("couldn't open pipe");

    if ((*in = fdopen(pipedes[0], "r")) == NULL)
        panic("couldn't open file on read pipe");

    if ((*out = fdopen(pipedes[1], "w")) == NULL)
        panic("couldn't open file on write pipe");
}

int main(int argc, char **argv) {
    flags_t flags;
    FILE *stream = stdin, *out = stdout;

    getflags(argc, argv, &flags);

    setlocale(LC_ALL, flags.locale);
    /* Required as some locales use commas for decimal numbers. */
    setlocale(LC_NUMERIC, "en_US.UTF-8");

    if (flags.help) {
        help(&flags);
        exit(EXIT_SUCCESS);
    }

    if (flags.version) {
        version();
        exit(EXIT_SUCCESS);
    }

    if (flags.file && (stream = fopen(flags.file, "r")) == NULL)
        panic("input file");

    if (flags.output && (out = fopen(flags.output, "w")) == NULL)
        panic("output file");

    /* Single process. Uses the same API as the multi-process solution. */
    if (!flags.parallel && flags.csv) {
        struct vec_t *words = vec();
        struct map_t *counters = map();

        split(stream, _push, words);
        count(counters, _next, iter(words));
        csv(counters, out);

        exit(EXIT_SUCCESS);
    }

    /* Single process. Uses the same API as the multi-process solution. */
    if (!flags.parallel && flags.text) {
        struct vec_t *rows = vec();
        struct map_t *csv = map();

        lines(stream, _push, rows);
        insert(csv, _next, iter(rows));
        text(csv, out, flags.number, flags.word);

        exit(EXIT_SUCCESS);
    }

    if (flags.parallel) {
        pid_t pid = 0;
        FILE *parser_out, *counter_in, *counter_out, *printer_in;

        pipe3(&parser_out, &counter_in);
        if ((pid = fork()) == -1)
            panic("fork _parser");

        /* PROCESS 1. */
        if (pid == 0) {
            if (flags.csv)
                /* Reads input and splits the words and sends them to counter. */
                split(stream, _fprintf, parser_out);
            if (flags.text)
                /* Reads input and splits the lines and sends them to generator. */
                lines(stream, _fprintf, parser_out);

            /* EOC */
            fprintf(parser_out, "\n");
            fflush(parser_out);

            exit(EXIT_SUCCESS);
        }

        pipe3(&counter_out, &printer_in);
        if ((pid = fork()) == -1)
            panic("fork _counter");

        /* PROCESS 2. */
        if (pid == 0) {
            if (flags.csv) {
                /* Counts the words and converts the counters to CSV lines, which are sent the printer. */
                struct map_t *counters = map();
                count(counters, _getstr, counter_in);
                csv(counters, counter_out);
            }

            if (flags.text) {
                /* Builds the csv and generates the text which is sent to the printer. */
                struct map_t *csv = map();
                insert(csv, _getstr, counter_in);
                text(csv, counter_out, flags.number, flags.word);
            }

            fprintf(counter_out, "\n");
            fflush(counter_out);

            /* EOC */

            exit(EXIT_SUCCESS);
        }

        /* PROCESS 3. Prints results from PROCESS 2. */
        {
            char *lineptr = NULL;
            size_t n = 0;

            while (1) {
                if (flags.csv)
                    getline(&lineptr, &n, printer_in);
                else if (flags.text)
                    getdelim(&lineptr, &n, ' ', printer_in);

                if (*lineptr == '\n') break;

                fprintf(out, "%s", lineptr);
                lineptr = NULL;
            }
            exit(EXIT_SUCCESS);
        }
    }

    exit(EXIT_SUCCESS);
}
