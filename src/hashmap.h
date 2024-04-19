#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "vector.h"
#include <stddef.h>

/* */
typedef struct Entry {
  void *key;
  void *value;
} Entry;

/* */
typedef struct HashMap {
  Vector **buckets;
  size_t size;
} HashMap;

HashMap *hash_map(size_t size);

// TODO: -1 if error
// TODO: pass constructor + arguments as needed

// void insert(HashMap *d, void *item, size_t (*hash)(const void *),
//             void *(*alloc)(const void *));

// TODO: return -1 if error
// TODO: actual dictionary
// TODO: key to insert
// TODO: hashing function for the key
// TODO: compare function for the key
// TODO: size to allocate if item doesn't exists
/* */
Vector *at(HashMap *map, size_t index);

/*  */
void insert(HashMap *map, void *key, void *value, size_t (*hash)(const void *),
            int (*cmp)(const void *, const void *));
// TODO: dealloc

// TODO: -1 if not exists, or better NULL as we are treating pointers, or
// better: NULL -> not found, -1 error
// TODO: actual dictionary
// TODO: key to insert
// TODO: hashing function for the key
// TODO: compare function for the key
/* */
void *find(HashMap *map, void *key, size_t (*hash)(const void *),
           int (*cmp)(const void *, const void *));

/* */
size_t wcshash(const wchar_t *string);

#endif
