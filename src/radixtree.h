#ifndef RADIXTREE_H
#define	RADIXTREE_H

#include "structs.h"

typedef enum {
	S_DEFAULT = 0,
	S_FETCHNEXT
} RadixTreeStatus;

void radix_tree_init(Node *tree, char type, unsigned char size, void *child);
void *radix_tree_get(Node *tree, char *string, unsigned int length);
void radix_tree_set(Node *tree, char *string, unsigned int length, void *data);
void *radix_tree_try_set(Node *tree, char *string, unsigned int length, void *data);
void **radix_tree_get_next(Node *tree, char *string, unsigned int length);
void radix_tree_remove(Node *tree, char *string, unsigned int length);
void radix_tree_dispose(Node *tree);

void radix_tree_iterator_init(Iterator *iterator, Node *tree);
void radix_tree_iterator_dispose(Iterator *iterator);
void **radix_tree_iterator_next(Iterator *iterator);


//Integer functions

void *radix_tree_get_int(Node *tree, int number);
void radix_tree_set_int(Node *tree, int number, void *data);
void *radix_tree_get_next_int(Node *tree, int number);


// Padded little-endian integer functions

void *radix_tree_get_ple_int(Node *tree, int number);
void radix_tree_set_ple_int(Node *tree, int number, void *data);
void *radix_tree_get_next_ple_int(Node *tree, int number);

#endif	//RADIXTREE_H


