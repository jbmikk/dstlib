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

void bsearch_iterator_init(BsearchIterator *it, Bsearch *bsearch, bool inverse);
void bsearch_iterator_revert(BsearchIterator *it);
void bsearch_iterator_dispose(BsearchIterator *it);
bool bsearch_iterator_next(BsearchIterator *it);
BsearchEntry *bsearch_iterator_current(BsearchIterator *it);

#endif	//BSEARCH_H


