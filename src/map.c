#include "map.h"

#include <stdlib.h>
#include <string.h>

#include "vec.h"

static void balance(struct map_t *map, struct entry_t *entry);
static void rotate_left(struct map_t *map, struct entry_t *entry);
static void rotate_right(struct map_t *map, struct entry_t *entry);

/* Property 1. https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Properties */
typedef enum color_t {
    RED,
    BLACK
} color_t;

static struct entry_t {
    char *key;
    void *value;
    /* Property 1. https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Properties */
    color_t color;
    struct entry_t *left, *right, *parent;
} entry_t;

char *key(struct entry_t *entry) {
    return entry->key;
}

void *value(struct entry_t *entry) {
    return entry->value;
}

void *alloc(struct entry_t *entry, size_t size) {
    if (entry->value == NULL)
        if ((entry->value = calloc(1, sizeof(size))) == NULL)
            return NULL;

    return entry->value;
}

static struct map_t {
    /* Red Black Tree's ROOT and NIL leaves. */
    struct entry_t *root, *NIL;
    /* Easier iteration of MAP's ENTRIES. */
    struct vec_t *entries;
} map_t;

struct map_t *map() {
    struct map_t *map;

    if ((map = malloc(sizeof(map_t))) == NULL)
        return NULL;

    if ((map->NIL = malloc(sizeof(entry_t))) == NULL) {
        free(map);
        return NULL;
    }

    map->NIL->key = NULL;
    map->NIL->value = NULL;
    /* Property 2. https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Properties */
    map->NIL->color = BLACK;
    map->NIL->left = NULL;
    map->NIL->right = NULL;
    map->NIL->parent = NULL;

    map->root = map->NIL;
    if ((map->entries = vec()) == NULL) {
        free(map->NIL);
        free(map);
        return NULL;
    }

    return map;
}

struct iter_t *entries(const struct map_t *map) { return iter(map->entries); }

struct entry_t *entry(struct map_t *map, char *key) {
    /* Start TREE visit from ROOT. */
    struct entry_t *entry = map->root, *parent = NULL;
    int cmp;

    while (entry != map->NIL) {
        /* An ENTRY for KEY already exists. */
        if (!(cmp = strcmp(key, entry->key)))
            return entry;

        /* Use the last visited ENTRY as parent for the new ENTRY. */
        parent = entry;

        /* Continue visit. */
        if (cmp < 0)
            entry = entry->left;
        else
            entry = entry->right;
    }

    if ((entry = malloc(sizeof(entry_t))) == NULL)
        return NULL;

    entry->key = key;
    entry->value = NULL;
    /* New NODEs are RED. */
    entry->color = RED;
    entry->left = map->NIL;
    entry->right = map->NIL;
    entry->parent = parent;

    if (push(map->entries, entry) == -1) {
        free(entry);
        return NULL;
    }

    if (parent == NULL)
        /* PARENT == NULL implies there is no ROOT. */
        return (map->root = entry);

    /* Insertion. */
    if (cmp < 0)
        parent->left = entry;
    else
        parent->right = entry;

    balance(map, entry);
    return entry;
}

/* Balance TREE after insertion. */
/* I'll refer to the number of BLACK nodes in any PATH from a node to its descendants as B-HEIGHT. */
static void balance(struct map_t *map, struct entry_t *entry) {
    struct entry_t *parent, *grandpa, *uncle;

    /* ENTRY is the ROOT, nothing to balance. */
    if ((parent = entry->parent) == NULL)
        return;

    /* PARENT is BLACK, ENTRY is RED, nothing to balance. */
    /* Property 3. states that only RED nodes can't have a RED child. */
    /* Property 4. adding a RED node maintains the B-HEIGHT. */
    if (parent->color == BLACK)
        return;

    /* Property 4. changing the color of the ROOT to BLACK maintains the B-HEIGHT. */
    if ((grandpa = parent->parent) == NULL) {
        parent->color = BLACK;
        return;
    }

    uncle = grandpa->left == parent ? grandpa->right : grandpa->left;

    if (uncle != NULL && uncle->color == RED) {
        parent->color = BLACK;
        grandpa->color = RED;
        uncle->color = BLACK;

        balance(map, grandpa);
        return;
    }

    if (parent == grandpa->left) {
        if (entry == parent->right) {
            rotate_left(map, parent);
            parent = entry;
        }
        rotate_right(map, grandpa);
    } else {
        if (entry == parent->left) {
            rotate_right(map, parent);
            parent = entry;
        }
        rotate_left(map, grandpa);
    }

    parent->color = BLACK;
    grandpa->color = RED;
}

static void rotate_left(struct map_t *map, struct entry_t *entry) {
    struct entry_t *parent = entry->parent, *right_child = entry->right;

    entry->right = right_child->left;
    if (right_child->left != NULL)
        right_child->left->parent = entry;

    right_child->left = entry;
    entry->parent = right_child;

    if (parent == NULL)
        map->root = right_child;
    else if (parent->left == entry)
        parent->left = right_child;
    else if (parent->right == entry)
        parent->right = right_child;

    if (right_child != NULL)
        right_child->parent = parent;
}

static void rotate_right(struct map_t *map, struct entry_t *entry) {
    struct entry_t *parent = entry->parent, *left_child = entry->left;

    entry->left = left_child->right;
    if (left_child->right != NULL)
        left_child->right->parent = entry;

    left_child->right = entry;
    entry->parent = left_child;

    if (parent == NULL)
        map->root = left_child;
    else if (parent->left == entry)
        parent->left = left_child;
    else if (parent->right == entry)
        parent->right = left_child;

    if (left_child != NULL)
        left_child->parent = parent;
}
