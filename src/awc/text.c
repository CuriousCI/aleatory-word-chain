#include "text.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

#include "../collections/map.h"
#include "../collections/vec.h"
#include "util.h"

/* Splits STREAM into LINES which collects in a COLLECTION */
int split_lines(FILE *stream, int (*collect)(void *, char *), void *collection) {
    char *lineptr = NULL;
    size_t n = 0;
    int read = 0;

    while ((read = (getline(&lineptr, &n, stream))) != -1) {
        if (ferror(stream))
            return -1;

        lineptr[read - 1] = 0;
        if (collect(collection, lineptr) == -1)
            return -1;

        lineptr = NULL;
    }

    return 0;
}

/* ROW in CSV */
static struct row_t {
    /* MAP { WORD: FREQUENCY } */
    struct map_t *words;
} row_t;

int insert_lines(struct map_t *csv, char *(*consume)(void *), void *collection) {
    char *line;

    while ((line = consume(collection))) {
        struct entry_t *word_entry;
        struct row_t *word_row;
        char *word, *next, *frequency;
        void **val;
        float total = 0;

        /* First WORD in line is the WORD */
        if ((word = strtok(line, ",\n\0")) == NULL)
            expect("unable to parse CSV, check for empty lines");

        /* Create an associated ENTRY in the CSV MAP.  */
        if ((word_entry = find(csv, word)) == NULL)
            return -1;

        /* Make a new ROW in the ENTRY if there isn't ONE. */
        if (*(val = value(word_entry)) == NULL)
            if ((*val = calloc(1, sizeof(row_t))) == NULL)
                return -1;

        word_row = *val;

        if (word_row->words == NULL)
            if ((word_row->words = map()) == NULL)
                return -1;

        /* Parse line */
        while ((next = strtok(NULL, ",\n\0")) && (frequency = strtok(NULL, ",\n"))) {
            struct entry_t *next_entry;
            float f;

            if ((next_entry = find(word_row->words, next)) == NULL)
                return -1;

            f = atof(frequency);
            *(float *)value(next_entry) = f;
            total += f;
        }

        /* I prefer a bigger tollerance. */
        if (total < 1 - 0.05) {
            fprintf(stderr, "On word \"%s\"\n", key(word_entry));
            expect("CSV line has total frequency < 1 - 0.05");
        }
    }

    return 0;
}

int text(struct map_t *csv, int (*collect)(void *, char *), void *collection, size_t n, char *start) {
    struct entry_t *word_entry;
    bool capitalize = true;
    char *word;
    size_t i;

    if (n < 1)
        return -1;

    srand(time(NULL));

    if (start != NULL) {
        /* Temporary wide char string to lowercase start word. */
        wchar_t *pwcs;
        size_t n;

        /* Convert START WORD to UNICODE */
        if ((n = mbstowcs(NULL, start, 0)) == (size_t)-1)
            return -1;

        if ((pwcs = calloc(n + 1, sizeof(wchar_t))) == NULL)
            return -1;

        if (mbstowcs(pwcs, start, n) == (size_t)-1)
            return -1;

        /* Map to lowercase. */
        for (i = 0; i < n; i++)
            pwcs[i] = towlower(pwcs[i]);

        /* Convert UNICODE WORD to UTF-8. */
        if ((n = wcstombs(NULL, pwcs, 0)) == (size_t)-1)
            return -1;

        if ((start = calloc(n + 1, sizeof(char))) == NULL)
            return -1;

        if (wcstombs(start, pwcs, n) == (size_t)-1)
            return -1;

        /* Finds entry associated with START in CSV map. */
        if ((word_entry = find(csv, start)) == NULL)
            return -1;

        /* If it's value is NULL, START wasn't in CSV. */
        if (*value(word_entry) == NULL)
            expect("starting word not found in CSV");
    } else {
        char *punctuation[3] = {"?", "!", "."};
        short n = 3;

        for (i = 0; i < 3; i++) {
            int index = rand() % n;

            /* Find the ENTRY associated with the PUNCTUATION at INDEX.  */
            if ((word_entry = find(csv, punctuation[index])) == NULL)
                return -1;

            /* If the ENTRY has no value, it means it wasn't in the CSV. */
            if (*value(word_entry) == NULL)
                /* Remove PUNCTUATION by swapping with the last valid one. */
                punctuation[index] = punctuation[--n];
            else
                /* Starting PUNCTUATION found. */
                break;
        }

        if (n == 0)
            expect("no punctuation found in CSV");
    }

    for (i = 0; i < n; i++) {
        float rng = (float)rand() / (float)RAND_MAX, total = 0;
        struct iter_t *next_entries_it;
        struct entry_t *entry, *next_entry;

        /* Iterator over the pairs (NEXT_WORD, FREQUENCY). */
        if ((next_entries_it = entries(((struct row_t *)*value(word_entry))->words)) == NULL)
            return -1;

        while ((entry = next(next_entries_it))) {
            float frequency = *(float *)value(entry);
            /* ENTRY will be NULL if the last ENTRY doesn't match. */
            next_entry = entry;
            if ((total + frequency) > rng)
                break;
            total += frequency;
        }

        word = key(next_entry);
        if ((word_entry = find(csv, word)) == NULL)
            return -1;

        if (!strcmp(word, "!") || !strcmp(word, "?") || !strcmp(word, ".")) {
            if (collect(collection, word) == -1)
                return -1;
            capitalize = true;
        } else {
            /* First word doesn't have a space before. */
            if (i != 0 && collect(collection, " ") == -1)
                return -1;

            if (capitalize) {
                wchar_t initial;
                char *buffer = calloc(MB_CUR_MAX + 1, sizeof(char));
                int bytes, len;

                /* Read the UNICODE value of the first letter and how many bytes it uses
                 * in UTF-8. */
                if ((bytes = mbtowc(&initial, word, 10)) == -1)
                    return -1;

                /* Reset internal state. */
                wctomb(NULL, 0);

                /* Map it to char* again as %ls isn't guaranteed to be supported in ANSI
                 * C. */
                if ((len = wctomb(buffer, towupper(initial))) == -1)
                    return -1;
                buffer[len] = 0;

                /* Print the capitalized INITIAL and the rest of the WORD */
                if (collect(collection, buffer) == -1)
                    return -1;

                /* If WORD doesn't have only the initial, collect the rest! */
                if (*(word + bytes))
                    if (collect(collection, word + bytes) == -1)
                        return -1;

                capitalize = false;
            } else {
                if (collect(collection, word) == -1)
                    return -1;
            }
        }
    }

    return 0;
}
