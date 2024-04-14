#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

/* */
typedef struct String {
  char *slice;
  size_t len;
} String;

/* */
String *str();

/* */
String *string_from_slice(char *slice);

/* */
void append(String *string, char character);

#endif // !STRING_H_
