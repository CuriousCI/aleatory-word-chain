#include "string.h"
#include <stdlib.h>
// #include <string.h>
#include <wchar.h>

// TODO: return -1 if error, or crash?
/* */
String *str() {
  String *string = malloc(sizeof(String));
  string->slice = calloc(1, sizeof(wchar_t));
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

  string->slice = calloc(2, sizeof(wchar_t));
  *(string->slice) = c;
  string->len = 2;

  return string;
}

String *string_from_slice(wchar_t *slice) {
  String *string = malloc(sizeof(String));

  size_t len = wcslen(slice);
  string->slice = calloc(len, sizeof(wchar_t));
  string->len = len;
  wcscpy(string->slice, slice);

  return string;
}

void append(String *string, wchar_t c) {
  string->slice = realloc(string->slice, ++string->len * sizeof(wchar_t));
  string->slice[string->len - 2] = c;
  string->slice[string->len - 1] = 0;
}
