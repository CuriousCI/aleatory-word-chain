#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "vector.h"
#include <stddef.h>
#include <wchar.h>

/* */
typedef struct Entry {
  wchar_t *key;
  void *value;
} Entry;

/* */
typedef struct HashMap {
  Vector **buckets;
  size_t size, items;
} HashMap;

HashMap *hash_map(size_t size);

// TODO: return -1 if error
// TODO: actual dictionary
// TODO: key to insert
// TODO: hashing function for the key
// TODO: compare function for the key
// TODO: size to allocate if item doesn't exists
// TODO: dealloc
// TODO: -1 if not exists, or better NULL as we are treating pointers, or
// better: NULL -> not found, -1 error
// TODO: actual dictionary
// TODO: key to insert
// TODO: hashing function for the key
// TODO: compare function for the key

/* */
Vector *at(HashMap *map, size_t index);

/*  */
void *value_o(HashMap *map, wchar_t *key, size_t size);

/* */
void *entry(HashMap *map, wchar_t *key);

/* */
size_t wcshash(const wchar_t *string);

#endif
