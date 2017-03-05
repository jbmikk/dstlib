#ifndef RADIXTREE_H
#define	RADIXTREE_H

#include "structs.h"

#include <stdint.h>

typedef enum {
	S_DEFAULT = 0,
	S_FETCHNEXT
} RadixTreeStatus;

void radix_tree_init(Node *tree);
void *radix_tree_get(Node *tree, unsigned char *string, unsigned short length);
void radix_tree_set(Node *tree, unsigned char *string, unsigned short length, void *data);
int radix_tree_contains(Node *tree, unsigned char *string, unsigned short length);
void *radix_tree_try_set(Node *tree, unsigned char *string, unsigned short length, void *data);
void **radix_tree_get_next(Node *tree, unsigned char *string, unsigned short length);
void **radix_tree_get_prev(Node *tree, unsigned char *string, unsigned short length);
void radix_tree_remove(Node *tree, unsigned char *string, unsigned short length);
void radix_tree_dispose(Node *tree);

void radix_tree_iterator_init(Iterator *iterator, Node *tree);
void radix_tree_iterator_dispose(Iterator *iterator);
void **radix_tree_iterator_next(Iterator *iterator);


//Integer functions

void *radix_tree_get_int(Node *tree, int number);
void radix_tree_set_int(Node *tree, int number, void *data);
int radix_tree_contains_int(Node *tree, int number);
void *radix_tree_get_next_int(Node *tree, int number);


// Padded little-endian integer functions

void *radix_tree_get_ple_int(Node *tree, int number);
void radix_tree_set_ple_int(Node *tree, int number, void *data);
int radix_tree_contains_ple_int(Node *tree, int number);
void *radix_tree_get_next_ple_int(Node *tree, int number);


// intptr_t functions

void *radix_tree_get_intptr(Node *tree, intptr_t ptr);
void radix_tree_set_intptr(Node *tree, intptr_t ptr, void *data);
int radix_tree_contains_intptr(Node *tree, intptr_t ptr);
void *radix_tree_get_next_intptr(Node *tree, intptr_t ptr);

#endif	//RADIXTREE_H


