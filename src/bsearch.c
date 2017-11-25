#include "bsearch.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "cmemory.h"

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

void bsearch_dispose(Bsearch *bsearch)
{
	free(bsearch->entries);
	set_null(bsearch->entries);
	// TODO: Is it necessary? We should have a generic conditional macro
	// for clean up purposes. Something like: optional(x = NULL);
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
	free(bsearch->entries);
	bsearch->entries = NULL;
	bsearch->count = 0;
}

void bsearch_cursor_init(BsearchCursor *cur, Bsearch *bsearch, _Bool inverse)
{
	cur->bsearch = bsearch;
	cur->step = 1;
	if(bsearch->entries) {
		cur->current = bsearch->entries - 1;
		cur->last = bsearch->entries - 1 + bsearch->count;
	} else {
		cur->current = NULL;
		cur->last = NULL;
	}
}

void bsearch_cursor_revert(BsearchCursor *cur)
{
	BsearchEntry *temp = cur->current;
	if(cur->current) {
		cur->current = cur->last + cur->step;
		cur->last = temp + cur->step;
	}
	cur->step = -cur->step;
}

void bsearch_cursor_dispose(BsearchCursor *cur)
{
	set_null(cur->current);
	set_null(cur->last);
}

_Bool bsearch_cursor_next(BsearchCursor *cur)
{
	_Bool not_last = cur->current != cur->last;
	cur->current += cur->step;
	return not_last;
}

BsearchEntry *bsearch_cursor_current(BsearchCursor *cur)
{
	return cur->current;
}
