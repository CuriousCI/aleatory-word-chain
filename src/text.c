#include "text.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "map.h"
#include "vec.h"

int lines(FILE *stream, int (*collect)(void *, char *), void *collection) {
    char *lineptr = NULL;
    size_t n = 0;

    /* getline == -1 indicates EOF or ERROR */
    while ((getline(&lineptr, &n, stream)) != -1) {
        if (collect(collection, lineptr) == -1)
            return -1;

        lineptr = NULL;
    }

    if (ferror(stream))
        return -1;

    return 0;
}

/* Lists the frequencies of a WORD's NEXT words. */
static struct counter_t {
    struct map_t *words;
} counter_t;

int insert(struct map_t *csv, char *(*consume)(void *), void *collection) {
    char *line;

    while ((line = consume(collection))) {
        struct entry_t *word_entry;
        struct counter_t *word_counter;
        char *word, *next, *freq;

        if ((word = strtok(line, ",\n")) == NULL)
            return -1;

        if ((word_entry = entry(csv, word)) == NULL)
            return -1;

        if ((word_counter = alloc(word_entry, sizeof(counter_t))) == NULL)
            return -1;

        if (word_counter->words == NULL)
            if ((word_counter->words = map()) == NULL)
                return -1;

        while ((next = strtok(NULL, ",\n")) && (freq = strtok(NULL, ",\n")))
            *(float *)alloc(
                entry(word_counter->words, next),
                sizeof(float)
            ) = atof(freq);
    }

    return 0;
}

/* TODO: casual start! */
/* TODO: not found! */
void text(struct map_t *csv, FILE *stream, size_t n, char *start) {
    struct entry_t *e;
    size_t i;
    bool capitalize = false;

    srand(time(NULL));

    if (start != NULL) {
        e = entry(csv, start);
    } else {
        if ((e = entry(csv, "?")) == NULL) {
            if ((e = entry(csv, "!")) == NULL) {
                if ((e = entry(csv, ".")) == NULL) {
                    return;
                }
            }
        }

        capitalize = true;
    }

    fprintf(stream, "%s", key(e));
    fflush(stream);

    for (i = 0; i < n; i++) {
        float rng = (float)rand() / (float)RAND_MAX, total = 0;
        struct iter_t *it = entries(((struct counter_t *)value(e))->words);
        struct entry_t *q, *qa;

        while ((q = next(it))) {
            qa = q;

            if (total > rng - 10e-6)
                break;

            total += *(float *)value(q);
        }

        char *k = key(qa);
        e = entry(csv, k);

        if (!strcmp(k, "!") || !strcmp(k, "?") || !strcmp(k, ".")) {
            capitalize = true;
            fprintf(stream, "%s", key(qa));
        } else {
            fprintf(stream, " ");
            if (capitalize) {
                /* TODO: capitalize */
                fprintf(stream, "%c%s", toupper(k[0]), k + 1);
                capitalize = false;
            } else {
                fprintf(stream, "%s", k);
            }
        }
        fflush(stream);
    }

    fflush(stream);
}
