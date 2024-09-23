#include "csv.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "../collections/vec.h"
#include "util.h"

/* Collects wchars into WORDS. Returns -1 on ERROR. */
static int parse(wchar_t wc, char *words[2]);

/* Adds NEXT to the counter of WORD. Returns -1 on ERROR. */
static int add(struct map_t *counters, char *word, char *next);

/* wc = NULL resets internal state of the parser.
 * Returns the length of the first word.
 *
 * It could return up to two words
 * - words[0] is set to a STR if WC ends a word, otherwise it's set to NULL
 * - words[1] is either "!", "?", "." or NULL
 * - Ex. "ciao!" - reading '!' generates two words: ["ciao", "!"]
 * - The second word, if set, has always length 1 ("?", "!", ".")
 *
 * The advantage of working one wide character at a time si having fine-grained control over the parsing.
 * It works with wide characters to be able to use 'iswalpha()' which is locale-dependent.
 * */
static int parse(wchar_t wc, char *words[2]) {
    static char *question_mark = "?", *exclamation_mark = "!", *period = ".", *word = NULL;
    static size_t n = 0;
    char *buffer = malloc((MB_CUR_MAX + 1) * sizeof(char));
    bool is_printable = false;

    /* Reset 'parse' internal state. */
    if (!wc)
        word = NULL;

    if (wc == L'?' || wc == L'!' || wc == L'.') {
        words[0] = word;
        words[1] = wc == L'?' ? question_mark : (wc == L'!' ? exclamation_mark : period);

        word = NULL;
        return 0;
    }

    /* No punctuation. */
    words[1] = NULL;

    /* WC is a valid UNICODE character or digit in set locale. */
    if ((is_printable = iswalpha(wc) || iswdigit(wc) || wc == L'\'')) {
        /* Buffer to accomodate UTF-8 encoding of WC's UNICODE value . */
        int lengthening;

        wc = towlower(wc);

        /* Convert WC UNICODE codepoint to UTF-8 encoded bytes. */
        if ((lengthening = wctomb(buffer, wc)) == -1)
            expect("unable to encode UNICODE character to UTF-8");
        buffer[lengthening] = 0;

        if (word == NULL) {
            /* Alloc a new STR. */
            if ((word = malloc((lengthening + 1) * sizeof(char))) == NULL)
                return -1;

            /* Append WC encoded to UTF-8 to STR. */
            strcpy(word, buffer);
            n = lengthening;
        } else {
            /* Realloc STR to accomodate WC encoded to UTF-8. */
            if ((word = realloc(word, (n + lengthening + 1) * sizeof(char))) == NULL)
                return -1;

            /* Append WC encoded to UTF-8 to STR. */
            strcpy(word + n, buffer);
            n += lengthening;
        }

        /* Terminate STR with NULL. */
        word[n] = 0;
    }

    /* A WORD terminates with a '\'' or a non-printable character in set locale. */
    if ((!is_printable && word != NULL) || wc == L'\'') {
        words[0] = word;
        word = NULL;
        return 0;
    }

    /* No word generated. */
    words[0] = NULL;
    return 0;
}

/* Counts the occurences of a WORD and its NEXT WORDS. */
static struct counter_t {
    size_t count;
    struct map_t *words;
} counter_t;

/* Adds NEXT to the counter of WORD. Returns -1 on ERROR. */
static int add(struct map_t *counters, char *word, char *next) {
    struct counter_t *counter;
    struct entry_t *word_entry;
    void **val;

    if ((word_entry = find(counters, word)) == NULL)
        return -1;

    if (*(val = value(word_entry)) == NULL)
        /* If the ENTRY has no value, allocate a COUNTER in it. */
        if ((*val = calloc(1, sizeof(counter_t))) == NULL)
            return -1;
    counter = *val;

    counter->count++;
    if (counter->words == NULL)
        if ((counter->words = map()) == NULL)
            return -1;

    /* Find next word */
    if ((word_entry = find(counter->words, next)) == NULL)
        return -1;

    /* Increase its counter. */
    (*(size_t *)value(word_entry) += 1);

    return 0;
}

int split_words(FILE *stream, int (*collect)(void *, char *), void *collection) {
    wchar_t wc;
    char *words[2], *first = NULL;

    /* Reads one WC at a time */
    while ((wc = fgetwc(stream)) != (wchar_t)WEOF) {
        size_t word;

        if (parse(wc, words) == -1)
            return -1;

        /* Collects generated WORDs, if present. */
        for (word = 0; word < 2; word++)
            if (words[word]) {
                /* Sets FIRST WORD. */
                if (!first)
                    first = words[word];

                if (collect(collection, words[word]) == -1)
                    return -1;
            }
    }

    /* Collects FIRST WORD again at the end. */
    if (collect(collection, first) == -1)
        return -1;

    if (ferror(stream))
        return -1;

    return 0;
}

int count_words(struct map_t *counters, char *(*consume)(void *), void *collection) {
    char *word = consume(collection), *next;

    /* Iterater each pair of subsequent WORDS. */
    while ((next = consume(collection))) {
        if (add(counters, word, next) == -1)
            return -1;

        word = next;
    }

    return 0;
}

int csv(struct map_t *counters, int (*collect)(void *, char *), void *collection) {
    struct iter_t *entries_it;
    struct entry_t *entry;

    entries_it = entries(counters);
    while ((entry = next(entries_it))) {
        struct iter_t *subsequents;
        struct entry_t *subsequent;
        struct counter_t *counter;

        collect(collection, key(entry));
        counter = *value(entry);

        if ((subsequents = entries(counter->words)) == NULL)
            return -1;

        while ((subsequent = next(subsequents))) {
            char *word, *line;
            float frequency;
            int n;

            word = key(subsequent);
            frequency = (float)*(size_t *)value(subsequent) / (float)counter->count;

            if ((n = snprintf(NULL, 0, ",%s,%g", word, frequency)) == -1)
                return -1;

            line = calloc(n + 1, sizeof(char));
            snprintf(line, n + 1, ",%s,%g", word, frequency);

            if (collect(collection, line) == -1)
                return -1;
        }

        if (collect(collection, "\n") == -1)
            return -1;
    }

    return 0;
}
