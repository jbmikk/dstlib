#ifndef CSTRUCT
#define CSTRUCT

typedef struct _Node {
    void *child;
    char type;
    unsigned char size;
    char key;
} Node;

typedef struct _DataNode {
    Node node;
    void *data;
} DataNode;

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