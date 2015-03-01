#ifndef RADIXTREE_H
#define	RADIXTREE_H

#include "ctypes.h"
#include "cstruct.h"

typedef enum {
	S_DEFAULT = 0,
	S_FETCHNEXT
} CRadixTreeStatus;

cpointer radix_tree_get(CNode *tree, cchar *string, cuint length);
void radix_tree_set(CNode *tree, cchar *string, cuint length, cpointer data);
cpointer *radix_tree_get_next(CNode *tree, cchar *string, cuint length);
void radix_tree_iterator_init(CNode *tree, CIterator *iterator);
void radix_tree_iterator_dispose(CNode *tree, CIterator *iterator);
cpointer *radix_tree_iterator_next(CNode *tree, CIterator *iterator);

#endif	//RADIXTREE_H


