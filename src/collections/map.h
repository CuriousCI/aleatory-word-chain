#ifndef MAP_H_
#define MAP_H_

#include <stddef.h>

/* Allocate MAP (Red Black Tree). Returns NULL on ERROR. */
struct map_t *map();

/* Find the ENTRY associated with KEY. Create an ENTRY associated to KEY with value NULL if none is found. Returns NULL on ERROR. */
struct entry_t *find(struct map_t *map, char *key);

/* Iterator over MAP's entries. Returns NULL on ERROR. */
struct iter_t *entries(const struct map_t *map);

/* KEY of an ENTRY. */
char *key(struct entry_t *entry);

/* POINTER to VALUE of an ENTRY. */
void **value(struct entry_t *entry);

#endif /* map.h */
