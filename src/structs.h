#ifndef CSTRUCT
#define CSTRUCT

typedef struct _Node {
	struct _Node *child;
	unsigned char child_count;
	char key;
	char *array;
	unsigned short size;
	void *data;
} Node;

typedef struct _Iterator {
	Node *root;
	void *key;
	unsigned short size;
	void *data;
} Iterator;

typedef struct _ScanStatus {
	char *key;
	unsigned short size;
	unsigned int index;
	unsigned int subindex;
	unsigned int type;
	unsigned int found;
} ScanStatus;

typedef struct _SNode {
	void *data;
	struct _SNode *next;
} SNode;

#define nzs(S) (S), (strlen(S))

#endif // CSTRUCT
