#include "bsearch.h"

#include <stddef.h>
#include <stdio.h>

#include "cmemory.h"
#include "dbg.h"

void bsearch_init(Node *parent)
{
	parent->child = NULL;
	parent->child_count = 0;
}

Node *bsearch_get(Node *parent, unsigned char key)
{
	Node *children = parent->child;
	Node *next;
	int left = 0;
	int right = parent->child_count-1;

	//TODO: test binary search with more than 128 elements
	//TODO: At most there will always be 8 iterations, possible to unroll?
	while(left <= right) {
		unsigned int i = left+((right - left)>>1);
		if(children[i].key < key) {
			left = i+1;
		} else if(children[i].key > key) {
			right = i-1;
		} else {
			next = &(children[i]);
			return next;
		}
	}
	return NULL;
}


/**
 * Get closest greater than or equal child
 */
Node *bsearch_get_gte(Node *parent, unsigned char key)
{
	Node *children = parent->child;
	Node *next = NULL;
	int left = 0;
	int right = parent->child_count-1;

	while(left <= right) {
		unsigned int i = left+((right - left)>>1);
		if(children[i].key < key) {
			left = i+1;
		} else if(children[i].key > key) {
			next = &children[i];
			right = i-1;
		} else {
			next = &children[i];
			break;
		}
	}
	return next;
}

Node *bsearch_insert(Node *parent, unsigned char key)
{
	Node *new_children;
	Node *new_node;
	Node *src = parent->child;
	Node *dst;
	Node *end = src+parent->child_count;

	dst = new_children = c_new(Node, parent->child_count+1);
	check_mem(new_children);

	if(src != NULL) {
		while(src < end && src->key < key)
			*dst++ = *src++;
		new_node = dst++;
		while(src < end)
			*dst++ = *src++;

		c_free(parent->child);
	} else {
		new_node = new_children;
	}

	new_node->key = key;
	//TODO: should use node_init
	new_node->data = NULL;

	//assign new children
	parent->child = new_children;
	parent->child_count++;

	return new_node;
error:
	return NULL;
}

int bsearch_delete(Node *parent, unsigned char key)
{
	if(bsearch_get(parent, key)!= NULL) {
		Node *new_children = NULL;
		if(parent->child_count > 1) {
			new_children = c_new(Node, parent->child_count-1);
			check_mem(new_children);

			Node *dst = new_children;
			Node *src = parent->child;
			Node *end = src+parent->child_count;
			while(src < end && src->key < key)
				*dst++ = *src++;
			src++;
			while(src < end)
				*dst++ = *src++;
		}

		//replace parent's children
		c_free(parent->child);
		parent->child = new_children;
		parent->child_count--;
	}
	return 0;
error:
	return -1;
}

void bsearch_delete_all(Node *parent)
{
	if(parent->child != NULL) {
		c_free(parent->child);
		parent->child = NULL;
		parent->child_count = 0;
	}
}
