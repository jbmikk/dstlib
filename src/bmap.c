#include "bmap.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "cmemory.h"

struct BMapScan {
	struct BMapEntry *prev;
	struct BMapEntry *equal;
	struct BMapEntry *next;
	int eq_diff;
};

int char_key_compare(const BMapComparator *c, const BMapEntry *e)
{
	//return e->key - c->uchar_key;
	// Assume key is always the first member.
	return *((typeof(c->char_key)*)e) - c->char_key;
}

int uchar_key_compare(const BMapComparator *c, const BMapEntry *e)
{
	//return e->key - c->uchar_key;
	// Assume key is always the first member.
	return *((typeof(c->uchar_key)*)e) - c->uchar_key;
}

int int_key_compare(const BMapComparator *c, const BMapEntry *e)
{
	return *((typeof(c->int_key)*)e) - c->int_key;
}

int uint_key_compare(const BMapComparator *c, const BMapEntry *e)
{
	return *((typeof(c->uint_key)*)e) - c->uint_key;
}

int long_key_compare(const BMapComparator *c, const BMapEntry *e)
{
	return *((typeof(c->long_key)*)e) - c->long_key;
}

int ulong_key_compare(const BMapComparator *c, const BMapEntry *e)
{
	return *((typeof(c->ulong_key)*)e) - c->ulong_key;
}

// TODO: Refactor binary search into separate generic from BMap.
//	 Bsearch generic should work without using key-value mapping.
//	 Pretty much like standard c bsearch, but with generic structure.
static void _scan(struct BMapScan *scan, BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	char *entries = (char *)bmap->entries;
	int left = 0;
	int right = bmap->count-1;

	//TODO: test binary search with more than 128 elements
	//TODO: At most there will always be 8 iterations, possible to unroll?
	while(left <= right) {
		unsigned int i = left+((right - left)>>1);
		BMapEntry *entry = (BMapEntry *)&entries[i * size];
		int diff = cmp->compare(cmp, entry);
		if(diff < 0) {
			scan->prev = entry;
			left = i+1;
		} else if(diff > 0) {
			scan->next = entry;
			right = i-1;
		} else {
			scan->equal = entry;
			break;
		}
	}
}

static void _scan_previous_key(struct BMapScan *scan, BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	char *entries = (char *)bmap->entries;
	int left = 0;
	int right = bmap->count-1;

	while(left <= right) {
		unsigned int i = left+((right - left)>>1);
		BMapEntry *entry = (BMapEntry *)&entries[i * size];
		int diff = cmp->compare(cmp, entry);
		if(diff < 0) {
			scan->prev = entry;
			left = i+1;
		} else if(diff >= 0) {
			scan->next = entry;
			right = i-1;
			scan->equal = entry;
			scan->eq_diff = diff;
		}
	}
}

static void _scan_next_key(struct BMapScan *scan, BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	char *entries = (char *)bmap->entries;
	int left = 0;
	int right = bmap->count-1;

	while(left <= right) {
		unsigned int i = left+((right - left)>>1);
		BMapEntry *entry = (BMapEntry *)&entries[i * size];
		int diff = cmp->compare(cmp, entry);
		if(diff <= 0) {
			scan->prev = entry;
			left = i+1;
			scan->equal = entry;
			scan->eq_diff = diff;
		} else if(diff > 0) {
			scan->next = entry;
			right = i-1;
		}
	}
}


/**
 * Ensure next entry is defined if available.
 */
static void _scan_next(struct BMapScan *scan, BMap *bmap, unsigned int size)
{
	char *entry = (char *)scan->equal;
	char *entries = (char *)bmap->entries;
	if(scan->equal) {
		if(entry < entries + ((bmap->count - 1) * size)) {
			scan->next = (BMapEntry *)(entry + size);
		}
	}
}

/**
 * Ensure previous entry is defined if available.
 */
static void _scan_previous(struct BMapScan *scan, BMap *bmap, unsigned int size)
{
	if(scan->equal) {
		if(scan->equal > bmap->entries) {
			scan->prev = (void*)scan->equal - size;
		}
	}
}

void bmap_init(BMap *bmap)
{
	bmap->entries = NULL;
	bmap->count = 0;
}

void bmap_dispose(BMap *bmap)
{
	free(bmap->entries);
	set_null(bmap->entries);
	// TODO: Is it necessary? We should have a generic conditional macro
	// for clean up purposes. Something like: optional(x = NULL);
	bmap->count = 0;
}

unsigned int bmap_count(BMap *bmap)
{
	return bmap->count;
}

BMapEntry *bmap_first(BMap *bmap)
{
	return bmap->entries;
}

BMapEntry *bmap_get(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan(&scan, bmap, size, cmp);
	return scan.equal;
}

int bmap_get_index(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	char *entry = (char *)bmap_get(bmap, size, cmp);
	char *entries = (char *)bmap->entries;
	// TODO: Could division be optimized?
	return entry? (entry - entries) / size: -1;
}

/**
 * Get closest greater than or equal child
 */
BMapEntry *bmap_get_gte(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan(&scan, bmap, size, cmp);
	return scan.equal? scan.equal: scan.next;
}

/**
 * Get closest less than or equal child
 */
BMapEntry *bmap_get_lte(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan(&scan, bmap, size, cmp);
	return scan.equal? scan.equal: scan.prev;
}

/**
 * Get closest greater than key
 */
BMapEntry *bmap_get_gt(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan(&scan, bmap, size, cmp);
	_scan_next(&scan, bmap, size);
	return scan.next;
}

/**
 * Get closest less than key
 */
BMapEntry *bmap_get_lt(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan(&scan, bmap, size, cmp);
	_scan_previous(&scan, bmap, size);
	return scan.prev;
}

BMapEntry *bmap_m_get(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan_previous_key(&scan, bmap, size, cmp);

	return scan.eq_diff == 0? scan.equal: NULL;
}

BMapEntry *bmap_m_get_at(BMap *bmap, unsigned int size, BMapComparator *cmp, int index)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan_previous_key(&scan, bmap, size, cmp);

	BMapEntry *result;
	if(scan.next) {
		char *entries = (char *)bmap->entries;
		char *end = entries + (bmap->count * size);

		result = (BMapEntry *)(((char *)scan.next) + size * index);
		// TODO: validate > entries?
		if((char *)result >= end || cmp->compare(cmp, result)) {
			result = NULL;
		}
	} else {
		result = NULL;
	}
	return result;
}


static BMapEntry *_prepend(BMap *bmap, unsigned int size, BMapEntry *entry)
{
	char *entries = (char *)bmap->entries;
	unsigned int index;
	unsigned int count = bmap->count * size;
	if(entry) {
		index = ((char *)entry) - entries;
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

	bmap->count++;
	bmap->entries = (BMapEntry *)entries;

	return (BMapEntry *)&entries[index];
error:
	return NULL;
}

BMapEntry *bmap_insert(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan(&scan, bmap, size, cmp);

	if(!scan.equal) {
		return _prepend(bmap, size, scan.next);
	} else {
		return NULL;
	}

//TODO: Should change interface, possible to return NULL without errors.
	return NULL;
}

BMapEntry *bmap_m_append(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan_next_key(&scan, bmap, size, cmp);

	return _prepend(bmap, size, scan.next);
}

BMapEntry *bmap_m_prepend(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan_previous_key(&scan, bmap, size, cmp);

	return _prepend(bmap, size, scan.next);
}

int bmap_delete(BMap *bmap, unsigned int size, BMapComparator *cmp)
{
	struct BMapScan scan = {NULL, NULL, NULL, 0};
	_scan(&scan, bmap, size, cmp);
	if(scan.equal) {
		char *entries = (char *)bmap->entries;
		unsigned int index = ((char *)scan.equal) - entries;
		unsigned int count = bmap->count * size;

		memmove(
			scan.equal,
			(void*)scan.equal + size,
			count - index - size
		);

		// TODO: Realloc call may be avoidable. We can just leave the 
		// memory allocated and only call realloc on insert.
		bmap->entries = realloc(
			entries,
			count - size
		);

		bmap->count--;
		if(bmap->count) {
			check_mem(bmap->entries);
		}
	}
	return 0;
error:
	return -1;
}

void bmap_delete_all(BMap *bmap)
{
	free(bmap->entries);
	bmap->entries = NULL;
	bmap->count = 0;
}

void bmap_cursor_init(BMapCursor *cur, BMap *bmap, unsigned int size)
{
	cur->bmap = bmap;
	cur->step = size;
	if(bmap->entries) {
		cur->current = (void*)bmap->entries - size;
		cur->last = (void*)bmap->entries + (bmap->count - 1) * size;
	} else {
		cur->current = NULL;
		cur->last = NULL;
	}
}

void bmap_cursor_revert(BMapCursor *cur)
{
	BMapEntry *temp = cur->current;
	if(cur->current) {
		cur->current = (void*)cur->last + cur->step;
		cur->last = (void*)temp + cur->step;
	}
	cur->step = -cur->step;
}

void bmap_cursor_dispose(BMapCursor *cur)
{
	set_null(cur->current);
	set_null(cur->last);
}

_Bool bmap_cursor_next(BMapCursor *cur)
{
	_Bool not_last = cur->current != cur->last;
	cur->current = (void*)cur->current + cur->step;
	return not_last;
}

void bmap_cursor_move(BMapCursor *cur, unsigned int size, BMapComparator *cmp)
{
	cur->current = bmap_get(cur->bmap, size, cmp);
}

void bmap_cursor_move_lt(BMapCursor *cur, unsigned int size, BMapComparator *cmp)
{
	cur->current = bmap_get_lt(cur->bmap, size, cmp);
	if(!cur->current && cur->bmap->entries) {
		char *entries = (char *)cur->bmap->entries;
		cur->current = (BMapEntry *)(entries - size);
	}
}

void bmap_cursor_move_gt(BMapCursor *cur, unsigned int size, BMapComparator *cmp)
{
	cur->current = bmap_get_gt(cur->bmap, size, cmp);
	if(!cur->current && cur->bmap->entries) {
		// This equals initial value in reverse mode.
		char *entries = (char *)cur->bmap->entries;
		cur->current = (BMapEntry *)(entries + (cur->bmap->count * size));
	}
}

BMapEntry *bmap_cursor_current(BMapCursor *cur)
{
	return cur->current;
}
