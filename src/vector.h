#ifndef VECTOR_H_
#define VECTOR_H_

// TODO: sistema questa struttura
// TODO: generazione automatica documentazione dal codice?

#include <stddef.h>

// TODO: typedef void *Generic; ??
// TODO: docstrings and attributes and examples?

/* */
typedef struct Vector {
  void **data;
  size_t len; // len of vector
} Vector;

/* */
Vector *vec();

/* */
void *get(Vector *vector, size_t index);

/* */
void push(Vector *vector, void *item);

/* */
void *insert(Vector *vector, void *item, int (*compare)(void *, void *));

#endif // !VECTOR_H_
