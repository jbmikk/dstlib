#include "bsearch.h"

#include <stddef.h>
#include <stdio.h>

#include "cmemory.h"
#include "dbg.h"

struct BsearchScan {
	struct BsearchEntry *prev;
	struct BsearchEntry *equal;
	struct BsearchEntry *next;
};

static void _scan(struct BsearchScan *scan, Bsearch *bsearch, unsigned char key)
{
	BsearchEntry *entries = bsearch->entries;
	int left = 0;
	int right = bsearch->count-1;

	//TODO: test binary search with more than 128 elements
	//TODO: At most there will always be 8 iterations, possible to unroll?
	while(left <= right) {
		unsigned int i = left+((right - left)>>1);
		if(entries[i].key < key) {
			scan->prev = &entries[i];
			left = i+1;
		} else if(entries[i].key > key) {
			scan->next = &entries[i];
			right = i-1;
		} else {
			scan->equal = &entries[i];
			break;
		}
	}
}

void bsearch_init(Bsearch *bsearch)
{
	bsearch->entries = NULL;
	bsearch->count = 0;
}

BsearchEntry *bsearch_get(Bsearch *bsearch, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, key);
	return scan.equal;
}

/**
 * Get closest greater than or equal child
 */
BsearchEntry *bsearch_get_gte(Bsearch *bsearch, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, key);
	return scan.equal? scan.equal: scan.next;
}

/**
 * Get closest less than or equal child
 */
BsearchEntry *bsearch_get_lte(Bsearch *bsearch, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, key);
	return scan.equal? scan.equal: scan.prev;
}

BsearchEntry *bsearch_insert(Bsearch *bsearch, unsigned char key)
{
	BsearchEntry *new_entries;
	BsearchEntry *new_node;
	BsearchEntry *src = bsearch->entries;
	BsearchEntry *dst;
	BsearchEntry *end = src + bsearch->count;

	dst = new_entries = c_new(BsearchEntry, bsearch->count + 1);
	check_mem(new_entries);

	if(src != NULL) {
		while(src < end && src->key < key)
			*dst++ = *src++;
		new_node = dst++;
		while(src < end)
			*dst++ = *src++;

		c_free(bsearch->entries);
	} else {
		new_node = new_entries;
	}

	new_node->key = key;
	//TODO: should use node_init
	new_node->node.data = NULL;

	//assign new children
	bsearch->entries = new_entries;
	bsearch->count++;

	return new_node;
error:
	return NULL;
}

int bsearch_delete(Bsearch *bsearch, unsigned char key)
{
	if(bsearch_get(bsearch, key)!= NULL) {
		BsearchEntry *new_entries = NULL;
		if(bsearch->count > 1) {
			new_entries = c_new(BsearchEntry, bsearch->count-1);
			check_mem(new_entries);

			BsearchEntry *dst = new_entries;
			BsearchEntry *src = bsearch->entries;
			BsearchEntry *end = src + bsearch->count;
			while(src < end && src->key < key)
				*dst++ = *src++;
			src++;
			while(src < end)
				*dst++ = *src++;
		}

		//replace bsearch's entries
		c_free(bsearch->entries);
		bsearch->entries = new_entries;
		bsearch->count--;
	}
	return 0;
error:
	return -1;
}

void bsearch_delete_all(Bsearch *bsearch)
{
	if(bsearch->entries != NULL) {
		c_free(bsearch->entries);
		bsearch->entries = NULL;
		bsearch->count = 0;
	}
}
