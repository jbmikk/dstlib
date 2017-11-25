#ifndef BSEARCH_H
#define	BSEARCH_H

#include "structs.h"
#include <stdbool.h>

void bsearch_init(Bsearch *bsearch);
void bsearch_dispose(Bsearch *bsearch);
BsearchEntry *bsearch_get(Bsearch *bsearch, unsigned char key);
BsearchEntry *bsearch_insert(Bsearch *bsearch, unsigned char key);
BsearchEntry *bsearch_get_gte(Bsearch *bsearch, unsigned char key);
BsearchEntry *bsearch_get_lte(Bsearch *bsearch, unsigned char key);
int bsearch_delete(Bsearch *bsearch, unsigned char key);
void bsearch_delete_all(Bsearch *bsearch);

void bsearch_cursor_init(BsearchCursor *cur, Bsearch *bsearch);
void bsearch_cursor_revert(BsearchCursor *cur);
void bsearch_cursor_dispose(BsearchCursor *cur);
bool bsearch_cursor_next(BsearchCursor *cur);
BsearchEntry *bsearch_cursor_current(BsearchCursor *cur);

#endif	//BSEARCH_H


