#ifndef VEC_H_
#define VEC_H_

#include <stddef.h>

/* Allocate a VEC. Returns NULL on ERROR. */
struct vec_t *vec();

/* Push ITEM at the end of VEC. Returns -1 on ERROR. */
int push(struct vec_t *vec, void *item);

/* Create an ITER over VEC's ITEMs. Returns NULL on ERROR. */
struct iter_t *iter(const struct vec_t *vec);

/* Returns the current ITEM and moves ITER to the NEXT ITEM. Returns NULL if ITER is EMPTY. */
void *next(struct iter_t *iter);

#endif /* vec.h */
