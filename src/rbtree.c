#include "rbtree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

Node *FAKE_LEAF = &(Node){NULL, NULL, Black, NULL, NULL, NULL};

RBTree *tree() {
  RBTree *tree = calloc(1, sizeof(RBTree));

  *tree = (RBTree){FAKE_LEAF, vec()};

  return tree;
}

typedef enum { Left, Right } Direction;

static void rotate_sx(RBTree *tree, Node *node) {
  Node *parent = node->parent;
  Node *right_child = node->dx;

  node->dx = right_child->sx;
  if (right_child->sx != NULL)
    right_child->sx->parent = node;

  right_child->sx = node;
  node->parent = right_child;

  if (parent == NULL)
    tree->root = right_child;
  else if (parent->sx == node)
    parent->sx = right_child;
  else if (parent->dx == node)
    parent->dx = right_child;

  if (right_child != NULL)
    right_child->parent = parent;
}

static void rotate_dx(RBTree *tree, Node *node) {
  Node *parent = node->parent;
  Node *left_child = node->sx;

  node->sx = left_child->dx;
  if (left_child->dx != NULL)
    left_child->dx->parent = node;

  left_child->dx = node;
  node->parent = left_child;

  if (parent == NULL)
    tree->root = left_child;
  else if (parent->sx == node)
    parent->sx = left_child;
  else if (parent->dx == node)
    parent->dx = left_child;

  if (left_child != NULL)
    left_child->parent = parent;
}

static void balance(RBTree *tree, Node *node) {
  Node *parent = node->parent;

  if (parent == NULL)
    return;

  if (parent->color == Black)
    return;

  Node *grandpa = parent->parent;

  if (grandpa == NULL) {
    parent->color = Black;
    return;
  }

  Node *uncle = grandpa->sx == parent ? grandpa->dx : grandpa->sx;

  if (uncle != NULL && uncle->color == Red) {
    parent->color = Black;
    grandpa->color = Red;
    uncle->color = Black;

    balance(tree, grandpa);
  } else if (parent == grandpa->sx) {
    if (node == parent->dx) {
      rotate_sx(tree, parent);
      parent = node;
    }
    rotate_dx(tree, grandpa);
    parent->color = Black;
    grandpa->color = Red;
  } else {
    if (node == parent->sx) {
      rotate_dx(tree, parent);
      parent = node;
    }
    rotate_sx(tree, grandpa);
    parent->color = Black;
    grandpa->color = Red;
  }
}

Node *node(RBTree *tree, wchar_t *key) {
  Node *node = tree->root;
  Node *parent = NULL;
  int cmp;

  while (node != FAKE_LEAF) {
    parent = node;
    cmp = wcscmp(key, node->key);

    if (cmp == 0)
      return node;

    if (cmp < 0)
      node = node->sx;
    else
      node = node->dx;
  }

  node = calloc(1, sizeof(Node));
  *node = (Node){key, NULL, Red, FAKE_LEAF, FAKE_LEAF, parent};

  push(tree->nodes, node);

  if (parent == NULL) {
    tree->root = node;
    return node;
  } else if (cmp < 0)
    parent->sx = node;
  else
    parent->dx = node;

  balance(tree, node);
  return node;
}

void *value(RBTree *tree, wchar_t *key, size_t size) {
  Node *n = node(tree, key);

  if (n->value == NULL)
    n->value = calloc(1, size);

  return n->value;
}
