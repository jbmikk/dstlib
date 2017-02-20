#ifndef CSTRUCT
#define CSTRUCT

typedef struct _Node {
	//Bsearch
	struct _Node *child;
	unsigned char child_count;
	unsigned char key;

	//Radix tree specific
	unsigned short size;
	unsigned char *array;
	void *data;
} Node;

typedef struct _Iterator {
	Node *root;
	unsigned char *key;
	unsigned short size;
	void *data;
} Iterator;

#endif // CSTRUCT
