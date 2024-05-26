#include "vec.h"

#include <stdlib.h>

#define GOLDEN_RATIO 1.618034

/* I chose to allocate items contigously instead of using a Linked List. */
/* Rust does it with Vec<T> and C++ with std::vector, it shouldn't give many problems. */
static struct vec_t {
    void **items;
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
        /* The CAPACITY amortizes the numbers of calls to REALLOC. */
        vec->capacity *= GOLDEN_RATIO;
        vec->items = realloc(vec->items, vec->capacity * sizeof(void *));

        if (vec->items == NULL)
            return -1;
    }

    vec->items[vec->len - 1] = item;

    return 0;
}

static struct iter_t {
    void **item, **last;
} iter_t;

struct iter_t *iter(const struct vec_t *vec) {
    struct iter_t *iter;

    if ((iter = malloc(sizeof(iter_t))) == NULL)
        return NULL;

    iter->item = vec->items;
    iter->last = vec->items + vec->len;

    return iter;
}

void *next(struct iter_t *iter) {
    return iter->item >= iter->last ? NULL : *iter->item++;
}

/* void **items;
size_t len, index; */
/* TODO: set(ERRNO) to bad address */
/* if (vec == NULL)
    return NULL; */
/* Initialize ITER with the first item of the VEC. */
/* iter->items = vec->items;
iter->len = vec->len;
iter->index = 0; */

/* if (iter->index >= iter->len || iter == NULL)
    return NULL;

return iter->items[iter->index++]; */
/* if ()
    return NULL;*/
/* return *iter->item++;*/
