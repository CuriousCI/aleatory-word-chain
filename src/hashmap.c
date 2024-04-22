#include "hashmap.h"
#include <stdlib.h>
#include <wchar.h>

// TODO: typedef for cmp and hash functions

// typedef struct HashMap {
//   Vector **buckets;
//   size_t size;
// } HashMap;

HashMap *hash_map(size_t size) {
  HashMap *map = calloc(1, sizeof(HashMap));

  map->size = size;
  map->buckets = calloc(size, sizeof(Vector *));
  for (size_t bucket = 0; bucket < size; bucket++)
    map->buckets[bucket] = vec();

  return map;
}

Vector *at(HashMap *map, size_t index) { return map->buckets[index]; }

// void insert(HashMap *map, void *key, void *value, size_t (*hash)(const void
// *),
//             int (*cmp)(const void *, const void *)) {
//   Vector *bucket = map->buckets[hash(key) % map->size];
//
//   for (int i = 0; i < bucket->len; i++) {
//     Entry *entry = get(bucket, i);
//     if (cmp(key, entry->key) == 0) {
//       entry->value = value;
//       return;
//     }
//   }
//
//   Entry *entry = calloc(1, sizeof(Entry));
//   entry->key = key;
//   entry->value = value;
//   push(bucket, entry);
// }

void *value(const HashMap *map, wchar_t *key, size_t size) {
  Vector *bucket = map->buckets[wcshash(key) % map->size];

  for (int i = 0; i < bucket->len; i++) {
    Entry *entry = get(bucket, i);
    if (wcscmp(key, entry->key) == 0)
      return entry->value;
  }

  Entry *entry = calloc(1, sizeof(Entry));
  entry->key = key;
  entry->value = calloc(1, size);
  push(bucket, entry);

  return entry->value;
}

// void *initialize(const HashMap *map, wchar_t *key, void
// *(initializer)(size_t),
//                  size_t size) {
//   Vector *bucket = map->buckets[wcshash(key) % map->size];
//
//   for (int i = 0; i < bucket->len; i++) {
//     Entry *entry = get(bucket, i);
//     if (wcscmp(key, entry->key) == 0)
//       return entry->value;
//   }
//
//   Entry *entry = calloc(1, sizeof(Entry));
//   entry->key = key;
//   entry->value = initializer(size);
//   push(bucket, entry);
//
//   return entry->value;
// }

void *entry(const HashMap *map, wchar_t *key) {
  // void *entry(const HashMap *map, wchar_t *key, size_t size) {
  Vector *bucket = map->buckets[wcshash(key) % map->size];

  for (int i = 0; i < bucket->len; i++) {
    Entry *entry = get(bucket, i);
    if (wcscmp(key, entry->key) == 0)
      return entry;
  }

  Entry *entry = calloc(1, sizeof(Entry));
  entry->key = key;
  // entry->value = calloc(1, size);
  push(bucket, entry);

  return entry;
}

// Entry *entry = get(bucket, i);
// if (wcscmp(key, entry->key) == 0)
//   return &entry->value;

// void *find(HashMap *map, void *key, size_t (*hash)(const void *),
//            int (*cmp)(const void *, const void *)) {
//   Vector *bucket = map->buckets[hash(key) % map->size];
//
//   for (int i = 0; i < bucket->len; i++) {
//     Entry *entry = get(bucket, i);
//     if (cmp(key, entry->key) == 0)
//       return entry;
//   }
//
//   Entry *entry = calloc(1, sizeof(Entry));
//   entry->key = key;
//   push(bucket, entry);
//   return entry;
// }

// TODO: -1 if error
// TODO: pass constructor + arguments as needed
/*  */
static const size_t POWERS[] = {
    31,        961,       29791,     923521,    28629151,  887503681,
    512613868, 891029773, 621922720, 279604149, 667728547, 699584777,
    687127898, 300964649, 329904038, 227025088, 37777665,  171107606,
    304335741, 434407890, 466644473, 465978537, 445334521, 805370034,
    966470838, 960595717, 778466966, 132475730, 106747594, 309175387,
};

// https://cp-algorithms.com/string/string-hashing.html
size_t wcshash(const wchar_t *string) {
  const size_t modulo = 1e9 + 9;
  size_t hash = 0, power = 0;

  while (*string) {
    hash =
        (hash + (*string - 'a' + 1) * POWERS[power < 30 ? power : 29]) % modulo;
    power++;
    string++;
  }

  return hash;
}

// void dict_insert(HashMap *dictionary, void *item, size_t hash) {
//   Vector *bucket = dictionary->buckets[hash % dictionary->size];
//
//   int found = 0;
//
//   for (int i = 0; i < bucket->len; i++) {
//     Entry *e = get(bucket, i);
//     if (e->key == hash) {
//       e->value = item;
//       found = 1;
//       break;
//     }
//   }
//
//   if (!found) {
//     Entry *e = calloc(1, sizeof(Entry));
//     e->key = hash;
//     e->value = item;
//     push(bucket, e);
//   }
//   // insert(bucket, hash);
//   // return find(bucket, hash);
// }

/* */
// void *dict_get(HashMap *dictionary, size_t hash) {}

// long long compute_hash(string const& s) {
//     const int p = 31;
//     const int m = 1e9 + 9;
//     long long hash_value = 0;
//     long long p_pow = 1;
//     for (char c : s) {
//         hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
//         p_pow = (p_pow * p) % m;
//     }
//     return hash_value;
// }
