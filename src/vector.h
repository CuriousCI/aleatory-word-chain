#ifndef VECTOR_H_
#define VECTOR_H_

// TODO: sistema questa struttura
// TODO: generazione automatica documentazione dal codice?

#include <stddef.h>

// TODO: typedef void *Generic; ??
// TODO: docstrings and attributes and examples?
// TODO: encapsulate data

/* A generic collection */
typedef struct Vector {
  void **data;
  size_t len;
} Vector;

/* Allocate an empty Vector on the heap. */
Vector *vec();

// TODO: returns -1 if outside of array
/* Returns the i-th element of the vector. */
void *get(Vector *vector, size_t index);

/* Pushes an item at the end of the Vector. */
void push(Vector *vector, void *item);

/* Searches if items exists in vector. If it does, it returns the pointer to the
 * existing item, otherwise it pushes the new item at the end of the Vector.  */
void *vec_insert(Vector *vector, void *item, int (*compare)(void *, void *));

#endif // !VECTOR_H_
