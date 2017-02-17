#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "radixtree.h"
#include "radixtree_p.h"
#include "test.h"

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

	t_assert(tree->child_count == 1);
	t_assert(tree->child->array != NULL);
	t_assert(tree->child->size == 3);
	t_assert(!strcmp(out1, "BLUE"));
}

void test_radix_tree__set_and_contains(){
	char *in1="BLUE";
	int out1 = 0, out2 = 0;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("blue"), in1);
	out1 = radix_tree_contains(tree, nzs("blue"));
	out2 = radix_tree_contains(tree, nzs("green"));

	t_assert(tree->child_count == 1);
	t_assert(tree->child->array != NULL);
	t_assert(tree->child->size == 3);
	t_assert(out1 == 1);
	t_assert(out2 == 0);
}

void test_radix_tree__set_and_get_1key(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("b"), in1);
	out1 = radix_tree_get(tree, nzs("b"));

	t_assert(tree->child_count == 1);
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

	t_assert(tree->child_count == 0);
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

	t_assert(tree->child_count == 1);
	t_assert(tree->array == 0);
	t_assert(tree->size == 0);
	t_assert(out1 == NULL);
	t_assert(!strcmp(out2, "GREEN"));
}

void test_radix_tree__set_and_remove_4key(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set(tree, nzs("blue"), in1);
	radix_tree_remove(tree, nzs("blue"));
	out1= radix_tree_get(tree, nzs("blue"));

	t_assert(tree->child_count == 0);
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

	t_assert(tree->child_count == 1);
	t_assert(tree->child->array != NULL);
	t_assert(tree->child->size == 4);
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

	t_assert(tree->child_count == 1);
	t_assert(tree->child->array != NULL);
	t_assert(tree->child->size == 9);
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

	t_assert(tree->child_count == 1);
	t_assert(tree->child->array != NULL);
	t_assert(tree->child->size == 2);
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

	t_assert(tree->child_count == 1);
	t_assert(tree->child->array != NULL);
	t_assert(tree->child->size == 2);
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

	t_assert(tree->child_count == 1);
	t_assert(tree->child->array != NULL);
	t_assert(tree->child->size == 9);
	t_assert(!strcmp(out1, "LIGHTGREEN"));
	t_assert(out2 == NULL);
}

void test_radix_tree__set_long_key(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	int length = 1024;
	char key[length];
	for(int i = 0; i < length; i++) {
		key[i] = 'a';
	}

	radix_tree_set(tree, key, length, in1);
	out1 = radix_tree_get(tree, key, length);

	t_assert(tree->child_count == 1);
	t_assert(tree->child->array != NULL);
	t_assert(tree->child->size == 1023);
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

void test_radix_tree__set_and_get_int(){
	char *in1="BLUE", *out1;
	Node *tree = &fixture.tree;

	radix_tree_set_int(tree, 1000, in1);
	out1 = radix_tree_get_int(tree, 1000);

	t_assert(out1 != NULL);
	t_assert(!strcmp(out1, "BLUE"));
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
	t_test(test_radix_tree__get_next);
	t_test(test_radix_tree__set_and_get_int);
	return t_done();
}

