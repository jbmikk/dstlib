#ifndef CSTRUCT
#define CSTRUCT

struct BsearchEntry;

typedef struct Bsearch {
	struct BsearchEntry *entries;
	unsigned char count;
} Bsearch;

typedef struct Node {
	Bsearch children;

	//Radix tree specific
	unsigned short size;
	unsigned char *array;
	void *data;
} Node;

struct BsearchEntry {
	unsigned char key;
	//TODO: remove dependency to Node type.
	struct Node node;
};

typedef struct BsearchEntry BsearchEntry;

typedef struct BsearchIterator {
	Bsearch *bsearch;
	BsearchEntry *current;
	BsearchEntry *last;
	int step;
} BsearchIterator;

typedef struct _Iterator {
	Node *root;
	unsigned char *key;
	unsigned short size;
	void *data;
} Iterator;

#endif // CSTRUCT
