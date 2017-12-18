#ifndef BSEARCH_H
#define	BSEARCH_H

#include <stdbool.h>

#include "generics.h"

struct BsearchEntry;

typedef struct Bsearch {
	struct BsearchEntry *entries;
	unsigned char count;
} Bsearch;

struct BsearchEntry {
	unsigned char key;
};

typedef struct BsearchEntry BsearchEntry;

typedef struct BsearchCursor {
	Bsearch *bsearch;
	BsearchEntry *current;
	BsearchEntry *last;
	int step;
} BsearchCursor;

void bsearch_init(Bsearch *bsearch);
void bsearch_dispose(Bsearch *bsearch);
unsigned int bsearch_count(Bsearch *bsearch);
BsearchEntry *bsearch_first(Bsearch *bsearch);
BsearchEntry *bsearch_get(Bsearch *bsearch, unsigned int size, unsigned char key);
BsearchEntry *bsearch_insert(Bsearch *bsearch, unsigned int size, unsigned char key);
BsearchEntry *bsearch_get_gte(Bsearch *bsearch, unsigned int size, unsigned char key);
BsearchEntry *bsearch_get_lte(Bsearch *bsearch, unsigned int size, unsigned char key);
BsearchEntry *bsearch_get_gt(Bsearch *bsearch, unsigned int size, unsigned char key);
BsearchEntry *bsearch_get_lt(Bsearch *bsearch, unsigned int size, unsigned char key);
int bsearch_delete(Bsearch *bsearch, unsigned int size, unsigned char key);
void bsearch_delete_all(Bsearch *bsearch);

void bsearch_cursor_init(BsearchCursor *cur, Bsearch *bsearch, unsigned int size);
void bsearch_cursor_revert(BsearchCursor *cur);
void bsearch_cursor_dispose(BsearchCursor *cur);
bool bsearch_cursor_next(BsearchCursor *cur);
void bsearch_cursor_move(BsearchCursor *cur, unsigned int size, unsigned char key);
void bsearch_cursor_move_lt(BsearchCursor *cur, unsigned int size, unsigned char key);
void bsearch_cursor_move_gt(BsearchCursor *cur, unsigned int size, unsigned char key);
BsearchEntry *bsearch_cursor_current(BsearchCursor *cur);

#define PROTOTYPE(BLOCK) ;
#define IMPLEMENTATION(BLOCK) BLOCK

#define BSEARCH_ENTRY_STRUCT(TYPE, UPPER, LOWER) \
typedef struct S(BsearchEntry, UPPER) { \
	BsearchEntry entry; \
	TYPE LOWER; \
} S(BsearchEntry, UPPER);

#define BSEARCH_STRUCT(TYPE, UPPER, LOWER) \
typedef struct S(Bsearch, UPPER) { \
	Bsearch bsearch; \
} S(Bsearch, UPPER);

#define BSEARCH_CURSOR_STRUCT(TYPE, UPPER, LOWER) \
typedef struct S(BsearchCursor, UPPER) { \
	BsearchCursor cursor; \
} S(BsearchCursor, UPPER);


#define BSEARCH_INIT(TYPE, UPPER, LOWER, BODY) \
void bsearch_##LOWER##_init(struct S(Bsearch, UPPER) *bsearch) BODY({ \
	bsearch_init(&bsearch->bsearch); \
})

#define BSEARCH_DISPOSE(TYPE, UPPER, LOWER, BODY) \
void bsearch_##LOWER##_dispose(struct S(Bsearch, UPPER) *bsearch) BODY({ \
	bsearch_dispose(&bsearch->bsearch); \
})

#define BSEARCH_INSERT(TYPE, UPPER, LOWER, BODY) \
S(BsearchEntry, UPPER) *bsearch_##LOWER##_insert( \
	struct S(Bsearch, UPPER) *bsearch, \
	unsigned char key, \
	TYPE LOWER \
) BODY({ \
	struct S(BsearchEntry, UPPER) *entry = (struct S(BsearchEntry, UPPER) *)bsearch_insert( \
		&bsearch->bsearch, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
	if(entry) { \
		entry->LOWER = LOWER; \
	} \
	return entry; \
})

#define BSEARCH_COUNT(TYPE, UPPER, LOWER, BODY) \
unsigned int bsearch_##LOWER##_count( \
	struct S(Bsearch, UPPER) *bsearch \
) BODY({ \
	return bsearch_count(&bsearch->bsearch); \
})

#define BSEARCH_FIRST(TYPE, UPPER, LOWER, BODY) \
S(BsearchEntry, UPPER) *bsearch_##LOWER##_first( \
	struct S(Bsearch, UPPER) *bsearch \
) BODY({ \
	return (S(BsearchEntry, UPPER) *)bsearch_first(&bsearch->bsearch); \
})

#define BSEARCH_GET(TYPE, UPPER, LOWER, BODY) \
S(BsearchEntry, UPPER) *bsearch_##LOWER##_get( \
	struct S(Bsearch, UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (S(BsearchEntry, UPPER) *)bsearch_get( \
		&bsearch->bsearch, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_GET_GTE(TYPE, UPPER, LOWER, BODY) \
S(BsearchEntry, UPPER) *bsearch_##LOWER##_get_gte( \
	struct S(Bsearch, UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (S(BsearchEntry, UPPER) *)bsearch_get_gte( \
		&bsearch->bsearch, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_GET_LTE(TYPE, UPPER, LOWER, BODY) \
S(BsearchEntry, UPPER) *bsearch_##LOWER##_get_lte( \
	struct S(Bsearch, UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (S(BsearchEntry, UPPER) *)bsearch_get_lte( \
		&bsearch->bsearch, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_GET_GT(TYPE, UPPER, LOWER, BODY) \
S(BsearchEntry, UPPER) *bsearch_##LOWER##_get_gt( \
	struct S(Bsearch, UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (S(BsearchEntry, UPPER) *)bsearch_get_gt( \
		&bsearch->bsearch, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_GET_LT(TYPE, UPPER, LOWER, BODY) \
S(BsearchEntry, UPPER) *bsearch_##LOWER##_get_lt( \
	struct S(Bsearch, UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (S(BsearchEntry, UPPER) *)bsearch_get_lt( \
		&bsearch->bsearch, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_DELETE(TYPE, UPPER, LOWER, BODY) \
int bsearch_##LOWER##_delete( \
	struct S(Bsearch, UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return bsearch_delete( \
		&bsearch->bsearch, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_CURSOR_INIT(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_init( \
	struct S(BsearchCursor, UPPER) *cursor, \
	struct S(Bsearch, UPPER) *bsearch \
) BODY({ \
	bsearch_cursor_init( \
		&cursor->cursor, \
		&bsearch->bsearch, \
		sizeof(struct S(BsearchEntry, UPPER)) \
	); \
})

#define BSEARCH_CURSOR_REVERT(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_revert( \
	struct S(BsearchCursor, UPPER) *cursor \
) BODY({ \
	bsearch_cursor_revert(&cursor->cursor); \
})

#define BSEARCH_CURSOR_DISPOSE(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_dispose( \
	struct S(BsearchCursor, UPPER) *cursor \
) BODY({ \
	bsearch_cursor_dispose(&cursor->cursor); \
})

#define BSEARCH_CURSOR_NEXT(TYPE, UPPER, LOWER, BODY) \
bool bsearch_cursor_##LOWER##_next( \
	struct S(BsearchCursor, UPPER) *cursor \
) BODY({ \
	return bsearch_cursor_next(&cursor->cursor); \
})

#define BSEARCH_CURSOR_MOVE(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_move( \
	struct S(BsearchCursor, UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bsearch_cursor_move( \
		&cursor->cursor, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_CURSOR_MOVE_LT(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_move_lt( \
	struct S(BsearchCursor, UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bsearch_cursor_move_lt( \
		&cursor->cursor, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_CURSOR_MOVE_GT(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_move_gt( \
	struct S(BsearchCursor, UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bsearch_cursor_move_gt( \
		&cursor->cursor, \
		sizeof(struct S(BsearchEntry, UPPER)), \
		key \
	); \
})

#define BSEARCH_CURSOR_CURRENT(TYPE, UPPER, LOWER, BODY) \
S(BsearchEntry, UPPER) *bsearch_cursor_##LOWER##_current( \
	struct S(BsearchCursor, UPPER) *cursor \
) BODY({ \
	return (S(BsearchEntry, UPPER) *)bsearch_cursor_current( \
		&cursor->cursor \
	); \
})


#define DEFINE_BSEARCH_FUNCTIONS(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_INIT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_DISPOSE(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_INSERT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_COUNT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_FIRST(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_GET(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_GET_GTE(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_GET_LTE(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_GET_GT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_GET_LT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_DELETE(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_CURSOR_INIT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_CURSOR_REVERT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_CURSOR_DISPOSE(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_CURSOR_NEXT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_CURSOR_MOVE(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_CURSOR_MOVE_LT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_CURSOR_MOVE_GT(TYPE, UPPER, LOWER, BODY) \
	BSEARCH_CURSOR_CURRENT(TYPE, UPPER, LOWER, BODY)

#define DEFINE_BSEARCH(TYPE, UPPER, LOWER) \
	BSEARCH_ENTRY_STRUCT(TYPE, UPPER, LOWER) \
	BSEARCH_STRUCT(TYPE, UPPER, LOWER) \
	BSEARCH_CURSOR_STRUCT(TYPE, UPPER, LOWER) \
	DEFINE_BSEARCH_FUNCTIONS(TYPE, UPPER, LOWER, PROTOTYPE)

#endif	//BSEARCH_H


