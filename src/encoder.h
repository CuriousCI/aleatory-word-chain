#ifndef ENCODER_H_
#define ENCODER_H_

#include "rbtree.h"

struct parser;

/* */
Vector *parse(wchar_t *str);

void generic_parse(wchar_t *str, void *data, void (*save)(void *, wchar_t *));

void generic_count(wchar_t *current, wchar_t *next, RBTree *counter);

// struct parser *new_parser();

wchar_t *generic_parse_2(wchar_t *str, struct parser *p);

// /* */
// void parse(wchar_t *str, wchar_t *(*retrieve), void (*send)(wchar_t *));

/* */
RBTree *count(Vector *words);

/* */
void save(RBTree *counter);

/* */
// void save(RBTree *counter, FILE *file);

#endif // !ENCODER_H_
