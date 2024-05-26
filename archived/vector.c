// #include "vec.h"
// #include <stdlib.h>
//
// static struct vec_t {
//   void **data;
//   size_t len;
// } vec_t;
//
// struct vec_t *vec() {
//   struct vec_t *vec = calloc(1, sizeof(struct vec_t));
//   *vec = (struct vec_t){calloc(1, sizeof(void *)), 1};
//
//   return vec;
// }
//
// void push(struct vec_t *vec, void *item) {
//   vec->data = realloc(vec->data, ++vec->len * sizeof(void *));
//   vec->data[vec->len - 1] = item;
// }
//
// static struct iter_t {
//   void **data;
//   size_t len, index;
// } iter_t;
//
// struct iter_t *iter(struct vec_t *vec) {
//   struct iter_t *iter = calloc(1, sizeof(iter_t));
//   *iter = (struct iter_t){vec->data, vec->len, 0};
//   return iter;
// }
//
// void *next(struct iter_t *iter) {
//   if (iter->index == iter->len)
//     return NULL;
//
//   return iter->data[iter->index++];
// }

// ---------------------------------------------------

// Vector *vec() {
//   Vector *vector = calloc(1, sizeof(Vector));
//
//   vector->len = 0;
//   vector->size = 8;
//   vector->data = calloc(vector->size, sizeof(void *));
//
//   return vector;
// }
//
// // TODO: error handling
// void *get(Vector *vector, size_t index) { return vector->data[index]; }
//
// void push(Vector *vector, void *item) {
//   if (++vector->len >= vector->size) {
//     vector->size = vector->size * 2;
//     vector->data = realloc(vector->data, vector->size * sizeof(void *));
//   }
//   vector->data[vector->len - 1] = item;
// }

// Vector *vec() {
//   Vector *vector = calloc(1, sizeof(Vector));
//   vector->len = 0;
//   vector->data = calloc(vector->len, sizeof(void *));
//   return vector;
// }
//
// void *get(Vector *vector, size_t index) { return vector->data[index]; }
//
// void push(Vector *vector, void *item) {
//   vector->data = realloc(vector->data, ++vector->len * sizeof(void *));
//   vector->data[vector->len - 1] = item;
// }

// vector->size++;
