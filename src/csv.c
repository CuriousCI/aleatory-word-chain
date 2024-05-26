#include "csv.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "vec.h"

/* Parses STREAM of wchars into TOKENS. Returns -1 on ERROR.
 *
 * wc = NULL resets internal state of the parser.
 * Returns the length of the first token.
 *
 * It could return up to two tokens
 * - Eg. "ciao!" - reading '!' generates two tokens: 'ciao' and '!'
 * - The second token has always length 1 ("?", "!", ".")
 *
 * The advantage of working one wide character at a time si having fine-grained control over the parsing.
 * It works with wide characters to be able to use 'iswalpha()' which is locale-dependent.
 * */

static int parse(wchar_t wc, char *tokens[2]) {
    static char *question_mark = "?", *exclamation_mark = "!", *period = ".", *str = NULL;
    static size_t len = 0;
    bool is_printable = false;

    /* Reset 'parse' internal state. */
    if (!wc)
        str = NULL;

    if (wc == L'?' || wc == L'!' || wc == L'.') {
        tokens[0] = str;
        tokens[1] = wc == L'?' ? question_mark : (wc == L'!' ? exclamation_mark : period);

        str = NULL;
        return len;
    }

    /* No punctuation. */
    tokens[1] = NULL;

    /* WC is a valid UNICODE character or digit in set locale. */
    if ((is_printable = iswalpha(wc) || iswdigit(wc) || wc == L'\'')) {
        /* Buffer to accomodate UTF-8 encoding of UNICODE value of WC. */
        char buffer[MB_CUR_MAX + 1];
        int lengthening;

        wc = towlower(wc);

        /* Convert WC UNICODE codepoint to UTF-8 encoded bytes. */
        if ((lengthening = wctomb(buffer, wc)) == -1)
            return -1;
        buffer[lengthening] = 0;

        if (str == NULL) {
            /* Alloc a new STR. */
            if ((str = malloc((lengthening + 1) * sizeof(char))) == NULL)
                return -1;

            /* Append WC encoded to UTF-8 to STR. */
            strcpy(str, buffer);
            len = lengthening;
        } else {
            /* Realloc STR to accomodate WC encoded to UTF-8. */
            if ((str = realloc(str, (len + lengthening + 1) * sizeof(char))) == NULL)
                return -1;

            /* Append WC encoded to UTF-8 to STR. */
            strcpy(str + len, buffer);
            len += lengthening;
        }

        /* Terminate STR with NULL. */
        str[len] = 0;
    }

    /* A token terminates with a \' or a non-printable character in set locale. */
    if ((!is_printable && str != NULL) || wc == L'\'') {
        tokens[0] = str;
        str = NULL;
        return len;
    }

    /* No token generated. */
    tokens[0] = NULL;
    return 0;
}

/* Counts the occurences of a WORD and its NEXT words. */
static struct counter_t {
    size_t count;
    struct map_t *words;
} counter_t;

/* Adds NEXT to the counter of WORD. Returns -1 on ERROR. */
static int add(struct map_t *counters, char *word, char *next) {
    struct counter_t *counter;
    struct entry_t *e;
    size_t *next_count;

    if ((e = entry(counters, word)) == NULL)
        return -1;

    if ((counter = alloc(e, sizeof(counter_t))) == NULL)
        return -1;

    counter->count++;
    if (counter->words == NULL)
        if ((counter->words = map()) == NULL)
            return -1;

    if ((e = entry(counter->words, next)) == NULL)
        return -1;

    if ((next_count = alloc(e, sizeof(size_t))) == NULL)
        return -1;

    (*next_count)++;

    return 0;
}

int split(FILE *stream, int (*collect)(void *, char *), void *collection) {
    wchar_t wc;
    char *tokens[2], *first = NULL;

    while ((wc = fgetwc(stream)) != WEOF) {
        size_t token;

        if (parse(wc, tokens) == -1)
            return -1;

        for (token = 0; token < 2; token++)
            if (tokens[token]) {
                if (!first)
                    first = tokens[token];

                if (collect(collection, tokens[token]) == -1)
                    return -1;
            }
    }

    if (collect(collection, first) == -1)
        return -1;

    if (ferror(stream))
        return -1;

    return 0;
}

int count(struct map_t *counters, char *(*consume)(void *), void *collection) {
    char *word = consume(collection), *next;

    while ((next = consume(collection))) {
        if (add(counters, word, next) == -1)
            return -1;

        word = next;
    }

    return 0;
}

void csv(struct map_t *counters, FILE *stream) {
    struct iter_t *it;
    struct entry_t *e;

    it = entries(counters);
    while ((e = next(it))) {
        struct iter_t *subsequents;
        struct entry_t *subsequent;
        struct counter_t *counter;

        fprintf(stream, "%s", key(e));

        counter = value(e);
        subsequents = entries(counter->words);
        while ((subsequent = next(subsequents)))
            fprintf(
                stream,
                ",%s,%g",
                key(subsequent),
                (float)*(size_t *)value(subsequent) / (float)counter->count
            );

        fprintf(stream, "\n");
    }

    fflush(stream);
}
