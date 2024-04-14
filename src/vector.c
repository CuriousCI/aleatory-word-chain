#include "vector.h"
#include <stdlib.h>

Vector *vec() {
  Vector *vector = malloc(sizeof(Vector));
  vector->data = malloc(0);
  vector->len = 0;
  return vector;
}

void *get(Vector *vector, size_t index) { return vector->data[index]; }

void push(Vector *vector, void *item) {
  vector->data = realloc(vector->data, ++vector->len * sizeof(void *));
  vector->data[vector->len - 1] = item;
}

void *insert(Vector *vector, void *item, int (*compare)(void *, void *)) {
  for (int i = 0; i < vector->len; i++)
    if (compare(vector->data[i], item) == 0) {
      free(item);
      return vector->data[i];
    }

  push(vector, item);
  return item;
}
