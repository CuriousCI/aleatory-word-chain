#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// TODO: typedef for cmp and hash functions

HashMap *hash_map(size_t size) {
  HashMap *map = calloc(1, sizeof(HashMap));

  map->size = 100;
  map->items = 0;
  map->buckets = calloc(map->size, sizeof(Vector *));

  return map;
}

Vector *at(HashMap *map, size_t index) { return map->buckets[index]; }

void *value_o(HashMap *map, wchar_t *key, size_t size) {
  Entry *e = entry(map, key);
  if (e->value == NULL)
    e->value = calloc(1, size);

  return e->value;
}

void *entry(HashMap *map, wchar_t *key) {
  size_t hash = wcshash(key);
  Vector *bucket = map->buckets[hash % map->size];

  if (bucket == NULL) {
    map->buckets[hash % map->size] = vec();
    bucket = map->buckets[hash % map->size];
  }

  for (size_t item = 0; item < bucket->len; item++) {
    Entry *entry = get(bucket, item);
    if (wcscmp(key, entry->key) == 0)
      return entry;
  }

  Entry *entry = calloc(1, sizeof(Entry));
  entry->key = key;
  push(bucket, entry);

  if (++map->items >= map->size * 3) {
    map->buckets = realloc(map->buckets, map->size * 2 * sizeof(Vector *));
    for (size_t bucket = map->size; bucket < map->size * 2; bucket++)
      map->buckets[bucket] = NULL;
    map->size <<= 1;
  }

  return entry;
}

// bucket = vec();
// bucket = map->buckets[wcshash(key) % map->size];
// bucket = vec();
// size_t old = map->size;
// map->size *= 2;
// memset(map->buckets + old, 0, old);
// memset(map->buckets, 0, map->size);
// map->buckets = realloc(map->buckets, map->size * 2 * sizeof(Vector *));
// for (size_t bucket = map->size; bucket < map->size * 2; bucket++)
//   map->buckets[bucket] = vec();

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

/* https://cp-algorithms.com/string/string-hashing.html */
size_t wcshash(const wchar_t *string) {
  const size_t modulo = 1e9 + 9;
  size_t hash = 0, power = 0;

  while (*string) {
    hash = (hash + (*string - 'a' + 1) * POWERS[power % 30]) % modulo;
    power++;
    string++;
  }

  return hash;
}

// hash =
//     (hash + (*string - 'a' + 1) * POWERS[power < 30 ? power : 29]) %
//     modulo;
