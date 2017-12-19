#ifndef BMAP_H
#define	BMAP_H

#include <stdbool.h>

#include "generics.h"

struct BMapEntry;

typedef struct BMap {
	struct BMapEntry *entries;
	unsigned char count;
} BMap;

struct BMapEntry {
	unsigned char key;
};

typedef struct BMapEntry BMapEntry;

typedef struct BMapCursor {
	BMap *bmap;
	BMapEntry *current;
	BMapEntry *last;
	int step;
} BMapCursor;

void bmap_init(BMap *bmap);
void bmap_dispose(BMap *bmap);
unsigned int bmap_count(BMap *bmap);
BMapEntry *bmap_first(BMap *bmap);
BMapEntry *bmap_get(BMap *bmap, unsigned int size, unsigned char key);
BMapEntry *bmap_insert(BMap *bmap, unsigned int size, unsigned char key);
BMapEntry *bmap_get_gte(BMap *bmap, unsigned int size, unsigned char key);
BMapEntry *bmap_get_lte(BMap *bmap, unsigned int size, unsigned char key);
BMapEntry *bmap_get_gt(BMap *bmap, unsigned int size, unsigned char key);
BMapEntry *bmap_get_lt(BMap *bmap, unsigned int size, unsigned char key);
int bmap_delete(BMap *bmap, unsigned int size, unsigned char key);
void bmap_delete_all(BMap *bmap);

void bmap_cursor_init(BMapCursor *cur, BMap *bmap, unsigned int size);
void bmap_cursor_revert(BMapCursor *cur);
void bmap_cursor_dispose(BMapCursor *cur);
bool bmap_cursor_next(BMapCursor *cur);
void bmap_cursor_move(BMapCursor *cur, unsigned int size, unsigned char key);
void bmap_cursor_move_lt(BMapCursor *cur, unsigned int size, unsigned char key);
void bmap_cursor_move_gt(BMapCursor *cur, unsigned int size, unsigned char key);
BMapEntry *bmap_cursor_current(BMapCursor *cur);

#define PROTOTYPE(BLOCK) ;
#define IMPLEMENTATION(BLOCK) BLOCK

#define BMAP_ENTRY_STRUCT(TYPE, UPPER, LOWER) \
typedef struct S(BMapEntry, UPPER) { \
	BMapEntry entry; \
	TYPE LOWER; \
} S(BMapEntry, UPPER);

#define BMAP_STRUCT(TYPE, UPPER, LOWER) \
typedef struct S(BMap, UPPER) { \
	BMap bmap; \
} S(BMap, UPPER);

#define BMAP_CURSOR_STRUCT(TYPE, UPPER, LOWER) \
typedef struct S(BMapCursor, UPPER) { \
	BMapCursor cursor; \
} S(BMapCursor, UPPER);


#define BMAP_INIT(TYPE, UPPER, LOWER, BODY) \
void bmap_##LOWER##_init(struct S(BMap, UPPER) *bmap) BODY({ \
	bmap_init(&bmap->bmap); \
})

#define BMAP_DISPOSE(TYPE, UPPER, LOWER, BODY) \
void bmap_##LOWER##_dispose(struct S(BMap, UPPER) *bmap) BODY({ \
	bmap_dispose(&bmap->bmap); \
})

#define BMAP_INSERT(TYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_insert( \
	struct S(BMap, UPPER) *bmap, \
	unsigned char key, \
	TYPE LOWER \
) BODY({ \
	struct S(BMapEntry, UPPER) *entry = (struct S(BMapEntry, UPPER) *)bmap_insert( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
	if(entry) { \
		entry->LOWER = LOWER; \
	} \
	return entry; \
})

#define BMAP_COUNT(TYPE, UPPER, LOWER, BODY) \
unsigned int bmap_##LOWER##_count( \
	struct S(BMap, UPPER) *bmap \
) BODY({ \
	return bmap_count(&bmap->bmap); \
})

#define BMAP_FIRST(TYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_first( \
	struct S(BMap, UPPER) *bmap \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_first(&bmap->bmap); \
})

#define BMAP_GET(TYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get( \
	struct S(BMap, UPPER) *bmap, \
	unsigned char key \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_get( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_GET_GTE(TYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get_gte( \
	struct S(BMap, UPPER) *bmap, \
	unsigned char key \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_get_gte( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_GET_LTE(TYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get_lte( \
	struct S(BMap, UPPER) *bmap, \
	unsigned char key \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_get_lte( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_GET_GT(TYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get_gt( \
	struct S(BMap, UPPER) *bmap, \
	unsigned char key \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_get_gt( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_GET_LT(TYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get_lt( \
	struct S(BMap, UPPER) *bmap, \
	unsigned char key \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_get_lt( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_DELETE(TYPE, UPPER, LOWER, BODY) \
int bmap_##LOWER##_delete( \
	struct S(BMap, UPPER) *bmap, \
	unsigned char key \
) BODY({ \
	return bmap_delete( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_CURSOR_INIT(TYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_init( \
	struct S(BMapCursor, UPPER) *cursor, \
	struct S(BMap, UPPER) *bmap \
) BODY({ \
	bmap_cursor_init( \
		&cursor->cursor, \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)) \
	); \
})

#define BMAP_CURSOR_REVERT(TYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_revert( \
	struct S(BMapCursor, UPPER) *cursor \
) BODY({ \
	bmap_cursor_revert(&cursor->cursor); \
})

#define BMAP_CURSOR_DISPOSE(TYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_dispose( \
	struct S(BMapCursor, UPPER) *cursor \
) BODY({ \
	bmap_cursor_dispose(&cursor->cursor); \
})

#define BMAP_CURSOR_NEXT(TYPE, UPPER, LOWER, BODY) \
bool bmap_cursor_##LOWER##_next( \
	struct S(BMapCursor, UPPER) *cursor \
) BODY({ \
	return bmap_cursor_next(&cursor->cursor); \
})

#define BMAP_CURSOR_MOVE(TYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_move( \
	struct S(BMapCursor, UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bmap_cursor_move( \
		&cursor->cursor, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_CURSOR_MOVE_LT(TYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_move_lt( \
	struct S(BMapCursor, UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bmap_cursor_move_lt( \
		&cursor->cursor, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_CURSOR_MOVE_GT(TYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_move_gt( \
	struct S(BMapCursor, UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bmap_cursor_move_gt( \
		&cursor->cursor, \
		sizeof(struct S(BMapEntry, UPPER)), \
		key \
	); \
})

#define BMAP_CURSOR_CURRENT(TYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_cursor_##LOWER##_current( \
	struct S(BMapCursor, UPPER) *cursor \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_cursor_current( \
		&cursor->cursor \
	); \
})


#define DEFINE_BMAP_FUNCTIONS(TYPE, UPPER, LOWER, BODY) \
	BMAP_INIT(TYPE, UPPER, LOWER, BODY) \
	BMAP_DISPOSE(TYPE, UPPER, LOWER, BODY) \
	BMAP_INSERT(TYPE, UPPER, LOWER, BODY) \
	BMAP_COUNT(TYPE, UPPER, LOWER, BODY) \
	BMAP_FIRST(TYPE, UPPER, LOWER, BODY) \
	BMAP_GET(TYPE, UPPER, LOWER, BODY) \
	BMAP_GET_GTE(TYPE, UPPER, LOWER, BODY) \
	BMAP_GET_LTE(TYPE, UPPER, LOWER, BODY) \
	BMAP_GET_GT(TYPE, UPPER, LOWER, BODY) \
	BMAP_GET_LT(TYPE, UPPER, LOWER, BODY) \
	BMAP_DELETE(TYPE, UPPER, LOWER, BODY) \
	BMAP_CURSOR_INIT(TYPE, UPPER, LOWER, BODY) \
	BMAP_CURSOR_REVERT(TYPE, UPPER, LOWER, BODY) \
	BMAP_CURSOR_DISPOSE(TYPE, UPPER, LOWER, BODY) \
	BMAP_CURSOR_NEXT(TYPE, UPPER, LOWER, BODY) \
	BMAP_CURSOR_MOVE(TYPE, UPPER, LOWER, BODY) \
	BMAP_CURSOR_MOVE_LT(TYPE, UPPER, LOWER, BODY) \
	BMAP_CURSOR_MOVE_GT(TYPE, UPPER, LOWER, BODY) \
	BMAP_CURSOR_CURRENT(TYPE, UPPER, LOWER, BODY)

#define DEFINE_BMAP(TYPE, UPPER, LOWER) \
	BMAP_ENTRY_STRUCT(TYPE, UPPER, LOWER) \
	BMAP_STRUCT(TYPE, UPPER, LOWER) \
	BMAP_CURSOR_STRUCT(TYPE, UPPER, LOWER) \
	DEFINE_BMAP_FUNCTIONS(TYPE, UPPER, LOWER, PROTOTYPE)

#endif	//BMAP_H


