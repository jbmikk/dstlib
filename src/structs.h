#ifndef CSTRUCT
#define CSTRUCT

typedef struct _Node {
	struct _Node *child;
	unsigned char child_count;
	unsigned char key;
	unsigned char *array;
	unsigned short size;
	void *data;
} Node;

typedef struct _Iterator {
	Node *root;
	unsigned char *key;
	unsigned short size;
	void *data;
} Iterator;

#endif // CSTRUCT
