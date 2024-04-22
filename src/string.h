#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

/* */
typedef struct String {
  wchar_t *slice;
  size_t len;
} String;

/* Allocate an empty String. */
String *str();

/* Allocate a slice containing a single wchar_t. */
wchar_t *slice_from_wchar_t(wchar_t c);

/* Allocate a String if size wcslen(slice) containing a slice in O(n). */
// String *string_from_slice(wchar_t *slice);

/* Allocate a String containing a single wchar_t. */
String *string_from_wchar_t(wchar_t c);

// TODO: returns -1 if ....
/* Appends a wchar_t at the end of the string. */
void append(String *string, wchar_t c);

#endif // !STRING_H_
