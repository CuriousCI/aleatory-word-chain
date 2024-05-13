#ifndef ENCODER_H_
#define ENCODER_H_

#include "rbtree.h"

/* */
Vector *parse(wchar_t *str);

// /* */
// void parse(wchar_t *str, wchar_t *(*retrieve), void (*send)(wchar_t *));

/* */
RBTree *count(Vector *words);

/* */
void save(RBTree *counter);

/* */
// void save(RBTree *counter, FILE *file);

#endif // !ENCODER_H_
