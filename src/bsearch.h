#ifndef BSEARCH_H
#define	BSEARCH_H

#include <stdbool.h>

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

#define BSEARCH_ENTRY_TYPE(NAME) BsearchEntry##NAME
#define BSEARCH_TYPE(NAME) Bsearch##NAME
#define BSEARCH_CURSOR_TYPE(NAME) BsearchCursor##NAME

#define BSEARCH_ENTRY_STRUCT(TYPE, UPPER, LOWER) \
typedef struct BSEARCH_ENTRY_TYPE(UPPER) { \
	BsearchEntry entry; \
	TYPE LOWER; \
} BSEARCH_ENTRY_TYPE(UPPER);

#define BSEARCH_STRUCT(TYPE, UPPER, LOWER) \
typedef struct BSEARCH_TYPE(UPPER) { \
	Bsearch bsearch; \
} BSEARCH_TYPE(UPPER);

#define BSEARCH_CURSOR_STRUCT(TYPE, UPPER, LOWER) \
typedef struct BSEARCH_CURSOR_TYPE(UPPER) { \
	BsearchCursor cursor; \
} BSEARCH_CURSOR_TYPE(UPPER);


#define BSEARCH_INIT(TYPE, UPPER, LOWER, BODY) \
void bsearch_##LOWER##_init(struct BSEARCH_TYPE(UPPER) *bsearch) BODY({ \
	bsearch_init(&bsearch->bsearch); \
})

#define BSEARCH_DISPOSE(TYPE, UPPER, LOWER, BODY) \
void bsearch_##LOWER##_dispose(struct BSEARCH_TYPE(UPPER) *bsearch) BODY({ \
	bsearch_dispose(&bsearch->bsearch); \
})

#define BSEARCH_INSERT(TYPE, UPPER, LOWER, BODY) \
BSEARCH_ENTRY_TYPE(UPPER) *bsearch_##LOWER##_insert( \
	struct BSEARCH_TYPE(UPPER) *bsearch, \
	unsigned char key, \
	TYPE LOWER \
) BODY({ \
	struct BSEARCH_ENTRY_TYPE(UPPER) *entry = (struct BSEARCH_ENTRY_TYPE(UPPER) *)bsearch_insert( \
		&bsearch->bsearch, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
	if(entry) { \
		entry->LOWER = LOWER; \
	} \
	return entry; \
})

#define BSEARCH_COUNT(TYPE, UPPER, LOWER, BODY) \
unsigned int bsearch_##LOWER##_count( \
	struct BSEARCH_TYPE(UPPER) *bsearch \
) BODY({ \
	return bsearch_count(&bsearch->bsearch); \
})

#define BSEARCH_FIRST(TYPE, UPPER, LOWER, BODY) \
BSEARCH_ENTRY_TYPE(UPPER) *bsearch_##LOWER##_first( \
	struct BSEARCH_TYPE(UPPER) *bsearch \
) BODY({ \
	return (BSEARCH_ENTRY_TYPE(UPPER) *)bsearch_first(&bsearch->bsearch); \
})

#define BSEARCH_GET(TYPE, UPPER, LOWER, BODY) \
BSEARCH_ENTRY_TYPE(UPPER) *bsearch_##LOWER##_get( \
	struct BSEARCH_TYPE(UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (BSEARCH_ENTRY_TYPE(UPPER) *)bsearch_get( \
		&bsearch->bsearch, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_GET_GTE(TYPE, UPPER, LOWER, BODY) \
BSEARCH_ENTRY_TYPE(UPPER) *bsearch_##LOWER##_get_gte( \
	struct BSEARCH_TYPE(UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (BSEARCH_ENTRY_TYPE(UPPER) *)bsearch_get_gte( \
		&bsearch->bsearch, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_GET_LTE(TYPE, UPPER, LOWER, BODY) \
BSEARCH_ENTRY_TYPE(UPPER) *bsearch_##LOWER##_get_lte( \
	struct BSEARCH_TYPE(UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (BSEARCH_ENTRY_TYPE(UPPER) *)bsearch_get_lte( \
		&bsearch->bsearch, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_GET_GT(TYPE, UPPER, LOWER, BODY) \
BSEARCH_ENTRY_TYPE(UPPER) *bsearch_##LOWER##_get_gt( \
	struct BSEARCH_TYPE(UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (BSEARCH_ENTRY_TYPE(UPPER) *)bsearch_get_gt( \
		&bsearch->bsearch, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_GET_LT(TYPE, UPPER, LOWER, BODY) \
BSEARCH_ENTRY_TYPE(UPPER) *bsearch_##LOWER##_get_lt( \
	struct BSEARCH_TYPE(UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return (BSEARCH_ENTRY_TYPE(UPPER) *)bsearch_get_lt( \
		&bsearch->bsearch, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_DELETE(TYPE, UPPER, LOWER, BODY) \
int bsearch_##LOWER##_delete( \
	struct BSEARCH_TYPE(UPPER) *bsearch, \
	unsigned char key \
) BODY({ \
	return bsearch_delete( \
		&bsearch->bsearch, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_CURSOR_INIT(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_init( \
	struct BSEARCH_CURSOR_TYPE(UPPER) *cursor, \
	struct BSEARCH_TYPE(UPPER) *bsearch \
) BODY({ \
	bsearch_cursor_init( \
		&cursor->cursor, \
		&bsearch->bsearch, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)) \
	); \
})

#define BSEARCH_CURSOR_REVERT(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_revert( \
	struct BSEARCH_CURSOR_TYPE(UPPER) *cursor \
) BODY({ \
	bsearch_cursor_revert(&cursor->cursor); \
})

#define BSEARCH_CURSOR_DISPOSE(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_dispose( \
	struct BSEARCH_CURSOR_TYPE(UPPER) *cursor \
) BODY({ \
	bsearch_cursor_dispose(&cursor->cursor); \
})

#define BSEARCH_CURSOR_NEXT(TYPE, UPPER, LOWER, BODY) \
bool bsearch_cursor_##LOWER##_next( \
	struct BSEARCH_CURSOR_TYPE(UPPER) *cursor \
) BODY({ \
	return bsearch_cursor_next(&cursor->cursor); \
})

#define BSEARCH_CURSOR_MOVE(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_move( \
	struct BSEARCH_CURSOR_TYPE(UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bsearch_cursor_move( \
		&cursor->cursor, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_CURSOR_MOVE_LT(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_move_lt( \
	struct BSEARCH_CURSOR_TYPE(UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bsearch_cursor_move_lt( \
		&cursor->cursor, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_CURSOR_MOVE_GT(TYPE, UPPER, LOWER, BODY) \
void bsearch_cursor_##LOWER##_move_gt( \
	struct BSEARCH_CURSOR_TYPE(UPPER) *cursor, \
	unsigned char key \
) BODY({ \
	bsearch_cursor_move_gt( \
		&cursor->cursor, \
		sizeof(struct BSEARCH_ENTRY_TYPE(UPPER)), \
		key \
	); \
})

#define BSEARCH_CURSOR_CURRENT(TYPE, UPPER, LOWER, BODY) \
BSEARCH_ENTRY_TYPE(UPPER) *bsearch_cursor_##LOWER##_current( \
	struct BSEARCH_CURSOR_TYPE(UPPER) *cursor \
) BODY({ \
	return (BSEARCH_ENTRY_TYPE(UPPER) *)bsearch_cursor_current( \
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


