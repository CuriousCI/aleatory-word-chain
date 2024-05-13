#ifndef RBTREE_H_
#define RBTREE_H_

#include "vector.h"
#include <stddef.h>
#include <wchar.h>

typedef enum { Red, Black } Color;

// char status = 0;
// 0000_0000 -> BLACK
// 0000_0001 -> RED, else BLACK

// typedef struct Node2 {
//   wchar_t *key;
//   void *value;
//   char color;
//   struct Node2 *sx, *dx, *parent;
// } Node2;

typedef struct Node {
  wchar_t *key;
  void *value;
  // Color color;
  char color;
  struct Node *sx, *dx;
  struct Node *parent;
} Node;

typedef struct RBTree {
  Node *root;
  Vector *nodes;
} RBTree;

RBTree *tree();

void *value(RBTree *tree, wchar_t *key, size_t size);

Node *node(RBTree *tree, wchar_t *key);

#endif
