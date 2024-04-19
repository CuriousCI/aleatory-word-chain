#ifndef BTREE_H_
#define BTREE_H_

/* */
typedef struct Node {
  void *values;
  void **children;
} Node;

/* */
Node *search(void *value, int (*cmp)(void *, void *));

/* */
Node *insert(void *value, int (*cmp)(void *, void *));

// I don't even need delete baby!

#endif
