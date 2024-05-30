#include "vec.h"

#include <stdlib.h>

/* A resizing factor of 2 is too big. */
#define GOLDEN_RATIO 1.618034

/* Rust does it with Vec<T> and C++ with std::vector, it shouldn't give many problems. */
static struct vec_t {
    void **items;
    /* LEN is the actual number of items. */
    /* CAPACITY is the allocated size. */
    size_t len, capacity;
} vec_t;

struct vec_t *vec() {
    struct vec_t *vec;

    if ((vec = malloc(sizeof(vec_t))) == NULL)
        return NULL;

    /* Initialize VEC with 0 ITEMS and 2 CAPACITY. */
    if ((vec->items = malloc((vec->capacity = 2) * sizeof(void *))) == NULL) {
        free(vec);
        return NULL;
    }

    vec->len = 0;

    return vec;
}

int push(struct vec_t *vec, void *item) {
    if (++vec->len >= vec->capacity) {
        /* CAPACITY amortizes the numbers of calls to REALLOC. */
        vec->capacity *= GOLDEN_RATIO;
        vec->items = realloc(vec->items, vec->capacity * sizeof(void *));

        if (vec->items == NULL)
            return -1;
    }

    /* PUSH to back. */
    vec->items[vec->len - 1] = item;

    return 0;
}

/* Useful to have a common interface to iterate collections. */
static struct iter_t {
    /* Current ITEM and pointer after last ITEM. */
    void **item, **end;
} iter_t;

struct iter_t *iter(const struct vec_t *vec) {
    struct iter_t *iter;

    if ((iter = malloc(sizeof(iter_t))) == NULL)
        return NULL;

    iter->item = vec->items;
    iter->end = vec->items + vec->len;

    return iter;
}

/* Check if ITER is empty. Return current ITEM and set ITEM to next.  */
void *next(struct iter_t *iter) {
    return iter->item >= iter->end ? NULL : *iter->item++;
}
