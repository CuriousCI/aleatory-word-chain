#ifndef MAP_H_
#define MAP_H_

#include <stddef.h>

/* Allocate an empty MAP (Implemented as a Red Black Tree). Returns NULL on ERROR. */
struct map_t *map();

/* Find the ENTRY associated with KEY. Create an ENTRY with KEY if NONE is found. Returns NULL on ERROR. */
struct entry_t *entry(struct map_t *map, char *key);

/* Iterator over MAP's entries. Returns NULL on ERROR. */
struct iter_t *entries(const struct map_t *map);

/* KEY of an entry. */
char *key(struct entry_t *entry);

/* VALUE of an entry. */
void *value(struct entry_t *entry);

/* VALUE of the entry. If VALUE is NULL allocate SIZE bytes. Returns NULL on ERROR. */
void *alloc(struct entry_t *entry, size_t size);

#endif /* map.h */
