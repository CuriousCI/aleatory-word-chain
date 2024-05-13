#ifndef DECODER_H_
#define DECODER_H_

#include "rbtree.h"

/* */
RBTree *parse_csv(wchar_t *str);

/* */
wchar_t *generate(RBTree *csv, size_t words);

/* */
wchar_t *generate_from(RBTree *csv, size_t words, wchar_t *word);

#endif // !DECODER_H_
