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

void rtree_init(RTree *tree);
void *rtree_get(RTree *tree, unsigned char *string, unsigned short length);
void rtree_set(RTree *tree, unsigned char *string, unsigned short length, void *data);
int rtree_contains(RTree *tree, unsigned char *string, unsigned short length);
void *rtree_try_set(RTree *tree, unsigned char *string, unsigned short length, void *data);
void **rtree_get_next(RTree *tree, unsigned char *string, unsigned short length);
void **rtree_get_prev(RTree *tree, unsigned char *string, unsigned short length);
void rtree_remove(RTree *tree, unsigned char *string, unsigned short length);
void rtree_dispose(RTree *tree);

void rtree_iterator_init(Iterator *iterator, RTree *tree);
void rtree_iterator_dispose(Iterator *iterator);
void **rtree_iterator_next(Iterator *iterator);


//Integer functions

void *rtree_get_int(RTree *tree, int number);
void rtree_set_int(RTree *tree, int number, void *data);
int rtree_contains_int(RTree *tree, int number);
void *rtree_get_next_int(RTree *tree, int number);


// Padded little-endian integer functions

void *rtree_get_ple_int(RTree *tree, int number);
void rtree_set_ple_int(RTree *tree, int number, void *data);
int rtree_contains_ple_int(RTree *tree, int number);
void *rtree_get_next_ple_int(RTree *tree, int number);


// intptr_t functions

void *rtree_get_intptr(RTree *tree, intptr_t ptr);
void rtree_set_intptr(RTree *tree, intptr_t ptr, void *data);
int rtree_contains_intptr(RTree *tree, intptr_t ptr);
void *rtree_get_next_intptr(RTree *tree, intptr_t ptr);

#endif	//RADIXTREE_H


