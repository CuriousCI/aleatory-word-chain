#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

typedef enum { Left, Right, None } Direction;

static Node *FAKE_LEAF = &(Node){NULL, NULL, Black, NULL, NULL, NULL};

RBTree *tree() {
  RBTree *tree = calloc(1, sizeof(RBTree));

  tree->root = FAKE_LEAF;
  tree->nodes = vec();

  return tree;
}

static void rotate_sx(Node *node) {
  Node *parent = node->parent, *y = node->dx;
  Node *alpha = node->sx, *beta = y->sx, *gamma = node->dx;

  if (parent != NULL) {
    if (parent->sx == node)
      parent->sx = y;
    else
      parent->dx = y;
  }
  y->parent = parent;

  y->sx = node;
  node->parent = y;
  node->sx = alpha;
  node->dx = beta;
  alpha->parent = node;
  beta->parent = node;
}

static void rotate_dx(Node *node) {
  Node *parent = node->parent, *y = node->sx;
  Node *alpha = y->sx, *beta = y->dx, *gamma = node->dx;

  if (parent != NULL) {
    if (parent->sx == node)
      parent->sx = y;
    else
      parent->dx = y;
  }
  y->parent = parent;

  y->sx = alpha;
  y->dx = node;
  alpha->parent = y;
  node->parent = y;
  node->sx = beta;
  node->sx->parent = y;
}

static void balance(Node *node) {
  if (node->parent == NULL) {
    node->color = Black;
    return;
  }

  if (node->parent->parent == NULL || node->parent->color == Black)
    return;

  Node *grandpa = node->parent->parent;
  Node *father = node->parent;
  Node *uncle = grandpa->sx == father ? grandpa->dx : grandpa->sx;

  if (uncle->color == Red) {
    grandpa->color = Red;
    father->color = Black;
    uncle->color = Black;

    balance(grandpa);
  } else if (father->dx == node) {
    rotate_sx(father);
    balance(father);
  } else {
    grandpa->color = Red;
    father->color = Black;

    if (grandpa->sx == father)
      rotate_dx(grandpa);
    else if (grandpa->dx == father) {
      rotate_sx(grandpa);
      balance(node);
    }
  }
}

static Node *find_insert(RBTree *tree, Node *node, Node *parent, wchar_t *key,
                         Direction direction) {
  if (node == FAKE_LEAF) {
    node = calloc(1, sizeof(Node));

    *node = (Node){key, NULL, Red, FAKE_LEAF, FAKE_LEAF, parent};
    push(tree->nodes, node);

    if (parent == NULL)
      tree->root = node;
    else if (direction == Left)
      parent->sx = node;
    else
      parent->dx = node;

    balance(node);
    return node;
  }

  int cmp = wcscmp(key, node->key);

  if (cmp == 0)
    return node;

  if (cmp < 0)
    return find_insert(tree, node->sx, node, key, Left);

  return find_insert(tree, node->dx, node, key, Right);
}

Node *node(RBTree *tree, wchar_t *key) {
  Node *node = find_insert(tree, tree->root, NULL, key, None);

  if (tree->root->parent != NULL) {
    tree->root = tree->root->parent;
  }

  return node;
}

void *value(RBTree *tree, wchar_t *key, size_t size) {
  Node *n = node(tree, key);

  if (n->value == NULL)
    n->value = calloc(1, size);

  return n->value;
}

void visit_node(Node *node) {
  if (node == FAKE_LEAF)
    return;

  if (node->color == Red)
    fprintf(stderr, "\x1b[1;31m %ls - %p\n", node->key, node->value);
  else
    fprintf(stderr, "\x1b[1;35m %ls - %p\n", node->key, node->value);

  visit_node(node->sx);
  visit_node(node->dx);
}

// fprintf(stderr, "\x1b[1;30m\x1b[1;47m %ls - %p\n", node->key, node->value);

void visit(RBTree *tree) { visit_node(tree->root); }
