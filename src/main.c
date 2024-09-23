#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "awc/csv.h"
#include "awc/text.h"
#include "awc/util.h"
#include "collections/map.h"
#include "collections/vec.h"

/* Delimiter to separate WORDS for inter-process communication */
#define DELIMITER '\1'

/* Redefined to match 'int (*collect)(void *, char *)'. */
int _push(void *vec, char *item) {
    return push(vec, item);
}

/* Redefined to match 'int (*collect)(void *, char *)'. */
int _fprintdelim(void *stream, char *buf) {
    return fprintf(stream, "%s%c", buf, DELIMITER) < 0 ? -1 : 0;
}

/* Redefined to match 'int (*collect)(void *, char *)'. */
int _fprintf(void *stream, char *buf) {
    return fprintf(stream, "%s", buf) < 0 ? -1 : 0;
}

/* Redefined to match 'char *(*consume)(void *)'. */
char *_next(void *iter) {
    return next(iter);
}

/* Redefined to match 'char *(*consume)(void *)'. */
char *_getdelim(void *stream) {
    char *lineptr = NULL;
    size_t n = 0, read;

    read = getdelim(&lineptr, &n, DELIMITER, stream);
    /* End of communication. */
    if (*lineptr == DELIMITER)
        return NULL;

    /* NULL terminate LINEPTR. */
    lineptr[read - 1] = 0;
    return lineptr;
}

/* Quality of life improvement. I thought the name would be funny :) */
void pipe3(FILE **out, FILE **in) {
    int pipedes[2];

    if (pipe(pipedes) == -1)
        expect("couldn't open pipe");

    if ((*in = fdopen(pipedes[0], "r")) == NULL)
        expect("couldn't open file on read pipe");

    if ((*out = fdopen(pipedes[1], "w")) == NULL)
        expect("couldn't open file on write pipe");
}

int main(int argc, char **argv) {
    flags_t flags;
    /* By defaults reads from stdin and writes to stdout to mike the command usable with redirection. */
    FILE *in = stdin, *out = stdout;
    char *locale = NULL;

    /* getpriority(2) sets errno to 0 and then modifies it to signal errors. I decided to do the same for this program. */
    errno = 0;

    getflags(argc, argv, &flags);

    locale = setlocale(LC_ALL, flags.locale);
    if (locale == NULL || (!strcmp(flags.locale, "") && !strcmp(flags.locale, locale)))
        expect("locale not supported!");

    /* Some locales use commas for decimal numbers. C.utf8 is guaranteed to exist and uses dots. */
    setlocale(LC_NUMERIC, "C.utf8");

    if (flags.help) {
        help();
        exit(EXIT_SUCCESS);
    }

    if (flags.version) {
        version();
        exit(EXIT_SUCCESS);
    }

    if (flags.file && (in = fopen(flags.file, "r")) == NULL)
        expect("unable to open input file");

    if (flags.output && (out = fopen(flags.output, "w")) == NULL)
        expect("unable to open output file");

    /* SINGLE-PROCESS */
    if (!flags.parallel) {
        struct vec_t *_vec;
        struct map_t *_map;

        if ((_vec = vec()) == NULL)
            expect("Unable to initialize vec for words/lines");

        if ((_map = map()) == NULL)
            expect("Unable to initialize map for csv");

        if (flags.csv) {
            /* TEXT to CSV conversion. */

            if (split_words(in, _push, _vec) == -1)
                expect("unable to split input stream in words");

            if (fclose(in) == -1) panic();

            if (count_words(_map, _next, iter(_vec)) == -1)
                expect("unable to count words");

            if (csv(_map, _fprintf, out) == -1)
                expect("unable to generate csv");

        } else if (flags.text) {
            /* CSV to TEXT conversion. */

            if (split_lines(in, _push, _vec) == -1)
                expect("unable to split input stream in lines!");

            if (fclose(in) == -1) panic();

            if (insert_lines(_map, _next, iter(_vec)) == -1)
                expect("unable to parse csv lines!");

            if (text(_map, _fprintf, out, flags.number, flags.word))
                expect("unable to generate text! Start WORD not found in CSV or punctuation is missing.");

            if (fprintf(out, "\n") == -1) panic();
        }

        if (fflush(out)) panic();
        if (fclose(out) == -1) panic();

        exit(EXIT_SUCCESS);
    }

    /* PARALLEL-PROCESSES */
    if (flags.parallel) {
        pid_t pid = 0;
        FILE *first_out, *second_in, *second_out, *third_in;

        pipe3(&first_out, &second_in);
        if ((pid = fork()) == -1) expect("fork parser/lines");

        /* PROCESS 1. */
        if (pid == 0) {
            if (fclose(second_in)) panic();

            if (flags.csv) {
                /* Reads input and splits the words and sends them to counter. */
                if (split_words(in, _fprintdelim, first_out) == -1)
                    expect("unable to split input stream in words");
            } else if (flags.text) {
                /* Reads input and splits the lines and sends them to generator. */
                if (split_lines(in, _fprintdelim, first_out) == -1)
                    expect("unable to split lines");
            }

            /* End of communication */
            if (fflush(first_out)) panic();
            if (fprintf(first_out, "%c", DELIMITER) < 0) panic();
            if (fflush(first_out)) panic();
            if (fclose(first_out)) panic();

            exit(EXIT_SUCCESS);
        }

        if (fclose(first_out)) panic();
        pipe3(&second_out, &third_in);

        if ((pid = fork()) == -1) expect("fork counter/generator");

        /* PROCESS 2. */
        if (pid == 0) {
            struct map_t *_map;

            fclose(third_in);

            if ((_map = map()) == NULL)
                expect("unable to initialize map csv");

            if (flags.csv) {
                /* Counts the words and converts the counters to CSV lines, which are sent the PROCESS 3. */

                if (count_words(_map, _getdelim, second_in) == -1)
                    expect("unable to count words");

                if (csv(_map, _fprintdelim, second_out) == -1)
                    expect("unable to generate csv");

            } else if (flags.text) {
                /* Builds the csv and generates the text which is sent to the PROCESS 3. */
                if (insert_lines(_map, _getdelim, second_in) == -1)
                    expect("unable to parse csv lines!");

                if (text(_map, _fprintdelim, second_out, flags.number, flags.word) == -1)
                    expect("Unable to generate text! Starting WORD not found in CSV or PUNCTUATION is missing in CSV.");
            }

            /* End of communication */
            if (fprintf(second_out, "%c", DELIMITER) < 0) panic();
            if (fflush(second_out)) panic();

            if (fclose(second_in)) panic();
            if (fclose(second_out)) panic();

            exit(EXIT_SUCCESS);
        }

        /* PROCESS 3. Prints results from PROCESS 2. */
        {
            char *word;
            while ((word = _getdelim(third_in)))
                if (fprintf(out, "%s", word) < 0) panic();

            if (flags.text)
                if (fprintf(out, "\n") == -1) panic();

            if (fflush(out)) panic();
            if (fclose(out)) panic();
            if (fclose(third_in)) panic();

            exit(EXIT_SUCCESS);
        }
    }

    exit(EXIT_SUCCESS);
}
