#include "radixtree.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmemory.h"
#include "bsearch.h"
#include "arrays.h"

#ifdef RADIXTREE_TRACE
// Fix (NODE)->key
#define trace(M, ...) fprintf(stderr, "RADIXTREE: " M "\n", ##__VA_ARGS__)
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

static void _node_init(Node *node, Bsearch children, void *data)
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
	BsearchEntry *entry = bsearch_get(&current->children, scan->key[scan->index]);
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

static void _push_node_key(Scan *scan, BsearchEntry *entry)
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

static void _pop_node_key(Scan *scan, BsearchEntry *entry)
{
	scan->index -= 1 + entry->node.size;
	scan->psize = scan->index;
}

/**
 * Scan the tree recursively for the next datanode.
 */
static void _tree_scan(Node *node, Scan *scan)
{
	unsigned int i = 0;
	BsearchEntry *entry;

	switch(scan->mode) {
	case S_DEFAULT:
		if (node->data) {
			scan->result = node;
			return;
		}
		break;
	case S_FETCHNEXT:
		if(scan->index >= scan->size) {
			scan->mode = S_DEFAULT;
			goto CONTINUE;
		}

		entry = bsearch_get_gte(&node->children, scan->key[scan->index]);

		if(!entry) {
			scan->mode = S_DEFAULT;
			return;
		}

		if(entry->key == scan->key[scan->index]) {
			unsigned int j = 0;
			unsigned int k = scan->index+1; 
			unsigned int child_size = entry->node.size;
			for (; j < child_size && k < scan->size; j++, k++) {
				if(scan->key[k] > entry->node.array[j]) {
					entry++;
					break;
				} else if (scan->key[k] < entry->node.array[j]) {
					break;
				}
			}
			if(j < child_size) {
				scan->mode = S_DEFAULT;
			}
		} else {
			scan->mode = S_DEFAULT;
		}

		i = (entry - node->children.entries);
		break;
	} 

CONTINUE:
	for(; i < node->children.count; i++) {
		BsearchEntry *current = node->children.entries + i;

		_push_node_key(scan, current);

		_tree_scan(&current->node, scan);

		if(scan->result != NULL)
			break;

		_pop_node_key(scan, current);
	}
}

/**
 * Scan the tree recursively for the previous datanode.
 */
static void _tree_scan_prev(Node *node, Scan *scan)
{
	unsigned int i = node->children.count - 1;
	BsearchEntry *next;

	if (scan->mode == S_FETCHNEXT) {
		if(scan->index >= scan->size) {
			scan->mode = S_DEFAULT;
			return;
		}

		next = bsearch_get_lte(&node->children, scan->key[scan->index]);

		if(!next) {
			scan->mode = S_DEFAULT;
			goto CONTINUE;
		}

		if(next->key == scan->key[scan->index]) {
			unsigned int j = 0;
			unsigned int k = scan->index+1; 
			unsigned int child_size = next->node.size;
			for (; j < child_size && k < scan->size; j++, k++) {
				if(scan->key[k] < next->node.array[j]) {
					break;
				} else if (scan->key[k] > next->node.array[j]) {
					scan->mode = S_DEFAULT;
					goto SKIP;
				}
			}
			if(j < child_size) {
				next--;
				scan->mode = S_DEFAULT;
			}
		} else {
			scan->mode = S_DEFAULT;
		}

	SKIP:
		i = (next - node->children.entries);
	} 

	if(node->children.count > 0) {
		for(; i < node->children.count; i--) {
			BsearchEntry *current = node->children.entries + i;

			_push_node_key(scan, current);

			_tree_scan_prev(&current->node, scan);

			if(scan->result != NULL)
				break;

			_pop_node_key(scan, current);
		}
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
		next_node = &bsearch_insert(&node->children, string[0])->node;

		_node_init(next_node, (Bsearch){NULL, 0}, NULL);
		_node_array_init(next_node);

		//TODO: If key length <= sizeof(ptr) don't malloc,
		// Just turn pointer into a union and store the array inline.
		if(length > 1) {
			_node_set_array(next_node, length-1);
			memcpy(next_node->array, string+1, length-1);
		}
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
		_node_init(node, (Bsearch){NULL, 0}, NULL);

		data_node = node;

                //After the data node we append the old suffix
		Node *branch = _build_node(node, old_suffix, old_suffix_size);
		_node_init(branch, old.children, old.data);
	} else {
		//make node point to new tree node
		_node_init(node, (Bsearch){NULL, 0}, NULL);

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
	if (node->children.count != 1 || node->data) {
		return;
	}

	BsearchEntry *child = node->children.entries;

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

	bsearch_delete(&node->children, child->key);

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

	if(node->children.count == 0 && previous) {
		trace_node("PREVIOUS", previous);

		if(node->array) {
			free(node->array);
			set_null(node->array);
		}

		char bkey = scan->key[scan->index - scan->subindex - 1];
		bsearch_delete(&previous->children, bkey);

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

void radix_tree_init(Node *tree)
{
	_node_init(tree, (Bsearch){NULL, 0}, NULL);
	_node_array_init(tree);
}

void *radix_tree_get(Node *tree, unsigned char *string, unsigned short length)
{
	trace("RADIXTREE-GET(%p)", tree);

	Scan scan;
	_scan_init(&scan, string, length, tree, S_DEFAULT);

	Node * node = _tree_seek(tree, &scan);

	if(scan.found == 1) {
		trace("FOUND %p", node);
		return node->data;
	} else {
		trace("NOTFOUND");
		return NULL;
	}
}

void radix_tree_set(Node *tree, unsigned char *string, unsigned short length, void *data)
{
	trace("RADIXTREE-SET(%p)", tree);

	Node *data_node = _build_data_node(tree, string, length);
	data_node->data = data;
}

int radix_tree_contains(Node *tree, unsigned char *string, unsigned short length)
{
	trace("RADIXTREE-CONTAINS(%p)", tree);

	Scan scan;
	_scan_init(&scan, string, length, tree, S_DEFAULT);

	_tree_seek(tree, &scan);

	if(scan.found == 1) {
		trace("FOUND");
		return 1;
	} else {
		trace("NOTFOUND");
		return 0;
	}
}

void *radix_tree_try_set(Node *tree, unsigned char *string, unsigned short length, void *data)
{
	trace("RADIXTREE-TRY-SET(%p)", tree);

	Node *data_node = _build_data_node(tree, string, length);

	void *previous_data = data_node->data;
	if(!previous_data) {
		data_node->data = data;
	}
	return previous_data;
}

void radix_tree_remove(Node *tree, unsigned char *string, unsigned short length)
{
	trace("RADIXTREE-REMOVE(%p)", tree);

	Scan scan;
	_scan_init(&scan, string, length, tree, S_DEFAULT);

	Node * node = _tree_seek(tree, &scan);

	trace_node("ROOT", tree);
	trace_node("NODE", node);
	trace("STATUS %i, %i, %p", scan.index, length, node->data);
	if(scan.found == 1 && node->data) {
		node->data = NULL;
		_pluck_node(node, &scan);
	}
}

void **radix_tree_get_next(Node *tree, unsigned char *string, unsigned short length)
{
	Scan scan;

	_scan_init(
		&scan,
		string,
		length,
		tree,
		string? S_FETCHNEXT: S_DEFAULT
	);
	_tree_scan(tree, &scan);

	free(scan.pkey);

	if(scan.result != NULL) {
		return scan.result->data;
	} else {
		return NULL;
	}
}

void **radix_tree_get_prev(Node *tree, unsigned char *string, unsigned short length)
{
	Scan scan;

	_scan_init(
		&scan,
		string,
		length,
		tree,
		string? S_FETCHNEXT: S_DEFAULT
	);
	_tree_scan_prev(tree, &scan);

	free(scan.pkey);

	if(scan.result != NULL) {
		return scan.result->data;
	} else {
		return NULL;
	}
}

void radix_tree_dispose(Node *tree)
{
	trace_node("DISPOSE", tree);
	if(tree->array) {
		free(tree->array);
		set_null(tree->array);
	}
	if(tree->children.entries) {
		BsearchEntry *entry = tree->children.entries;
		int i;
		for(i = 0; i < tree->children.count; i++) {
			radix_tree_dispose(&(entry + i)->node);
		}
		bsearch_dispose(&tree->children);
	}
}

void radix_tree_iterator_init(Iterator *iterator, Node *tree)
{
	iterator->root = tree;
	iterator->key = NULL;
	iterator->size = 0;
	iterator->data= NULL;
}

void radix_tree_iterator_dispose(Iterator *iterator)
{
	free(iterator->key);
}

void **radix_tree_iterator_next(Iterator *iterator)
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

void *radix_tree_get_int(Node *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array(buffer, number);
	return radix_tree_get(tree, buffer, sizeof(int));
}

void radix_tree_set_int(Node *tree, int number, void *data)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array(buffer, number);
	radix_tree_set(tree, buffer, sizeof(int), data);
}

int radix_tree_contains_int(Node *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array(buffer, number);
	return radix_tree_contains(tree, buffer, sizeof(int));
}

void *radix_tree_get_next_int(Node *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array(buffer, number);
	return radix_tree_get_next(tree, buffer, sizeof(int));
}

void *radix_tree_get_ple_int(Node *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array_le(buffer, number);
	return radix_tree_get(tree, buffer, sizeof(int));
}

void radix_tree_set_ple_int(Node *tree, int number, void *data)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array_le(buffer, number);
	radix_tree_set(tree, buffer, sizeof(int), data);
}

int radix_tree_contains_ple_int(Node *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array_le(buffer, number);
	return radix_tree_contains(tree, buffer, sizeof(int));
}

void *radix_tree_get_next_ple_int(Node *tree, int number)
{
	unsigned char buffer[sizeof(int)];
	int_to_padded_array_le(buffer, number);
	return radix_tree_get_next(tree, buffer, sizeof(int));
}

void *radix_tree_get_intptr(Node *tree, intptr_t ptr)
{
	unsigned char buffer[sizeof(intptr_t)];
	intptr_to_padded_array_le(buffer, ptr);
	return radix_tree_get(tree, buffer, sizeof(intptr_t));
}

void radix_tree_set_intptr(Node *tree, intptr_t ptr, void *data)
{
	unsigned char buffer[sizeof(intptr_t)];
	intptr_to_padded_array_le(buffer, ptr);
	radix_tree_set(tree, buffer, sizeof(intptr_t), data);
}

int radix_tree_contains_intptr(Node *tree, intptr_t ptr)
{
	unsigned char buffer[sizeof(intptr_t)];
	intptr_to_padded_array_le(buffer, ptr);
	return radix_tree_contains(tree, buffer, sizeof(intptr_t));
}

void *radix_tree_get_next_intptr(Node *tree, intptr_t ptr)
{
	unsigned char buffer[sizeof(intptr_t)];
	intptr_to_padded_array_le(buffer, ptr);
	return radix_tree_get_next(tree, buffer, sizeof(intptr_t));
}
