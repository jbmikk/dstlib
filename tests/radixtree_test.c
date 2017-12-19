#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "radixtree.h"
#include "test.h"
#include "arrays.h"

#define nzs(S) ((unsigned char*)S), (strlen(S))

typedef struct {
	Node tree;
	char *str1;
	char *str2;
	char *str3;
	char *str4;
	char *str5;
}RadixTreeFixture;

RadixTreeFixture fixture;

void t_setup(){
	radix_tree_init(&fixture.tree);
}
void t_teardown(){
	radix_tree_dispose(&fixture.tree);
}

void test_radix_tree__set_and_get(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("blue"), in1);
	out1 = radix_tree_get(tree, nzs("blue"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(bmap_node_first(&tree->children)->node.array != NULL);
	t_assert(bmap_node_first(&tree->children)->node.size == 3);
	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "BLUE"));
}

void test_radix_tree__set_and_contains(){
	char *in1="BLUE";
	int out1 = 0, out2 = 0;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("blue"), in1);
	out1 = radix_tree_contains(tree, nzs("blue"));
	out2 = radix_tree_contains(tree, nzs("green"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(bmap_node_first(&tree->children)->node.array != NULL);
	t_assert(bmap_node_first(&tree->children)->node.size == 3);
	t_assert(out1 == 1);
	t_assert(out2 == 0);
}

void test_radix_tree__set_and_get_1key(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("b"), in1);
	out1 = radix_tree_get(tree, nzs("b"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(tree->array == NULL);
	t_assert(tree->size == 0);
	t_assert(!strcmp(out1, "BLUE"));
}

void test_radix_tree__set_and_remove_1key(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("b"), in1);
	radix_tree_remove(tree, nzs("b"));
	out1 = radix_tree_get(tree, nzs("b"));

	t_assert(bmap_node_count(&tree->children) == 0);
	t_assert(tree->array == NULL);
	t_assert(tree->size == 0);
	t_assert(out1 == NULL);
}

void test_radix_tree__set2_and_remove1_1key(){
	char *in1="BLUE", *in2="GREEN", *out1, *out2;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("b"), in1);
	radix_tree_set(tree, nzs("g"), in2);
	radix_tree_remove(tree, nzs("b"));
	out1 = radix_tree_get(tree, nzs("b"));
	out2 = radix_tree_get(tree, nzs("g"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(tree->array == 0);
	t_assert(tree->size == 0);
	t_assert(out1 == NULL);
	t_assert(out2 != NULL);
	t_assert(!strcmp(out2, "GREEN"));
}

void test_radix_tree__set_and_remove_4key(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("blue"), in1);
	radix_tree_remove(tree, nzs("blue"));
	out1= radix_tree_get(tree, nzs("blue"));

	t_assert(bmap_node_count(&tree->children) == 0);
	t_assert(tree->array == NULL);
	t_assert(tree->size == 0);
	t_assert(out1 == NULL);
}

void test_radix_tree__set2_and_remove1_4key(){
	char *in1="BLUE", *in2="GREEN", *out1, *out2;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("blue"), in1);
	radix_tree_set(tree, nzs("green"), in2);
	radix_tree_remove(tree, nzs("blue"));
	out1 = radix_tree_get(tree, nzs("blue"));
	out2 = radix_tree_get(tree, nzs("green"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(bmap_node_first(&tree->children)->node.array != NULL);
	t_assert(bmap_node_first(&tree->children)->node.size == 4);
	t_assert(out1 == NULL);
	t_assert(out2 != NULL);
	t_assert(!strcmp(out2, "GREEN"));
}

void test_radix_tree__set2_and_remove1_4key_with_parent_array(){
	char *in1="BLUE", *in2="GREEN", *out1, *out2;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("lightblue"), in1);
	radix_tree_set(tree, nzs("lightgreen"), in2);
	radix_tree_remove(tree, nzs("lightblue"));
	out1 = radix_tree_get(tree, nzs("lightblue"));
	out2 = radix_tree_get(tree, nzs("lightgreen"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(bmap_node_first(&tree->children)->node.array != NULL);
	t_assert(bmap_node_first(&tree->children)->node.size == 9);
	t_assert(out1 == NULL);
	t_assert(!strcmp(out2, "GREEN"));
}

void test_radix_tree__set2_and_remove1_4key_deep(){
	char *in1="BLUE", *in2="GREEN", *in3="LIG", *out1, *out2, *out6;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("lightblue"), in1);
	radix_tree_set(tree, nzs("lightgreen"), in2);
	radix_tree_set(tree, nzs("lig"), in3);
	radix_tree_remove(tree, nzs("lightblue"));
	out1 = radix_tree_get(tree, nzs("lightblue"));
	out2 = radix_tree_get(tree, nzs("lightgreen"));
	out6 = radix_tree_get(tree, nzs("lig"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(bmap_node_first(&tree->children)->node.array != NULL);
	t_assert(bmap_node_first(&tree->children)->node.size == 2);
	t_assert(out1== NULL);
	t_assert(!strcmp(out2, "GREEN"));
	t_assert(!strcmp(out6, "LIG"));
}

void test_radix_tree__set2_and_remove1_3key(){
	char *in1="BLUE", *in2="GREEN", *out1, *out2;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("lbl"), in1);
	radix_tree_set(tree, nzs("lgr"), in2);
	radix_tree_remove(tree, nzs("lbl"));
	out1 = radix_tree_get(tree, nzs("lbl"));
	out2 = radix_tree_get(tree, nzs("lgr"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(bmap_node_first(&tree->children)->node.array != NULL);
	t_assert(bmap_node_first(&tree->children)->node.size == 2);
	t_assert(out1 == NULL);
	t_assert(!strcmp(out2, "GREEN"));
}

void test_radix_tree__set_before_tree(){
	char *in1="BLUER", *in2="BLUEST", *in3="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("bluer"), in1);
	radix_tree_set(tree, nzs("bluest"), in2);
	radix_tree_set(tree, nzs("blue"), in3);

	out1 = radix_tree_get(tree, nzs("blue"));

	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "BLUE"));
}

void test_radix_tree__try_set(){
	char *in1="BLUE", *in2="GREEN", *out1, *out2;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("blue"), in1);
	out1 = radix_tree_try_set(tree, nzs("blue"), in1);
	out2 = radix_tree_try_set(tree, nzs("green"), in2);

	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "BLUE"));
	t_assert(out2 == NULL);
}

void test_radix_tree__try_set_at_split_array(){
	char *in1="BLUER", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("bluer"), in1);
	out1 = radix_tree_try_set(tree, nzs("blue"), NULL);

	t_assert(out1 == NULL);
}

void test_radix_tree__remove_non_leaf_key(){
	char *in1="LIGHTGREEN", *in2="GREEN", *out1, *out2;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("lightgreen"), in1);
	radix_tree_set(tree, nzs("light"), in2);
	radix_tree_remove(tree, nzs("light"));
	out1 = radix_tree_get(tree, nzs("lightgreen"));
	out2 = radix_tree_get(tree, nzs("light"));

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(bmap_node_first(&tree->children)->node.array != NULL);
	t_assert(bmap_node_first(&tree->children)->node.size == 9);
	t_assert(!strcmp(out1, "LIGHTGREEN"));
	t_assert(out2 == NULL);
}

void test_radix_tree__set_long_key(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	int length = 1024;
	unsigned char key[length];
	for(int i = 0; i < length; i++) {
		key[i] = 'a';
	}

	radix_tree_set(tree, key, length, in1);
	out1 = radix_tree_get(tree, key, length);

	t_assert(bmap_node_count(&tree->children) == 1);
	t_assert(bmap_node_first(&tree->children)->node.array != NULL);
	t_assert(bmap_node_first(&tree->children)->node.size == 1023);
	t_assert(!strcmp(out1, "BLUE"));
}

void test_radix_tree__non_clashing_keys(){
	char *in1="BLUE", *in2="GREEN", *out1, *out2;
	Node *tree = &fixture.tree;
	radix_tree_set(tree, nzs("blue"), in1);
	radix_tree_set(tree, nzs("green"), in2);
	out1 = radix_tree_get(tree, nzs("blue"));
	out2 = radix_tree_get(tree, nzs("green"));
	t_assert(out1 != NULL);
	t_assert(out2 != NULL);
	t_assert(!strcmp(out1, "BLUE"));
	t_assert(!strcmp(out2, "GREEN"));
}

void test_radix_tree__first_clashing_keys(){
	char *in1="BLUE", *in2="BOBO", *out1, *out2;
	Node *tree = &fixture.tree;
	radix_tree_set(tree, nzs("blue"), in1);
	radix_tree_set(tree, nzs("bobo"), in2);
	out1 = radix_tree_get(tree, nzs("blue"));
	out2 = radix_tree_get(tree, nzs("bobo"));
	t_assert(out1 != NULL);
	t_assert(out2 != NULL);
	t_assert(!strcmp(out1, "BLUE"));
	t_assert(!strcmp(out2, "BOBO"));
}

void test_radix_tree__last_clashing_keys(){
	char *in1="BOBI", *in2="BOBO", *out1, *out2;
	Node *tree = &fixture.tree;
	radix_tree_set(tree, nzs("bobi"), in1);
	radix_tree_set(tree, nzs("bobo"), in2);
	out1 = radix_tree_get(tree, nzs("bobi"));
	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "BOBI"));
	out2 = radix_tree_get(tree, nzs("bobo"));
	t_assert(out2 != NULL);
	t_assert(!strcmp(out2, "BOBO"));
}

void test_radix_tree__add_prefix(){
	char *in1="DINOSAURIO", *in2="DINO", *out1, *out2;
	Node *tree = &fixture.tree;
	radix_tree_set(tree, nzs("dinosaurio"), in1);
	radix_tree_set(tree, nzs("dino"), in2);
	out1 = radix_tree_get(tree, nzs("dinosaurio"));
	out2 = radix_tree_get(tree, nzs("dino"));
	t_assert(out1 != NULL);
	t_assert(out2 != NULL);
	t_assert(!strcmp(out1, "DINOSAURIO"));
	t_assert(!strcmp(out2, "DINO"));
}

void test_radix_tree__add_suffix(){
	char *in1="DINOSAURIO", *in2="DINO", *out1, *out2;
	Node *tree = &fixture.tree;
	radix_tree_set(tree, nzs("dino"), in2);
	radix_tree_set(tree, nzs("dinosaurio"), in1);
	out1 = radix_tree_get(tree, nzs("dinosaurio"));
	out2 = radix_tree_get(tree, nzs("dino"));
	t_assert(out1 != NULL);
	t_assert(out2 != NULL);
	t_assert(!strcmp(out1, "DINOSAURIO"));
	t_assert(!strcmp(out2, "DINO"));
}

void test_radix_tree__iterate(){
	char *in1="DINOSAURIO", *in2="DINO", *in3="CASA", *in4="PIANO";
	char *out1, *out2, *out3, *out4, *out5;
	Node *tree = &fixture.tree;
	Iterator it;
	radix_tree_set(tree, nzs("dino"), in2);
	radix_tree_set(tree, nzs("piano"), in4);
	radix_tree_set(tree, nzs("dinosaurio"), in1);
	radix_tree_set(tree, nzs("casa"), in3);
	radix_tree_iterator_init(&it, tree);
	out1 = (char *)radix_tree_iterator_next(&it);
	out2 = (char *)radix_tree_iterator_next(&it);
	out3 = (char *)radix_tree_iterator_next(&it);
	out4 = (char *)radix_tree_iterator_next(&it);
	out5 = (char *)radix_tree_iterator_next(&it);
	radix_tree_iterator_dispose(&it);
	t_assert(out1 != NULL);
	t_assert(out2 != NULL);
	t_assert(out3 != NULL);
	t_assert(out4 != NULL);
	t_assert(!strcmp(out1, "CASA"));
	t_assert(!strcmp(out2, "DINO"));
	t_assert(!strcmp(out3, "DINOSAURIO"));
	t_assert(!strcmp(out4, "PIANO"));
	t_assert(out5 == NULL);
}

void test_radix_tree__iterate_empty(){
	char *out1;
	Node *tree = &fixture.tree;
	Iterator it;
	radix_tree_iterator_init(&it, tree);
	out1 = (char *)radix_tree_iterator_next(&it);
	radix_tree_iterator_dispose(&it);
	t_assert(out1 == NULL);
}

void test_radix_tree__iterate_binary(){
	char *in1="DINOSAURIO", *in2="DINO", *in3="CASA";
	char *out1, *out2, *out3, *out4;
	Node *tree = &fixture.tree;
	Iterator it;

	unsigned char buffer[sizeof(intptr_t)];
	unsigned int size;

	void *ptr1 = (void*)0xa927d0;
	void *ptr2 = (void*)0xa92870;
	void *ptr3 = (void*)0xa92910;

	int_to_array(buffer, &size, (intptr_t)ptr1);
	radix_tree_set(tree, buffer, size, in1);
	int_to_array(buffer, &size, (intptr_t)ptr2);
	radix_tree_set(tree, buffer, size, in2);
	int_to_array(buffer, &size, (intptr_t)ptr3);
	radix_tree_set(tree, buffer, size, in3);

	radix_tree_iterator_init(&it, tree);
	out1 = (char *)radix_tree_iterator_next(&it);
	out2 = (char *)radix_tree_iterator_next(&it);
	out3 = (char *)radix_tree_iterator_next(&it);
	out4 = (char *)radix_tree_iterator_next(&it);
	radix_tree_iterator_dispose(&it);

	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "CASA"));
	t_assert(out2 != NULL);
	t_assert(!strcmp(out2, "DINO"));
	t_assert(out3 != NULL);
	t_assert(!strcmp(out3, "DINOSAURIO"));
	t_assert(out4 == NULL);
}

void test_radix_tree__get_next(){
	char *in1="DINOSAURIO", *in2="DINO", *in3="CASA", *in4="PIANO";
	char *out1, *out2, *out3, *out4, *out5;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("dinosaurio"), in1);
	radix_tree_set(tree, nzs("dino"), in2);
	radix_tree_set(tree, nzs("casa"), in3);
	radix_tree_set(tree, nzs("piano"), in4);

	out1 = (char *)radix_tree_get_next(tree, NULL, 0);
	out2 = (char *)radix_tree_get_next(tree, nzs("dinosaurio"));
	out3 = (char *)radix_tree_get_next(tree, nzs("dino"));
	out4 = (char *)radix_tree_get_next(tree, nzs("casa"));
	out5 = (char *)radix_tree_get_next(tree, nzs("piano"));

	t_assert(!strcmp(out1, "CASA"));
	t_assert(!strcmp(out2, "PIANO"));
	t_assert(!strcmp(out3, "DINOSAURIO"));
	t_assert(!strcmp(out4, "DINO"));
	t_assert(out5 == NULL);
}

void test_radix_tree__get_next_full_key_scan(){
	char *in1="IN1", *in2="IN2", *in3="IN3",
	     *in4="IN4", *in5="IN5", *in6="IN6";
	char *out1, *out2, *out3, *out4, *out5,
	     *out6, *out7, *out8, *out9, *out10, *out11;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("accca"), in1);
	radix_tree_set(tree, nzs("acccb"), in2);
	radix_tree_set(tree, nzs("acccc"), in3);
	radix_tree_set(tree, nzs("accccaaa"), in4);
	radix_tree_set(tree, nzs("accccbbb"), in5);
	radix_tree_set(tree, nzs("acccce"), in6);

	out1 = (char *)radix_tree_get_next(tree, nzs("ammmb"));
	out2 = (char *)radix_tree_get_next(tree, nzs("acccb"));
	out3 = (char *)radix_tree_get_next(tree, nzs("aaaab"));
	out4 = (char *)radix_tree_get_next(tree, nzs("acccc"));
	out5 = (char *)radix_tree_get_next(tree, nzs("acccca"));
	out6 = (char *)radix_tree_get_next(tree, nzs("accccb"));
	out7 = (char *)radix_tree_get_next(tree, nzs("accccbd"));
	out8 = (char *)radix_tree_get_next(tree, nzs("accccd"));
	out9 = (char *)radix_tree_get_next(tree, nzs("accccf"));
	out10 = (char *)radix_tree_get_next(tree, nzs("az"));
	out11 = (char *)radix_tree_get_next(tree, nzs("acccbbbbb"));

	t_assert(out1 == NULL);
	t_assert(out2 != NULL);
	t_assert(!strcmp(out2, "IN3"));
	t_assert(out3 != NULL);
	t_assert(!strcmp(out3, "IN1"));
	t_assert(out4 != NULL);
	t_assert(!strcmp(out4, "IN4"));
	t_assert(out5 != NULL);
	t_assert(!strcmp(out5, "IN4"));
	t_assert(out6 != NULL);
	t_assert(!strcmp(out6, "IN5"));
	t_assert(out7 != NULL);
	t_assert(!strcmp(out7, "IN6"));
	t_assert(out8 != NULL);
	t_assert(!strcmp(out8, "IN6"));
	t_assert(out9 == NULL);
	t_assert(out10 == NULL);
	t_assert(out11 != NULL);
	t_assert(!strcmp(out11, "IN3"));
}

void test_radix_tree__get_prev(){
	char *in1="DINOSAURIO", *in2="DINO", *in3="CASA", *in4="PIANO";
	char *out1, *out2, *out3, *out4, *out5;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("dinosaurio"), in1);
	radix_tree_set(tree, nzs("dino"), in2);
	radix_tree_set(tree, nzs("casa"), in3);
	radix_tree_set(tree, nzs("piano"), in4);

	out1 = (char *)radix_tree_get_prev(tree, NULL, 0);
	out2 = (char *)radix_tree_get_prev(tree, nzs("dinosaurio"));
	out3 = (char *)radix_tree_get_prev(tree, nzs("dino"));
	out4 = (char *)radix_tree_get_prev(tree, nzs("casa"));
	out5 = (char *)radix_tree_get_prev(tree, nzs("piano"));

	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "PIANO"));
	t_assert(out2 != NULL);
	t_assert(!strcmp(out2, "DINO"));
	t_assert(out3 != NULL);
	t_assert(!strcmp(out3, "CASA"));
	t_assert(out4 == NULL);
	t_assert(out5 != NULL); 
	t_assert(!strcmp(out5, "DINOSAURIO"));
}

void test_radix_tree__get_prev_full_key_scan(){
	char *in1="IN1", *in2="IN2", *in3="IN3",
	     *in4="IN4", *in5="IN5", *in6="IN6";
	char *out1, *out2, *out3, *out4, *out5,
	     *out6, *out7, *out8, *out9, *out10, *out11;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("accca"), in1);
	radix_tree_set(tree, nzs("acccb"), in2);
	radix_tree_set(tree, nzs("acccc"), in3);
	radix_tree_set(tree, nzs("accccaaa"), in4);
	radix_tree_set(tree, nzs("accccbbb"), in5);
	radix_tree_set(tree, nzs("acccce"), in6);

	out1 = (char *)radix_tree_get_prev(tree, nzs("ammmb"));
	out2 = (char *)radix_tree_get_prev(tree, nzs("acccb"));
	out3 = (char *)radix_tree_get_prev(tree, nzs("aaaab"));
	out4 = (char *)radix_tree_get_prev(tree, nzs("acccc"));
	out5 = (char *)radix_tree_get_prev(tree, nzs("acccca"));
	out6 = (char *)radix_tree_get_prev(tree, nzs("accccb"));
	out7 = (char *)radix_tree_get_prev(tree, nzs("accccbd"));
	out8 = (char *)radix_tree_get_prev(tree, nzs("accccd"));
	out9 = (char *)radix_tree_get_prev(tree, nzs("accccf"));
	out10 = (char *)radix_tree_get_prev(tree, nzs("az"));
	out11 = (char *)radix_tree_get_prev(tree, nzs("acccbbbbb"));

	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "IN6"));
	t_assert(out2 != NULL);
	t_assert(!strcmp(out2, "IN1"));
	t_assert(out3 == NULL);
	t_assert(out4 != NULL);
	t_assert(!strcmp(out4, "IN2"));
	t_assert(out5 != NULL);
	t_assert(!strcmp(out5, "IN3"));
	t_assert(out6 != NULL);
	t_assert(!strcmp(out6, "IN4"));
	t_assert(out7 != NULL);
	t_assert(!strcmp(out7, "IN5"));
	t_assert(out8 != NULL);
	t_assert(!strcmp(out8, "IN5"));
	t_assert(out9 != NULL);
	t_assert(!strcmp(out9, "IN6"));
	t_assert(out10 != NULL);
	t_assert(!strcmp(out10, "IN6"));
	t_assert(out11 != NULL);
	t_assert(!strcmp(out11, "IN2"));
}
void test_radix_tree__set_and_get_int(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set_int(tree, 1000, in1);
	out1 = radix_tree_get_int(tree, 1000);

	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "BLUE"));
}

void test_radix_tree__set_and_get_intptr(){
	char *in1="BLUE", *out1, *ptr = NULL;
	Node *tree = &fixture.tree;

	radix_tree_set_intptr(tree, (intptr_t)&ptr, in1);
	out1 = radix_tree_get_intptr(tree, (intptr_t)&ptr);

	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "BLUE"));
}

void test_radix_tree__get_next_ple(){
	char *in1="A", *in2="B", *in3="C", *in4="D", *in5="E";
	char /* *out1, */ *out2, *out3, *out4, *out5, *out6;
	Node *tree = &fixture.tree;

	radix_tree_set_ple_int(tree, 20, in1);
	radix_tree_set_ple_int(tree, 40, in2);
	radix_tree_set_ple_int(tree, 128, in3);
	radix_tree_set_ple_int(tree, 255, in4);
	radix_tree_set_ple_int(tree, 256, in5);

	//out1 = (char *)radix_tree_get_next_ple_int(tree, 0);
	out2 = (char *)radix_tree_get_next_ple_int(tree, 20);
	out3 = (char *)radix_tree_get_next_ple_int(tree, 40);
	out4 = (char *)radix_tree_get_next_ple_int(tree, 128);
	out5 = (char *)radix_tree_get_next_ple_int(tree, 255);
	out6 = (char *)radix_tree_get_next_ple_int(tree, 256);

	//TODO: Should be able to get next key to any given value
	//t_assert(out1 != NULL);
	//t_assert(!strcmp(out1, "A"));
	t_assert(out2 != NULL);
	t_assert(!strcmp(out2, "B"));
	t_assert(out3 != NULL);
	t_assert(!strcmp(out3, "C"));
	t_assert(out4 != NULL);
	t_assert(!strcmp(out4, "D"));
	t_assert(out5 != NULL);
	t_assert(!strcmp(out5, "E"));
	t_assert(out6 == NULL);
}


int main(int argc, char** argv) {
	t_init();
	t_test(test_radix_tree__set_and_get);
	t_test(test_radix_tree__set_and_contains);
	t_test(test_radix_tree__set_and_get_1key);
	t_test(test_radix_tree__set_and_remove_1key);
	t_test(test_radix_tree__set2_and_remove1_1key);
	t_test(test_radix_tree__set_and_remove_4key);
	t_test(test_radix_tree__set2_and_remove1_4key);
	t_test(test_radix_tree__set2_and_remove1_4key_with_parent_array);
	t_test(test_radix_tree__set2_and_remove1_4key_deep);
	t_test(test_radix_tree__set2_and_remove1_3key);
	t_test(test_radix_tree__set_before_tree);
	t_test(test_radix_tree__try_set);
	t_test(test_radix_tree__try_set_at_split_array);
	t_test(test_radix_tree__remove_non_leaf_key);
	t_test(test_radix_tree__set_long_key);
	t_test(test_radix_tree__non_clashing_keys);
	t_test(test_radix_tree__first_clashing_keys);
	t_test(test_radix_tree__last_clashing_keys);
	t_test(test_radix_tree__add_prefix);
	t_test(test_radix_tree__add_suffix);
	t_test(test_radix_tree__iterate);
	t_test(test_radix_tree__iterate_empty);
	t_test(test_radix_tree__iterate_binary);
	t_test(test_radix_tree__get_next);
	t_test(test_radix_tree__get_next_full_key_scan);
	t_test(test_radix_tree__get_prev);
	t_test(test_radix_tree__get_prev_full_key_scan);
	t_test(test_radix_tree__set_and_get_int);
	t_test(test_radix_tree__set_and_get_intptr);
	t_test(test_radix_tree__get_next_ple);
	return t_done();
}

