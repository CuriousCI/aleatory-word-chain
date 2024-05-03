#include "string.h"
#include <stdlib.h>
// #include <string.h>
#include <wchar.h>

// TODO: return -1 if error, or crash?
/* */
String *str() {
  String *string = malloc(sizeof(String));

  string->len = 1;
  string->size = 1;
  string->str = calloc(string->size, sizeof(wchar_t));

  return string;
}

wchar_t *wchar_t_to_str(wchar_t c) {
  wchar_t *str = calloc(2, sizeof(wchar_t));
  *str = c;

  return str;
}

String *wchar_t_to_string(wchar_t c) {
  String *string = malloc(sizeof(String));

  string->str = calloc(2, sizeof(wchar_t));
  *(string->str) = c;

  string->len = 2;
  string->size = 2;

  return string;
}

void append(String *string, wchar_t c) {
  if (++string->len >= string->size) {
    string->size++;
    // string->size *= 2;
    string->str = realloc(string->str, string->size * sizeof(wchar_t));
  }

  string->str[string->len - 2] = c;
  string->str[string->len - 1] = 0;
}

// string->str = realloc(string->str, ++string->len * sizeof(wchar_t));
