#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

/* Allocate an empty STRING. */
struct string_t *string();

/* Append C at the end of STRING. */
void append(struct string_t *string, wchar_t c);

/* Return internal STR rappresentaiton of STRING. */
wchar_t *str(struct string_t *string);

#endif /* string.h */
