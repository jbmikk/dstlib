#ifndef CSTRUCT
#define CSTRUCT

typedef struct _Node {
	struct _Node *child;
	char type;
	unsigned char size;
	unsigned char key;
	void *array;
	void *data;
} Node;

typedef struct _Iterator {
	Node *root;
	void *key;
	unsigned char size;
	void *data;
} Iterator;

typedef struct _ScanStatus {
	void *key;
	unsigned char size;
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
