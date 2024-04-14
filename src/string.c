#include "string.h"
#include <stdlib.h>
#include <string.h>

String *str() {
  String *string = malloc(sizeof(String));
  string->slice = calloc(1, sizeof(char));
  string->len = 1;

  return string;
}

String *string_from_slice(char *slice) {
  String *string = malloc(sizeof(String));

  size_t len = strlen(slice);
  string->slice = calloc(len, sizeof(char));
  string->len = len;

  return string;
}

void append(String *string, char character) {
  string->slice = realloc(string->slice, ++string->len * sizeof(char));
  string->slice[string->len - 2] = character;
  string->slice[string->len - 1] = 0;
}
