#include "string.h"

#include <stdlib.h>
#include <wchar.h>

static struct string_t {
    wchar_t *str;
    size_t len;
} string_t;

struct string_t *string() {
    struct string_t *string = malloc(sizeof(string_t));

    /* Initialize STRING with a NULL character. */
    string->str = calloc(1, sizeof(wchar_t));
    string->len = 1;

    return string;
}

void append(struct string_t *string, wchar_t c) {
    string->str = realloc(string->str, ++string->len * sizeof(wchar_t));

    /* Add C to STRING and set the NULL character. */
    string->str[string->len - 2] = c;
    string->str[string->len - 1] = 0;
}

wchar_t *str(struct string_t *string) {
    return string->str;
}

/* *string = (struct string_t){calloc(1, sizeof(wchar_t)), 1}; */
