#include "vector.h"
#include <stdlib.h>

Vector *vec() {
  Vector *vector = calloc(1, sizeof(Vector));
  vector->data = calloc(0, sizeof(void *));
  vector->len = 0;
  return vector;
}

void *get(Vector *vector, size_t index) { return vector->data[index]; }

void push(Vector *vector, void *item) {
  vector->data = realloc(vector->data, ++vector->len * sizeof(void *));
  vector->data[vector->len - 1] = item;
}

// Vector *vec() {
//   Vector *vector = calloc(1, sizeof(Vector));
//   vector->len = 0;
//   vector->size = 1;
//   vector->data = calloc(vector->size, sizeof(void *));
//   return vector;
// }
//
// void *get(Vector *vector, size_t index) { return vector->data[index]; }
//
// void push(Vector *vector, void *item) {
//   if (++vector->len >= vector->size) {
//     vector->size = vector->size * 2;
//     vector->data = realloc(vector->data, vector->size * sizeof(void *));
//   }
//   vector->data[vector->len - 1] = item;
// }
