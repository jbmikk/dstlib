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

static void _scan(struct BsearchScan *scan, Bsearch *bsearch, unsigned int size, unsigned char key)
{
	char *entries = (char *)bsearch->entries;
	int left = 0;
	int right = bsearch->count-1;

	//TODO: test binary search with more than 128 elements
	//TODO: At most there will always be 8 iterations, possible to unroll?
	while(left <= right) {
		unsigned int i = left+((right - left)>>1);
		BsearchEntry *entry = (BsearchEntry *)&entries[i * size];
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


/**
 * Ensure previous and next entries are defined if available.
 */
static void _scan_out(struct BsearchScan *scan, Bsearch *bsearch, unsigned int size)
{
	if(scan->equal) {
		if(scan->equal > bsearch->entries) {
			scan->prev = scan->equal - size;
		}
		if(scan->equal < bsearch->entries + ((bsearch->count - 1) * size)) {
			scan->next = scan->equal + size;
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

unsigned int bsearch_count(Bsearch *bsearch)
{
	return bsearch->count;
}

BsearchEntry *bsearch_first(Bsearch *bsearch)
{
	return bsearch->entries;
}

BsearchEntry *bsearch_get(Bsearch *bsearch, unsigned int size, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, size, key);
	return scan.equal;
}

/**
 * Get closest greater than or equal child
 */
BsearchEntry *bsearch_get_gte(Bsearch *bsearch, unsigned int size, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, size, key);
	return scan.equal? scan.equal: scan.next;
}

/**
 * Get closest less than or equal child
 */
BsearchEntry *bsearch_get_lte(Bsearch *bsearch, unsigned int size, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, size, key);
	return scan.equal? scan.equal: scan.prev;
}

/**
 * Get closest greater than key
 */
BsearchEntry *bsearch_get_gt(Bsearch *bsearch, unsigned int size, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, size, key);
	_scan_out(&scan, bsearch, size);
	return scan.next;
}

/**
 * Get closest less than key
 */
BsearchEntry *bsearch_get_lt(Bsearch *bsearch, unsigned int size, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, size, key);
	_scan_out(&scan, bsearch, size);
	return scan.prev;
}

BsearchEntry *bsearch_insert(Bsearch *bsearch, unsigned int size, unsigned char key)
{
	char * entries = (char *)bsearch->entries;
	unsigned int index = 0;
	unsigned int count = bsearch->count * size;
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, size, key);

	if(!scan.equal) {
		if(scan.next) {
			index = ((char *)scan.next) - entries;
		} else {
			index = count;
		}
		entries = realloc(
			entries,
			count + size
		);
		check_mem(entries);

		memmove(
			entries + index + size,
			entries + index,
			count - index
		);

		bsearch->count++;
		bsearch->entries = (BsearchEntry *)entries;

		BsearchEntry *entry = (BsearchEntry *)&entries[index];
		entry->key = key;
		return entry;
	} else {
		return NULL;
	}

//TODO: Should change interface, possible to return NULL without errors.
error:
	return NULL;
}

int bsearch_delete(Bsearch *bsearch, unsigned int size, unsigned char key)
{
	struct BsearchScan scan = {NULL, NULL, NULL};
	_scan(&scan, bsearch, size, key);
	if(scan.equal) {
		char *entries = (char *)bsearch->entries;
		unsigned int index = ((char *)scan.equal) - entries;
		unsigned int count = bsearch->count * size;

		memmove(
			scan.equal,
			scan.equal + size,
			count - index - size
		);

		// TODO: Realloc call may be avoidable. We can just leave the 
		// memory allocated and only call realloc on insert.
		bsearch->entries = realloc(
			entries,
			count - size
		);

		bsearch->count--;
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

void bsearch_cursor_init(BsearchCursor *cur, Bsearch *bsearch, unsigned int size)
{
	cur->bsearch = bsearch;
	cur->step = size;
	if(bsearch->entries) {
		cur->current = bsearch->entries - size;
		cur->last = bsearch->entries + (bsearch->count - 1) * size;
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

void bsearch_cursor_move(BsearchCursor *cur, unsigned int size, unsigned char key)
{
	cur->current = bsearch_get(cur->bsearch, size, key);
}

void bsearch_cursor_move_lt(BsearchCursor *cur, unsigned int size, unsigned char key)
{
	cur->current = bsearch_get_lt(cur->bsearch, size, key);
	if(!cur->current && cur->bsearch->entries) {
		char *entries = (char *)cur->bsearch->entries;
		cur->current = (BsearchEntry *)(entries - size);
	}
}

void bsearch_cursor_move_gt(BsearchCursor *cur, unsigned int size, unsigned char key)
{
	cur->current = bsearch_get_gt(cur->bsearch, size, key);
	if(!cur->current && cur->bsearch->entries) {
		// This equals initial value in reverse mode.
		char *entries = (char *)cur->bsearch->entries;
		cur->current = (BsearchEntry *)(entries + (cur->bsearch->count * size));
	}
}

BsearchEntry *bsearch_cursor_current(BsearchCursor *cur)
{
	return cur->current;
}
