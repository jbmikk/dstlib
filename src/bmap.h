#ifndef BMap_H
#define	BMap_H

#include <stdbool.h>

#include "generics.h"

struct BMapEntry;

// Opaque type, never to be instanced.
typedef struct BMapEntry BMapEntry;

typedef struct BMap {
	struct BMapEntry *entries;
	//It should probably be a long to make sure
	unsigned int count;
} BMap;

typedef struct BMapCursor {
	BMap *bmap;
	BMapEntry *current;
	BMapEntry *last;
	int step;
} BMapCursor;


typedef struct BMapComparator {
	int (*compare)(const struct BMapComparator *c, const BMapEntry *e);
	union {
		char char_key;
		int int_key;
		long long_key;
		unsigned char uchar_key;
		unsigned int uint_key;
		unsigned long ulong_key;
	};
} BMapComparator;

#define TYPEMAP(_, JOIN, ...) \
	_(char, char, __VA_ARGS__) JOIN\
	_(int, int, __VA_ARGS__) JOIN\
	_(long, long, __VA_ARGS__) JOIN\
	_(unsigned char, uchar, __VA_ARGS__) JOIN\
	_(unsigned int, uint, __VA_ARGS__) JOIN\
	_(unsigned long, ulong, __VA_ARGS__)

#define COMMA ,

#define EXPAND_TYPE(FUNC, TYPE, ...) _Generic((TYPE), \
	TYPEMAP(FUNC, COMMA, __VA_ARGS__) \
)

#define _KEY_EXPANSION(TYPE, NAME, VAR) TYPE: VAR.NAME##_key = key

#define _COMPARATOR_EXPANSION(TYPE, NAME, VAR) TYPE: VAR.compare = NAME##_key_compare

#define COMPARATOR_INIT(CMP, KEY) \
	EXPAND_TYPE(_COMPARATOR_EXPANSION, KEY, CMP); \
	EXPAND_TYPE(_KEY_EXPANSION, KEY, CMP);



int char_key_compare(const BMapComparator *c, const BMapEntry *e);
int int_key_compare(const BMapComparator *c, const BMapEntry *e);
int long_key_compare(const BMapComparator *c, const BMapEntry *e);
int uchar_key_compare(const BMapComparator *c, const BMapEntry *e);
int uint_key_compare(const BMapComparator *c, const BMapEntry *e);
int ulong_key_compare(const BMapComparator *c, const BMapEntry *e);


void bmap_init(BMap *bmap);
void bmap_dispose(BMap *bmap);
unsigned int bmap_count(BMap *bmap);
BMapEntry *bmap_first(BMap *bmap);
BMapEntry *bmap_get(BMap *bmap, unsigned int size, BMapComparator *cmp);
BMapEntry *bmap_insert(BMap *bmap, unsigned int size, BMapComparator *cmp);
BMapEntry *bmap_m_append(BMap *bmap, unsigned int size, BMapComparator *cmp);
BMapEntry *bmap_m_prepend(BMap *bmap, unsigned int size, BMapComparator *cmp);
BMapEntry *bmap_get_gte(BMap *bmap, unsigned int size, BMapComparator *cmp);
BMapEntry *bmap_get_lte(BMap *bmap, unsigned int size, BMapComparator *cmp);
BMapEntry *bmap_get_gt(BMap *bmap, unsigned int size, BMapComparator *cmp);
BMapEntry *bmap_get_lt(BMap *bmap, unsigned int size, BMapComparator *cmp);
int bmap_delete(BMap *bmap, unsigned int size, BMapComparator *cmp);
void bmap_delete_all(BMap *bmap);

void bmap_cursor_init(BMapCursor *cur, BMap *bmap, unsigned int size);
void bmap_cursor_revert(BMapCursor *cur);
void bmap_cursor_dispose(BMapCursor *cur);
bool bmap_cursor_next(BMapCursor *cur);
void bmap_cursor_move(BMapCursor *cur, unsigned int size, BMapComparator *cmp);
void bmap_cursor_move_lt(BMapCursor *cur, unsigned int size, BMapComparator *cmp);
void bmap_cursor_move_gt(BMapCursor *cur, unsigned int size, BMapComparator *cmp);
BMapEntry *bmap_cursor_current(BMapCursor *cur);


#define BMapEntry_TYPEDEF(KTYPE, VTYPE, UPPER, LOWER) \
typedef struct S(BMapEntry, UPPER) { \
	KTYPE key; \
	VTYPE LOWER; \
} S(BMapEntry, UPPER);

#define BMap_TYPEDEF(KTYPE, VTYPE, UPPER, LOWER) \
typedef struct S(BMap, UPPER) { \
	BMap bmap; \
} S(BMap, UPPER);

#define BMapCursor_TYPEDEF(KTYPE, VTYPE, UPPER, LOWER) \
typedef struct S(BMapCursor, UPPER) { \
	BMapCursor cursor; \
} S(BMapCursor, UPPER);


#define BMap_INIT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
void bmap_##LOWER##_init(struct S(BMap, UPPER) *bmap) BODY({ \
	bmap_init(&bmap->bmap); \
})

#define BMap_DISPOSE(KTYPE, VTYPE, UPPER, LOWER, BODY) \
void bmap_##LOWER##_dispose(struct S(BMap, UPPER) *bmap) BODY({ \
	bmap_dispose(&bmap->bmap); \
})

#define BMap_INSERT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_insert( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key, \
	VTYPE LOWER \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	struct S(BMapEntry, UPPER) *entry = (struct S(BMapEntry, UPPER) *)bmap_insert( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
	if(entry) { \
		entry->key = key; \
		entry->LOWER = LOWER; \
	} \
	return entry; \
})

#define BMap_M_APPEND(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_m_append( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key, \
	VTYPE LOWER \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	struct S(BMapEntry, UPPER) *entry = (struct S(BMapEntry, UPPER) *)bmap_m_append( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
	if(entry) { \
		entry->key = key; \
		entry->LOWER = LOWER; \
	} \
	return entry; \
})

#define BMap_M_PREPEND(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_m_prepend( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key, \
	VTYPE LOWER \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	struct S(BMapEntry, UPPER) *entry = (struct S(BMapEntry, UPPER) *)bmap_m_prepend( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
	if(entry) { \
		entry->key = key; \
		entry->LOWER = LOWER; \
	} \
	return entry; \
})

#define BMap_COUNT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
unsigned int bmap_##LOWER##_count( \
	struct S(BMap, UPPER) *bmap \
) BODY({ \
	return bmap_count(&bmap->bmap); \
})

#define BMap_FIRST(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_first( \
	struct S(BMap, UPPER) *bmap \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_first(&bmap->bmap); \
})

#define BMap_GET(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	return (S(BMapEntry, UPPER) *)bmap_get( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMap_GET_GTE(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get_gte( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	return (S(BMapEntry, UPPER) *)bmap_get_gte( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMap_GET_LTE(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get_lte( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	return (S(BMapEntry, UPPER) *)bmap_get_lte( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMap_GET_GT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get_gt( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	return (S(BMapEntry, UPPER) *)bmap_get_gt( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMap_GET_LT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_##LOWER##_get_lt( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	return (S(BMapEntry, UPPER) *)bmap_get_lt( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMap_DELETE(KTYPE, VTYPE, UPPER, LOWER, BODY) \
int bmap_##LOWER##_delete( \
	struct S(BMap, UPPER) *bmap, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	return bmap_delete( \
		&bmap->bmap, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMapCursor_INIT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
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

#define BMapCursor_REVERT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_revert( \
	struct S(BMapCursor, UPPER) *cursor \
) BODY({ \
	bmap_cursor_revert(&cursor->cursor); \
})

#define BMapCursor_DISPOSE(KTYPE, VTYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_dispose( \
	struct S(BMapCursor, UPPER) *cursor \
) BODY({ \
	bmap_cursor_dispose(&cursor->cursor); \
})

#define BMapCursor_NEXT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
bool bmap_cursor_##LOWER##_next( \
	struct S(BMapCursor, UPPER) *cursor \
) BODY({ \
	return bmap_cursor_next(&cursor->cursor); \
})

#define BMapCursor_MOVE(KTYPE, VTYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_move( \
	struct S(BMapCursor, UPPER) *cursor, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	bmap_cursor_move( \
		&cursor->cursor, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMapCursor_MOVE_LT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_move_lt( \
	struct S(BMapCursor, UPPER) *cursor, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	bmap_cursor_move_lt( \
		&cursor->cursor, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMapCursor_MOVE_GT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
void bmap_cursor_##LOWER##_move_gt( \
	struct S(BMapCursor, UPPER) *cursor, \
	KTYPE key \
) BODY({ \
	BMapComparator cmp; \
	COMPARATOR_INIT(cmp, key); \
	bmap_cursor_move_gt( \
		&cursor->cursor, \
		sizeof(struct S(BMapEntry, UPPER)), \
		&cmp \
	); \
})

#define BMapCursor_CURRENT(KTYPE, VTYPE, UPPER, LOWER, BODY) \
S(BMapEntry, UPPER) *bmap_cursor_##LOWER##_current( \
	struct S(BMapCursor, UPPER) *cursor \
) BODY({ \
	return (S(BMapEntry, UPPER) *)bmap_cursor_current( \
		&cursor->cursor \
	); \
})


#define BMap_TYPE_LIST(_, ...) \
	_(BMapEntry, __VA_ARGS__) \
	_(BMap, __VA_ARGS__) \
	_(BMapCursor, __VA_ARGS__)

#define BMap_FUNCTION_LIST(_, ...) \
	_(BMap_INIT, __VA_ARGS__) \
	_(BMap_DISPOSE, __VA_ARGS__) \
	_(BMap_INSERT, __VA_ARGS__) \
	_(BMap_M_APPEND, __VA_ARGS__) \
	_(BMap_M_PREPEND, __VA_ARGS__) \
	_(BMap_COUNT, __VA_ARGS__) \
	_(BMap_FIRST, __VA_ARGS__) \
	_(BMap_GET, __VA_ARGS__) \
	_(BMap_GET_GTE, __VA_ARGS__) \
	_(BMap_GET_LTE, __VA_ARGS__) \
	_(BMap_GET_GT, __VA_ARGS__) \
	_(BMap_GET_LT, __VA_ARGS__) \
	_(BMap_DELETE, __VA_ARGS__) \
	_(BMapCursor_INIT, __VA_ARGS__) \
	_(BMapCursor_REVERT, __VA_ARGS__) \
	_(BMapCursor_DISPOSE, __VA_ARGS__) \
	_(BMapCursor_NEXT, __VA_ARGS__) \
	_(BMapCursor_MOVE, __VA_ARGS__) \
	_(BMapCursor_MOVE_LT, __VA_ARGS__) \
	_(BMapCursor_MOVE_GT, __VA_ARGS__) \
	_(BMapCursor_CURRENT, __VA_ARGS__)


#endif	//BMap_H


