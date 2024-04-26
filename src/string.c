#include "string.h"
#include <stdlib.h>
// #include <string.h>
#include <wchar.h>

// TODO: return -1 if error, or crash?
/* */
String *str() {
  String *string = malloc(sizeof(String));
  string->str = calloc(1, sizeof(wchar_t));
  string->len = 1;

  return string;
}

wchar_t *slice_from_wchar_t(wchar_t c) {
  wchar_t *slice = calloc(2, sizeof(wchar_t));
  *slice = c;

  return slice;
}

String *string_from_wchar_t(wchar_t c) {
  String *string = malloc(sizeof(String));

  string->str = calloc(2, sizeof(wchar_t));
  *(string->str) = c;
  string->len = 2;

  return string;
}

void append(String *string, wchar_t c) {
  string->str = realloc(string->str, ++string->len * sizeof(wchar_t));
  string->str[string->len - 2] = c;
  string->str[string->len - 1] = 0;
}
