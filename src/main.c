#include <stdio.h>
#include <stdlib.h>

// -p parallel

char *read_utf8(FILE *file) {
  //   0x80; // ASCII
  //   0xC0; // 1 bit
  //   0xE0; // 2 bit
  //   0xF0; // 3 bit
  //   0xF8; // 4 bit

  char *string = malloc(1 * sizeof(char));
  string[0] = 0;
  return string;
  // add a '.' before the string

  //   First code point 	Last code point 	Byte 1 	Byte 2 	Byte 3
  //   Byte 4
  // U+0000 	U+007F 	0xxxxxxx
  // U+0080 	U+07FF 	110xxxxx 	10xxxxxx
  // U+0800 	U+FFFF 	1110xxxx 	10xxxxxx 	10xxxxxx
  // U+010000 	[b]U+10FFFF 	11110xxx 	10xxxxxx 	10xxxxxx
  // 10xxxxxx
}

void to_lowercase(char *string) {
  while (string) {
    // if (*string )
    // to lowercase

    string++;
  }
}

// split into words!, ignore punctuation
char **split(char *string) {
  char **t = malloc(sizeof(char *) * 1);
  // max 30 chars per word

  return t;
}

// codepoint struct?

// key:
//  - left
//  - right
// value:
//  - count
//
//  sentence ? . !
//  word ' '

int main() {
  char *string;
  scanf("%s", string);
  printf("%s", string);

  return 0;
}
