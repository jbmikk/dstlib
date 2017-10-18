#ifndef CSTRUCT
#define CSTRUCT

typedef struct Node {
	//Bsearch
	struct Node *child;
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
