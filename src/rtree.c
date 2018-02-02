#include "rtree.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmemory.h"
#include "bmap.h"
#include "arrays.h"


DEFINE_BMAP_FUNCTIONS(unsigned char, struct Node, Node, node, IMPLEMENTATION)

#ifdef RTREE_TRACE
// Fix (NODE)->key
#define trace(M, ...) fprintf(stderr, "RTREE: " M "\n", ##__VA_ARGS__)
#define trace_node(M, NODE) \
	trace( \
		"" M "(%p) [:%.*s], DATA:%p, CHILDREN(%i):%p", \
		(NODE), \
		(NODE)->size, \
		(NODE)->array, \
		(NODE)->data, \
		(NODE)->children.count, \
		(NODE)->children.entries \
	);
#else
#define trace(M, ...)
#define trace_node(M, NODE)
#endif


typedef struct _Scan {
	unsigned char *key;
	unsigned short size;
	unsigned char *pkey;
	unsigned short psize;
	unsigned int index;
	unsigned int subindex;
	unsigned int mode;
	unsigned int found;
	Node *root;
	Node *previous;
	Node *result;
} Scan;


static void _scan_init(Scan *scan, unsigned char *key, unsigned short size, Node *root, unsigned int mode)
{
	scan->index = 0;
	scan->subindex = 0;
	scan->found = 0;
	scan->key = key;
	scan->size = size;
	scan->pkey = NULL;
	scan->psize = 0;
	scan->mode = mode;
	scan->previous = NULL;
	scan->root = root;
	scan->result = NULL;
}

static void _node_init(Node *node, BMapNode children, void *data)
{
	node->children = children;
	node->data = data;
}

static void _node_array_init(Node *node)
{
	node->size = 0;
	node->array = NULL;
}

static void _node_set_array(Node *node, unsigned short size)
{
	node->size = size;
	node->array = realloc(node->array, sizeof(unsigned char) * size);
}

/**
 * Seek the node for the given key either for setting or getting a value
 * If the key is not found it returns the closest matching node.
 */
static Node *_tree_seek(Node *tree, Scan *scan)
{
	Node *current = tree;

	//Move to the next node within the tree
	BMapEntryNode *entry = bmap_node_get(&current->children, scan->key[scan->index]);
	//Break if there is no node to move to
	if(entry == NULL) {
		scan->found = -1;
		return current;
	}
	current = &entry->node;
	scan->index++;

	trace_node("SEEK-ARRAY", current);
	int j = 0;
	unsigned int i = scan->index;

	for (; j < current->size && i < scan->size; j++, i++) {
		//Break if a character does not match
		trace("[%c-%c]", current->array[j], scan->key[i]);
		if(current->array[j] != scan->key[i]) {
			scan->subindex = j;
			scan->index = i;
			scan->found = -1;
			return current;
		}
	}
	scan->subindex = j;
	scan->index = i;

	//Break if it didn't match the whole array
	if(j < current->size) {
		scan->found = -1;
		return current;
	}

	scan->previous = tree;

	if(scan->index == scan->size) {
		scan->found = 1;
		return current;
	}
	return _tree_seek(current, scan);
}

static void _push_node_key(Scan *scan, BMapEntryNode *entry)
{
	unsigned int offset = scan->index;
	scan->index += 1 + entry->node.size;
	scan->psize = scan->index;

	scan->pkey = realloc(scan->pkey, sizeof(unsigned char) * scan->psize);

	scan->pkey[offset] = entry->key;

	if (entry->node.size) {
		memcpy(scan->pkey + offset + 1, entry->node.array, entry->node.size);
	}
}

static void _pop_node_key(Scan *scan, BMapEntryNode *entry)
{
	scan->index -= 1 + entry->node.size;
	scan->psize = scan->index;
}

static int lmemcmp(unsigned char *a1, unsigned int l1, unsigned char *a2, unsigned int l2)
{
	unsigned int i = 0;
	for (; i < l1 && i < l2; i++) {
		if(a1[i] > a2[i]) {
			return 1;
		}
		if(a1[i] < a2[i]) {
			return -1;
		}
	}
	return i < l2? -1: 0;
}

/**
 * Scan the tree recursively for the next datanode.
 */
static void _tree_scan(Node *node, Scan *scan)
{
	bool has_next;
	BMapCursorNode cur;
	BMapEntryNode *entry;

	bmap_cursor_node_init(&cur, &node->children);

	switch(scan->mode) {
	case S_DEFAULT:
		if (node->data) {
			scan->result = node;
			return;
		}
		has_next = bmap_cursor_node_next(&cur);
		break;
	case S_FETCHNEXT:
		if(scan->index >= scan->size) {
			scan->mode = S_DEFAULT;
			has_next = bmap_cursor_node_next(&cur);
			goto CONTINUE;
		}

		bmap_cursor_node_move_lt(&cur, scan->key[scan->index]);

		has_next = bmap_cursor_node_next(&cur);
		if(!has_next) {
			scan->mode = S_DEFAULT;
			return;
		}
		entry = bmap_cursor_node_current(&cur);

		if(entry->key == scan->key[scan->index]) {

			int result = lmemcmp(
				scan->key + scan->index + 1,
				scan->size - (scan->index + 1),
				entry->node.array,
				entry->node.size
			);

			if(result != 0) {
				scan->mode = S_DEFAULT;
			}
			if(result > 0) {
				has_next = bmap_cursor_node_next(&cur);
			}

		} else {
			scan->mode = S_DEFAULT;
		}

		break;
	} 

CONTINUE:
	while(has_next) {
		BMapEntryNode *current = bmap_cursor_node_current(&cur);

		_push_node_key(scan, current);

		_tree_scan(&current->node, scan);

		if(scan->result != NULL)
			break;

		_pop_node_key(scan, current);
		has_next = bmap_cursor_node_next(&cur);
	}
	bmap_cursor_node_dispose(&cur);
}

/**
 * Scan the tree recursively for the previous datanode.
 */
static void _tree_scan_prev(Node *node, Scan *scan)
{
	bool has_next;
	BMapCursorNode cur;
	BMapEntryNode *next;

	bmap_cursor_node_init(&cur, &node->children);
	bmap_cursor_node_revert(&cur);

	if (scan->mode == S_FETCHNEXT) {
		if(scan->index >= scan->size) {
			scan->mode = S_DEFAULT;
			return;
		}

		bmap_cursor_node_move_gt(&cur, scan->key[scan->index]);

		has_next = bmap_cursor_node_next(&cur);
		if(!has_next) {
			scan->mode = S_DEFAULT;
			goto CONTINUE;
		}

		next = bmap_cursor_node_current(&cur);

		if(next->key == scan->key[scan->index]) {

			int result = lmemcmp(
				scan->key + scan->index + 1,
				scan->size - (scan->index + 1),
				next->node.array,
				next->node.size
			);

			if(result > 0) {
				scan->mode = S_DEFAULT;
				goto SKIP;
			}
			if(result != 0) {
				has_next = bmap_cursor_node_next(&cur);
				scan->mode = S_DEFAULT;
			}

		} else {
			scan->mode = S_DEFAULT;
		}
	} else {
		has_next = bmap_cursor_node_next(&cur);
	}

SKIP:
	if(bmap_node_count(&node->children) > 0) {
		while(has_next) {
			BMapEntryNode *current = bmap_cursor_node_current(&cur);

			_push_node_key(scan, current);

			_tree_scan_prev(&current->node, scan);

			if(scan->result != NULL)
				break;

			_pop_node_key(scan, current);
			has_next = bmap_cursor_node_next(&cur);
		}
		bmap_cursor_node_dispose(&cur);
	}
CONTINUE:
	if(scan->mode == S_DEFAULT && scan->result == NULL) {
		if (node->data) {
			scan->result = node;
			return;
		}
	}
}
/**
 * Add new child node after given node
 */
static Node *_build_node(Node *node, unsigned char *string, unsigned short length)
{
	Node *next_node;
	if (length >= 1) {
		Node new_node;

		_node_init(&new_node, (BMapNode){(BMap){NULL, 0}}, NULL);
		_node_array_init(&new_node);

		//TODO: If key length <= sizeof(ptr) don't malloc,
		// Just turn pointer into a union and store the array inline.
		if(length > 1) {
			_node_set_array(&new_node, length-1);
			memcpy(new_node.array, string+1, length-1);
		}
		next_node = &bmap_node_insert(&node->children, string[0], new_node)->node;
		trace_node("BUILD-NODE", next_node);
	} else {
		next_node = node;
		trace_node("REUSE-NODE", next_node);
	}
	return next_node;
}

/**
 * create split array node in two using a tree node
 * TODO: Verify tests manage all cases.
 */
static Node * _split_node_array(Node *node, Scan *scan)
{
	unsigned subindex = scan->subindex;

	Node *data_node;
	Node old = *node;

	unsigned char *old_suffix = old.array+subindex;
	unsigned char *new_suffix = scan->key + scan->index;
	unsigned int old_suffix_size = old.size - subindex;
	unsigned int new_suffix_size = scan->size - scan->index;

	if (new_suffix_size == 0) {
                //No new suffix, we add data to current node
		_node_init(node, (BMapNode){(BMap){NULL, 0}}, NULL);

		data_node = node;

                //After the data node we append the old suffix
		Node *branch = _build_node(node, old_suffix, old_suffix_size);
		_node_init(branch, old.children, old.data);
	} else {
		//make node point to new tree node
		_node_init(node, (BMapNode){(BMap){NULL, 0}}, NULL);

		//add branch to hold old suffix and delete old data
		Node *branch1 = _build_node(node, old_suffix, old_suffix_size);
		_node_init(branch1, old.children, old.data);
		trace_node("OLD-BRANCH", branch1);

		//add branch to hold new suffix and return new node
		Node *branch2 = _build_node(node, new_suffix, new_suffix_size);

		data_node = branch2;
	}

	_node_set_array(node, subindex);
	trace_node("PREFIX", node);

	return data_node;
}

static void _compact_nodes(Node *node)
{
	if (bmap_node_count(&node->children) != 1 || node->data) {
		return;
	}

	//TODO: Add bmap_get_first
	BMapEntryNode *child = (BMapEntryNode *)node->children.bmap.entries;

	//Join arrays
	int joined_size = node->size + 1 + child->node.size;
	int end = node->size;

	_node_set_array(node, joined_size);

	node->array[end++] = child->key;

	memcpy(node->array+end, child->node.array, child->node.size);

	//Replace child
	if(child->node.array) {
		free(child->node.array);
		set_null(child->node.array);
	}

	Node cont = child->node;

	bmap_node_delete(&node->children, child->key);

	trace("new size: %i", joined_size);

	_node_init(node, cont.children, cont.data);
}

/**
 * Remove dangling node and compact tree
 */
static void _pluck_node(Node *node, Scan *scan)
{
	Node *previous = scan->previous;
	trace_node("CLEAN", node);

	if(bmap_node_count(&node->children) == 0 && previous) {
		trace_node("PREVIOUS", previous);

		if(node->array) {
			free(node->array);
			set_null(node->array);
		}

		char bkey = scan->key[scan->index - scan->subindex - 1];
		bmap_node_delete(&previous->children, bkey);

		if(previous != scan->root) {
			_compact_nodes(previous);
		}

	} else {
		_compact_nodes(node);
	}
}

static Node *_build_data_node(Node *tree, unsigned char *string, unsigned short length)
{
	Node *data_node;

	Scan scan;
	_scan_init(&scan, string, length, tree, S_DEFAULT);

	Node * node = _tree_seek(tree, &scan);

	if (scan.found == 1) {
		data_node = node;
	} else if (node->array && scan.subindex < node->size) {
		data_node = _split_node_array(node, &scan);
	} else {
		data_node = _build_node(node, string+scan.index, length-scan.index);
	}
	return data_node;
}

static void _node_dispose(Node *node)
{
	trace_node("DISPOSE", node);
	if(node->array) {
		free(node->array);
		set_null(node->array);
	}
	BMapCursorNode cur;
	bmap_cursor_node_init(&cur, &node->children);
	while(bmap_cursor_node_next(&cur)) {
		BMapEntryNode *entry = bmap_cursor_node_current(&cur);
		_node_dispose(&entry->node);
	}
	bmap_cursor_node_dispose(&cur);
	bmap_node_dispose(&node->children);
}

void rtree_init(RTree *tree)
{
	_node_init(&tree->root, (BMapNode){(BMap){NULL, 0}}, NULL);
	_node_array_init(&tree->root);
}

void *rtree_get(RTree *tree, unsigned char *string, unsigned short length)
{
	trace("RTREE-GET(%p)", &tree->node);

	Scan scan;
	_scan_init(&scan, string, length, &tree->root, S_DEFAULT);

	Node * node = _tree_seek(&tree->root, &scan);

	if(scan.found == 1) {
		trace("FOUND %p", node);
		return node->data;
	} else {
		trace("NOTFOUND");
		return NULL;
	}
}

void rtree_set(RTree *tree, unsigned char *string, unsigned short length, void *data)
{
	trace("RTREE-SET(%p)", &tree->root);

	Node *data_node = _build_data_node(&tree->root, string, length);
	data_node->data = data;
}

int rtree_contains(RTree *tree, unsigned char *string, unsigned short length)
{
	trace("RTREE-CONTAINS(%p)", &tree->root);

	Scan scan;
	_scan_init(&scan, string, length, &tree->root, S_DEFAULT);

	_tree_seek(&tree->root, &scan);

	if(scan.found == 1) {
		trace("FOUND");
		return 1;
	} else {
		trace("NOTFOUND");
		return 0;
	}
}

void *rtree_try_set(RTree *tree, unsigned char *string, unsigned short length, void *data)
{
	trace("RTREE-TRY-SET(%p)", &tree->root);

	Node *data_node = _build_data_node(&tree->root, string, length);

	void *previous_data = data_node->data;
	if(!previous_data) {
		data_node->data = data;
	}
	return previous_data;
}

void rtree_remove(RTree *tree, unsigned char *string, unsigned short length)
{
	trace("RTREE-REMOVE(%p)", &tree->root);

	Scan scan;
	_scan_init(&scan, string, length, &tree->root, S_DEFAULT);

	Node * node = _tree_seek(&tree->root, &scan);

	trace_node("ROOT", &tree->root);
	trace_node("NODE", node);
	trace("STATUS %i, %i, %p", scan.index, length, node->data);
	if(scan.found == 1 && node->data) {
		node->data = NULL;
		_pluck_node(node, &scan);
	}
}

void **rtree_get_next(RTree *tree, unsigned char *string, unsigned short length)
{
	Scan scan;

	_scan_init(
		&scan,
		string,
		length,
		&tree->root,
		string? S_FETCHNEXT: S_DEFAULT
	);
	_tree_scan(&tree->root, &scan);

	free(scan.pkey);

	if(scan.result != NULL) {
		return scan.result->data;
	} else {
		return NULL;
	}
}

void **rtree_get_prev(RTree *tree, unsigned char *string, unsigned short length)
{
	Scan scan;

	_scan_init(
		&scan,
		string,
		length,
		&tree->root,
		string? S_FETCHNEXT: S_DEFAULT
	);
	_tree_scan_prev(&tree->root, &scan);

	free(scan.pkey);

	if(scan.result != NULL) {
		return scan.result->data;
	} else {
		return NULL;
	}
}

void rtree_dispose(RTree *tree)
{
	_node_dispose(&tree->root);
}

void rtree_iterator_init(Iterator *iterator, RTree *tree)
{
	iterator->root = &tree->root;
	iterator->key = NULL;
	iterator->size = 0;
	iterator->data= NULL;
}

void rtree_iterator_dispose(Iterator *iterator)
{
	free(iterator->key);
}

void **rtree_iterator_next(Iterator *iterator)
{
	Scan scan;

	_scan_init(
		&scan,
		iterator->key,
		iterator->size,
		NULL,
		iterator->key? S_FETCHNEXT: S_DEFAULT
	);

	_tree_scan(iterator->root, &scan);

	if(iterator->key) {
		free(iterator->key);
	}

	iterator->key = scan.pkey;
	iterator->size = scan.psize;

	if(scan.result != NULL) {
		return scan.result->data;
	} else {
		return NULL;
	}
}

void *rtree_get_int(RTree *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array(buffer, number);
	return rtree_get(tree, buffer, sizeof(int));
}

void rtree_set_int(RTree *tree, int number, void *data)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array(buffer, number);
	rtree_set(tree, buffer, sizeof(int), data);
}

int rtree_contains_int(RTree *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array(buffer, number);
	return rtree_contains(tree, buffer, sizeof(int));
}

void *rtree_get_next_int(RTree *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array(buffer, number);
	return rtree_get_next(tree, buffer, sizeof(int));
}

void *rtree_get_ple_int(RTree *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array_le(buffer, number);
	return rtree_get(tree, buffer, sizeof(int));
}

void rtree_set_ple_int(RTree *tree, int number, void *data)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array_le(buffer, number);
	rtree_set(tree, buffer, sizeof(int), data);
}

int rtree_contains_ple_int(RTree *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array_le(buffer, number);
	return rtree_contains(tree, buffer, sizeof(int));
}

void *rtree_get_next_ple_int(RTree *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array_le(buffer, number);
	return rtree_get_next(tree, buffer, sizeof(int));
}

void *rtree_get_intptr(RTree *tree, intptr_t ptr)
{
	unsigned char buffer[sizeof(intptr_t)];
	intptr_to_padded_array_le(buffer, ptr);
	return rtree_get(tree, buffer, sizeof(intptr_t));
}

void rtree_set_intptr(RTree *tree, intptr_t ptr, void *data)
{
	unsigned char buffer[sizeof(intptr_t)];
	intptr_to_padded_array_le(buffer, ptr);
	rtree_set(tree, buffer, sizeof(intptr_t), data);
}

int rtree_contains_intptr(RTree *tree, intptr_t ptr)
{
	unsigned char buffer[sizeof(intptr_t)];
	intptr_to_padded_array_le(buffer, ptr);
	return rtree_contains(tree, buffer, sizeof(intptr_t));
}

void *rtree_get_next_intptr(RTree *tree, intptr_t ptr)
{
	unsigned char buffer[sizeof(intptr_t)];
	intptr_to_padded_array_le(buffer, ptr);
	return rtree_get_next(tree, buffer, sizeof(intptr_t));
}
