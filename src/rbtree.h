#ifndef RBTREE_H_
#define RBTREE_H_

#include "vector.h"
#include <stddef.h>
#include <wchar.h>

typedef enum { Red, Black } Color;

typedef struct Node {
  wchar_t *key;
  void *value;
  Color color;
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

void visit(RBTree *tree);

#endif

// Node **cache;
// size_t cache_size;
//
// tree->cache_size = 10;
// tree->cache = calloc(tree->cache_size, sizeof(Node *));
// size_t hash = wcshash(key);
// Node *c = tree->cache[hash % tree->cache_size];
// if (c != NULL && wcscmp(c->key, key) == 0)
//   return c;
// tree->cache[hash % tree->cache_size] = node;

// typedef struct {
// wchar_t *key;
// void *value;
// Color color;
// struct Node *sx, *dx;
// struct Node *parent;
// } Node;

// typedef struct Node {
// wchar_t *key;
// void *value;
// Color color;
// struct Node *sx, *dx;
// struct Node *parent;
// } Node;

// typedef struct RBTree {
// Node root;
// } RBTree;
