#ifndef RADIXTREE_H
#define	RADIXTREE_H

#include <stdint.h>
#include "bmap.h"

struct Node;

struct S(BMapEntry, Node);

BMAP_STRUCT(struct Node, Node, node)
	
typedef struct Node {
	BMapNode children;

	//Radix tree specific
	unsigned short size;
	unsigned char *array;
	void *data;
} Node;

BMAP_ENTRY_STRUCT(struct Node, Node, node)
BMAP_CURSOR_STRUCT(struct Node, Node, node)

typedef struct RTree {
	Node root;
} RTree;

DEFINE_BMAP_FUNCTIONS(struct Node, Node, node, PROTOTYPE)


typedef struct _Iterator {
	Node *root;
	unsigned char *key;
	unsigned short size;
	void *data;
} Iterator;

typedef enum {
	S_DEFAULT = 0,
	S_FETCHNEXT
} RadixTreeStatus;

void radix_tree_init(RTree *tree);
void *radix_tree_get(RTree *tree, unsigned char *string, unsigned short length);
void radix_tree_set(RTree *tree, unsigned char *string, unsigned short length, void *data);
int radix_tree_contains(RTree *tree, unsigned char *string, unsigned short length);
void *radix_tree_try_set(RTree *tree, unsigned char *string, unsigned short length, void *data);
void **radix_tree_get_next(RTree *tree, unsigned char *string, unsigned short length);
void **radix_tree_get_prev(RTree *tree, unsigned char *string, unsigned short length);
void radix_tree_remove(RTree *tree, unsigned char *string, unsigned short length);
void radix_tree_dispose(RTree *tree);

void radix_tree_iterator_init(Iterator *iterator, RTree *tree);
void radix_tree_iterator_dispose(Iterator *iterator);
void **radix_tree_iterator_next(Iterator *iterator);


//Integer functions

void *radix_tree_get_int(RTree *tree, int number);
void radix_tree_set_int(RTree *tree, int number, void *data);
int radix_tree_contains_int(RTree *tree, int number);
void *radix_tree_get_next_int(RTree *tree, int number);


// Padded little-endian integer functions

void *radix_tree_get_ple_int(RTree *tree, int number);
void radix_tree_set_ple_int(RTree *tree, int number, void *data);
int radix_tree_contains_ple_int(RTree *tree, int number);
void *radix_tree_get_next_ple_int(RTree *tree, int number);


// intptr_t functions

void *radix_tree_get_intptr(RTree *tree, intptr_t ptr);
void radix_tree_set_intptr(RTree *tree, intptr_t ptr, void *data);
int radix_tree_contains_intptr(RTree *tree, intptr_t ptr);
void *radix_tree_get_next_intptr(RTree *tree, intptr_t ptr);

#endif	//RADIXTREE_H


