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
		BsearchEntry *entry = &entries[i];
		if(entry->key < key) {
			scan->prev = entry;
			left = i+1;
		} else if(entry->key > key) {
			scan->next = entry;
			right = i-1;
		} else {
			scan->equal = entry;
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
	unsigned int index = 0;
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, key);

	if(!scan.equal) {
		if(scan.next) {
			index = scan.next - bsearch->entries;
		} else {
			index = bsearch->count;
		}
		bsearch->count++;
		bsearch->entries = realloc(
			bsearch->entries,
			bsearch->count * sizeof(BsearchEntry)
		);
		check_mem(bsearch->entries);

		memmove(
			bsearch->entries + index + 1,
			bsearch->entries + index,
			(bsearch->count - (index + 1)) * sizeof(BsearchEntry)
		);

		bsearch->entries[index].key = key;
		//TODO: should use node_init
		bsearch->entries[index].node.data = NULL;
	} else {
		return NULL;
	}

	return bsearch->entries + index;
//TODO: Should change interface, possible to return NULL without errors.
error:
	return NULL;
}

int bsearch_delete(Bsearch *bsearch, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, key);
	if(scan.equal) {
		unsigned int index = scan.equal - bsearch->entries;

		memmove(
			bsearch->entries + index,
			bsearch->entries + index + 1,
			(bsearch->count - (index + 1)) * sizeof(BsearchEntry)
		);

		bsearch->count--;

		// TODO: Realloc call may be avoidable. We can just leave the 
		// memory allocated and only call realloc on insert.
		bsearch->entries = realloc(
			bsearch->entries,
			bsearch->count * sizeof(BsearchEntry)
		);

		if(bsearch->count) {
			check_mem(bsearch->entries);
		}
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
