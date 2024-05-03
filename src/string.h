#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

/* */
typedef struct String {
  wchar_t *str;
  size_t len;
  size_t size;
} String;

/* Allocate an empty String. */
String *str();

/* Allocate a slice containing a single wchar_t. */
wchar_t *wchar_t_to_str(wchar_t c);

/* Allocate a String containing a single wchar_t. */
String *wchar_t_to_string(wchar_t c);

// TODO: returns -1 if ....
/* Appends a wchar_t at the end of the string. */
void append(String *string, wchar_t c);

#endif // !STRING_H_
