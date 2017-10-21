#ifndef BSEARCH_H
#define	BSEARCH_H

#include "structs.h"

void bsearch_init(Bsearch *bsearch);
BsearchEntry *bsearch_get(Bsearch *bsearch, unsigned char key);
BsearchEntry *bsearch_insert(Bsearch *bsearch, unsigned char key);
BsearchEntry *bsearch_get_gte(Bsearch *bsearch, unsigned char key);
BsearchEntry *bsearch_get_lte(Bsearch *bsearch, unsigned char key);
int bsearch_delete(Bsearch *bsearch, unsigned char key);
void bsearch_delete_all(Bsearch *bsearch);

#endif	//BSEARCH_H


