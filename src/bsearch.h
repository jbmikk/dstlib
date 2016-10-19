#ifndef BSEARCH_H
#define	BSEARCH_H

#include "structs.h"

Node *bsearch_get(Node *parent, unsigned char key);
Node *bsearch_insert(Node *parent, unsigned char key);
int bsearch_delete(Node *parent, unsigned char key);
void bsearch_delete_all(Node *parent);

#endif	//BSEARCH_H


