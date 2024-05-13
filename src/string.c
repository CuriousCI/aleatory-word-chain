#include "string.h"
#include <stdlib.h>
#include <wchar.h>

String *string() {
  String *string = malloc(sizeof(String));

  string->len = 1;
  string->str = calloc(string->len, sizeof(wchar_t));

  return string;
}

String *wctostr(wchar_t c) {
  String *string = malloc(sizeof(String));

  string->len = 2;
  string->str = calloc(string->len, sizeof(wchar_t));
  *(string->str) = c;

  return string;
}

void append(String *string, wchar_t c) {
  string->str = realloc(string->str, ++string->len * sizeof(wchar_t));
  string->str[string->len - 2] = c;
  string->str[string->len - 1] = 0;
}

// if (++string->len >= string->size) {
//   string->size++;
//   // string->size *= 2;
//   string->str = realloc(string->str, string->size * sizeof(wchar_t));
// }
// string->size = 2;

// string->str = realloc(string->str, ++string->len * sizeof(wchar_t));

// TODO: return -1 if error, or crash?
/* */
// String *str() {
//   String *string = malloc(sizeof(String));
//
//   string->len = 1;
//   // string->size = 1;
//   // string->str = calloc(string->size, sizeof(wchar_t));
//   string->str = calloc(string->len, sizeof(wchar_t));
//
//   return string;
// }
