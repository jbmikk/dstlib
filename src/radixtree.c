#include "radixtree.h"
#include "radixtree_p.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "cmemory.h"
#include "bsearch.h"
#include "arrays.h"

#ifdef RADIXTREE_TRACE
#define trace(M, ...) fprintf(stderr, "RADIXTREE: " M "\n", ##__VA_ARGS__)
#define trace_node(M, NODE) \
	trace( \
		"" M "(%p) [%c:%.*s], DATA:%p, CHILDREN(%i):%p", \
		(NODE), \
		(NODE)->key, \
		(NODE)->size, \
		(NODE)->array, \
		(NODE)->data, \
		(NODE)->child_count, \
		(NODE)->child \
	);
#else
#define trace(M, ...)
#define trace_node(M, NODE)
#endif

static void _scan_status_init(ScanStatus *status, unsigned char *key, unsigned short size)
{
	status->index = 0;
	status->subindex = 0;
	status->found = 0;
	status->key = key;
	status->size = size;
	status->type = S_DEFAULT;
}

static void _node_init(Node *node, unsigned char count, Node *child, void *data)
{
	node->child_count = count;
	node->child = child;
	node->data = data;
}

static void _node_set_array(Node *node, unsigned short size, unsigned char *array)
{
	node->size = size;
	node->array = array;
}

void radix_tree_init(Node *tree)
{
	_node_init(tree, 0, NULL, NULL);
	_node_set_array(tree, 0, NULL);
}

/**
 * Seek next node in the tree matching the current scan status
 */
static Node *_tree_seek_step(Node *tree, ScanStatus *status)
{
	Node *current = tree;

	if (current->child) {
		//Move to the next node within the tree
		char *key = (char *)status->key;
		Node *next = bsearch_get(current, key[status->index]);
		//Break if there is no node to move to
		if(next == NULL) {
			goto NOTFOUND;
		}
		current = next;
		status->index++;

		if (current->size > 0) {
			//Match array as far a possible
			unsigned char *array = current->array;
			int array_size = current->size;
			int j;
			unsigned int i = status->index;
			trace_node("SEEK-ARRAY", current);
			for (j = 0; j < array_size && i < status->size; j++, i++) {
				//Break if a character does not match
				trace("[%c-%c]", array[j], key[i]);
				if(array[j] != key[i]) {
					status->subindex = j;
					status->index = i;
					goto NOTFOUND;
				}
			}
			status->subindex = j;
			status->index = i;

			//Break if it didn't match the whole array
			if(j < array_size) {
				goto NOTFOUND;
			}
		}

	} else {
		trace_node("SEEK-LEAF", current);
		//Leaf node
		goto NOTFOUND;
	}

	status->found = status->index == status->size;
	return current;
NOTFOUND:
	status->found = -1;
	return current;
}

/**
 * Seek the node for the given key either for setting or getting a value
 * If the key is not found it returns the closest matching node.
 */
static Node *_tree_seek(Node *tree, ScanStatus *status)
{
	Node *current = tree;

	while(!status->found) {
		current = _tree_seek_step(current, status);
	}
	trace("SEEK-STATUS: %i", status->found);
	return current;
}

void scan_metadata_init(ScanMetadata *meta, Node *root) 
{
	meta->root = root;
	meta->previous = NULL;
}

void scan_metadata_push(ScanMetadata *meta, Node *node) 
{
	meta->previous = node;
}

/**
 * Same as seek, but return tree pointers necessary for removal
 */
static Node *_seek_metadata(Node *tree, ScanStatus *status, ScanMetadata *meta)
{
	Node *current = tree;

	scan_metadata_init(meta, tree);

	while(!status->found) {
		scan_metadata_push(meta, current);

		current = _tree_seek_step(current, status);
	}
	return current;
}

/**
 * Scan the tree recursively for the next datanode.
 */
static Node *_tree_scan(Node *node, ScanStatus *status, ScanStatus *post)
{
	char *key = (char *)status->key;
	Node *result = NULL;

	if (node->data) {
		if(status->type == S_DEFAULT) {
			// Only return data when status is default
			post->size = status->index;
			result = node;
			goto DATA_FOUND;
		} else if(status->index >= status->size) {
			// In FETCHNEXT mode we wait until the whole key was
			// matched, only then we scan for the next data node
			status->type = S_DEFAULT;
		}
	}
	
	if (node->child) {
		Node *children = node->child;
		Node *current = NULL;
		unsigned int i = 0;

		if (status->type == S_FETCHNEXT && status->size > 0) {
			Node *next = bsearch_get(node, key[status->index]);
			i = (next-children);
		} 

		for(; i < node->child_count && result == NULL; i++) {
			current = children+i;

			unsigned int offset = status->index;
			status->index += 1 + current->size;

			post->key = c_renew(post->key, unsigned char, status->index);

			((char *)post->key)[offset] = current->key;

			if (current->size) {
				memcpy(
					post->key + offset + 1,
					current->array,
					current->size
				);
			}

			result = _tree_scan(current, status, post);
			status->index -= 1 + current->size;

			if(result != NULL)
				break;
		}
	} 
DATA_FOUND:
	return result;
}

/**
 * Add new child node after given node
 */
static Node *_build_node(Node *node, unsigned char *string, unsigned int length)
{
	if (length >= 1) {
		node = bsearch_insert(node, string[0]);

		_node_init(node, 0, NULL, NULL);

		if(length > 1) {
			unsigned char *keys = c_malloc_n(length-1);

			for(unsigned int i = 0; i < length-1; i++){
				keys[i] = string[i+1];
			}

			_node_set_array(node, length-1, keys);
		} else {
			_node_set_array(node, 0, NULL);
		}
		trace_node("BUILD-NODE", node);
	} else {
		trace_node("REUSE-NODE", node);
	}
	return node;
}

/**
 * create split array node in two using a tree node
 * TODO: Verify tests manage all cases.
 */
static Node * _split_node_array(Node *node, ScanStatus *status)
{
	unsigned subindex = status->subindex;

	Node *data_node;
	Node old = *node;

	unsigned char *old_suffix = old.array+subindex;
	unsigned char *new_suffix = status->key + status->index;
	unsigned int old_suffix_size = old.size - subindex;
	unsigned int new_suffix_size = status->size - status->index;

	if (new_suffix_size == 0) {
                //No new suffix, we add data to current node
		_node_init(node, 0, NULL, NULL);

		data_node = node;

                //After the data node we append the old suffix
		Node *branch = _build_node(node, old_suffix, old_suffix_size);
		_node_init(branch, old.child_count, old.child, old.data);
	} else {
		//make node point to new tree node
		_node_init(node, 0, NULL, NULL);

		//add branch to hold old suffix and delete old data
		Node *branch1 = _build_node(node, old_suffix, old_suffix_size);
		_node_init(branch1, old.child_count, old.child, old.data);
		trace_node("OLD-BRANCH", branch1);

		//add branch to hold new suffix and return new node
		Node *branch2 = _build_node(node, new_suffix, new_suffix_size);

		data_node = branch2;
	}

	//TODO: what if subindex == 0?
	unsigned char *prefix = c_renew(node->array, unsigned char, subindex);
	_node_set_array(node, subindex, prefix);
	trace_node("PREFIX", node);

	return data_node;
}

static void _compact_nodes(Node *node)
{
	if (node->child_count != 1 || node->data) {
		return;
	}

	Node *child = node->child;

	//Join arrays
	int joined_size = node->size + 1 + child->size;
	unsigned char *joined = c_renew(node->array, unsigned char, joined_size);

	int i = node->size;

	joined[i++] = child->key;

	int j = 0;
	for(j; j < child->size; j++) {
		joined[i+j] = child->array[j];
	}

	//Replace child
	if(child->array) {
		c_free(child->array);
		set_null(child->array);
	}

	Node cont = *child;

	bsearch_delete(node, child->key);

	trace("new size: %i", joined_size);

	_node_init(node, cont.child_count, cont.child, cont.data);
	_node_set_array(node, joined_size, joined);
}

/**
 * Remove dangling node and compact tree
 */
static void _pluck_node(Node *node, ScanStatus *status, ScanMetadata *meta)
{
	trace_node("CLEAN", node);
	Node *previous = meta->previous;

	if(node->child_count == 0 && previous) {
		trace_node("PREVIOUS", previous);

		if(node->array) {
			c_free(node->array);
			set_null(node->array);
		}

		bsearch_delete(previous, node->key);

		if(previous != meta->root) {
			_compact_nodes(previous);
		}

	} else {
		_compact_nodes(node);
	}
}

void *radix_tree_get(Node *tree, unsigned char *string, unsigned int length)
{
	trace("RADIXTREE-GET(%p)", tree);

	ScanStatus status;
	_scan_status_init(&status, string, length);

	Node * node = _tree_seek(tree, &status);

	if(status.found == 1) {
		trace("FOUND %p", node);
		return node->data;
	} else {
		trace("NOTFOUND");
		return NULL;
	}
}

static Node *_build_data_node(Node *tree, unsigned char *string, unsigned int length)
{
	Node *data_node;

	ScanStatus status;
	_scan_status_init(&status, string, length);

	Node * node = _tree_seek(tree, &status);

	if (status.found == 1) {
		data_node = node;
	} else if (node->array && status.subindex < node->size) {
		data_node = _split_node_array(node, &status);
	} else {
		data_node = _build_node(node, string+status.index, length-status.index);
	}
	return data_node;
}

void radix_tree_set(Node *tree, unsigned char *string, unsigned int length, void *data)
{
	trace("RADIXTREE-SET(%p)", tree);

	Node *data_node = _build_data_node(tree, string, length);
	data_node->data = data;
}

int radix_tree_contains(Node *tree, unsigned char *string, unsigned int length)
{
	trace("RADIXTREE-CONTAINS(%p)", tree);

	ScanStatus status;
	_scan_status_init(&status, string, length);

	Node * node = _tree_seek(tree, &status);

	if(status.found == 1) {
		trace("FOUND %p", node);
		return 1;
	} else {
		trace("NOTFOUND");
		return 0;
	}
}

void *radix_tree_try_set(Node *tree, unsigned char *string, unsigned int length, void *data)
{
	trace("RADIXTREE-TRY-SET(%p)", tree);

	Node *data_node = _build_data_node(tree, string, length);

	void *previous_data = data_node->data;
	if(!previous_data) {
		data_node->data = data;
	}
	return previous_data;
}

void radix_tree_remove(Node *tree, unsigned char *string, unsigned int length)
{
	trace("RADIXTREE-REMOVE(%p)", tree);

	ScanStatus status;
	_scan_status_init(&status, string, length);

	ScanMetadata meta;

	Node * node = _seek_metadata(tree, &status, &meta);

	trace_node("ROOT", tree);
	trace_node("NODE", node);
	trace("STATUS %i, %i, %p", status.index, length, node->data);
	if(status.found == 1 && node->data) {
		node->data = NULL;
		_pluck_node(node, &status, &meta);
	}
}

void init_status(ScanStatus *status, ScanStatus *poststatus, unsigned char *string, unsigned int length)
{
	status->index = 0;
	status->subindex = 0;
	status->found = 0;
	status->key = string;
	status->size = length;
	if(string != NULL)
		status->type = S_FETCHNEXT;
	else
		status->type = S_DEFAULT;

	poststatus->key = c_new(unsigned char, 1);
	poststatus->size = 0;
	poststatus->index = 0;
	poststatus->subindex = 0;
	poststatus->found = 0;
}

void **radix_tree_get_next(Node *tree, unsigned char *string, unsigned int length)
{
	Node *res;
	ScanStatus status;
	ScanStatus poststatus;

	init_status(&status, &poststatus, string, length);

	res = _tree_scan(tree, &status, &poststatus);

	c_delete(poststatus.key);

	if(res != NULL) {
		return res->data;
	} else {
		return NULL;
	}
}

void radix_tree_dispose(Node *tree)
{
	trace_node("DISPOSE", tree);
	if(tree->array) {
		c_free(tree->array);
		set_null(tree->array);
	}
	if(tree->child) {
		Node *child = tree->child;
		int i;
		for(i = 0; i < tree->child_count; i++) {
			radix_tree_dispose(child+i);
		}
		c_delete(tree->child);
		set_null(tree->child);
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
	c_delete(iterator->key);
}

void **radix_tree_iterator_next(Iterator *iterator)
{
	Node *res;
	ScanStatus status;
	ScanStatus poststatus;

	init_status(&status, &poststatus, iterator->key, iterator->size);

	res = _tree_scan(iterator->root, &status, &poststatus);

	if(iterator->key) {
		c_delete(iterator->key);
	}

	iterator->key = poststatus.key;
	iterator->size = poststatus.size;

	if(res != NULL) {
		return res->data;
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
	int_to_padded_array_le(buffer, ptr);
	return radix_tree_get(tree, buffer, sizeof(intptr_t));
}

void radix_tree_set_intptr(Node *tree, intptr_t ptr, void *data)
{
	unsigned char buffer[sizeof(intptr_t)];
	int_to_padded_array_le(buffer, ptr);
	radix_tree_set(tree, buffer, sizeof(intptr_t), data);
}

int radix_tree_contains_intptr(Node *tree, intptr_t ptr)
{
	unsigned char buffer[sizeof(intptr_t)];
	int_to_padded_array_le(buffer, ptr);
	return radix_tree_contains(tree, buffer, sizeof(intptr_t));
}

void *radix_tree_get_next_intptr(Node *tree, intptr_t ptr)
{
	unsigned char buffer[sizeof(intptr_t)];
	int_to_padded_array_le(buffer, ptr);
	return radix_tree_get_next(tree, buffer, sizeof(intptr_t));
}
