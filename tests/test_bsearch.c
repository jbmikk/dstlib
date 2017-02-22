#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "bsearch.h"
#include "radixtree.h"

typedef struct {
	Node node;
	char *str1;
	char *str2;
	char *str3;
	char *str4;
	char *str5;
} BSearchFixture;

BSearchFixture fixture;

void t_setup(){
	bsearch_init(&fixture.node);
}
void t_teardown(){
	bsearch_delete_all(&fixture.node);
}

void bsearch__set_and_get(){
	Node *a1, *a2;
	a1 = bsearch_insert(&fixture.node, 'a');
	a2 = bsearch_get(&fixture.node, 'a');
	t_assert(a1 != NULL);
	t_assert(a2 != NULL);
	t_assert(a1 == a2);
}

void bsearch__set2_and_get2(){
	Node *a1, *a2, *b1, *b2;
	Node d1, d2;

	a1 = bsearch_insert(&fixture.node, 'a');
	a1->child = (void *) &d1;
	b1 = bsearch_insert(&fixture.node, 'b');
	b1->child = (void *) &d2;
	a2 = bsearch_get(&fixture.node, 'a');
	b2 = bsearch_get(&fixture.node, 'b');

	t_assert(fixture.node.child_count == 2);
	t_assert(a2 != NULL);
	t_assert(a2->child == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->child == (void *)&d2);
}

void bsearch__get_gte(){
	Node *in1, *in2, *out1, *out2, *out3, *out4, *out5;
	Node d1, d2;

	in1 = bsearch_insert(&fixture.node, 'b');
	in1->child = (void *) &d1;
	in2 = bsearch_insert(&fixture.node, 'd');
	in2->child = (void *) &d2;
	out1 = bsearch_get_gte(&fixture.node, 'b');
	out2 = bsearch_get_gte(&fixture.node, 'd');
	out3 = bsearch_get_gte(&fixture.node, 'a');
	out4 = bsearch_get_gte(&fixture.node, 'c');
	out5 = bsearch_get_gte(&fixture.node, 'e');

	t_assert(out1 != NULL);
	t_assert(out1->child == (void *)&d1);
	t_assert(out2 != NULL);
	t_assert(out2->child == (void *)&d2);
	t_assert(out3 != NULL);
	t_assert(out3->child == (void *)&d1);
	t_assert(out4 != NULL);
	t_assert(out4->child == (void *)&d2);
	t_assert(out5 == NULL);
}

void bsearch__set2_and_get2_ensure_unsigned(){

	bsearch_insert(&fixture.node, 10);
	bsearch_insert(&fixture.node, 128);

	t_assert(fixture.node.child_count == 2);
	t_assert(((Node*)fixture.node.child)->key == 10);
}

void bsearch__set2_and_delete1(){
	Node *a1, *a2, *b1, *b2;
	Node d1, d2;

	a1 = bsearch_insert(&fixture.node, 'a');
	a1->child = (void *) &d1;
	b1 = bsearch_insert(&fixture.node, 'b');
	b1->child = (void *) &d2;

	bsearch_delete(&fixture.node, 'a');

	a2 = bsearch_get(&fixture.node, 'a');
	b2 = bsearch_get(&fixture.node, 'b');

	t_assert(fixture.node.child_count == 1);
	t_assert(a2 == NULL);
	t_assert(b2 != NULL);
	t_assert(b2->child == (void *)&d2);
}

int main(int argc, char** argv) {
	t_init();
	t_test(bsearch__set_and_get);
	t_test(bsearch__set2_and_get2);
	t_test(bsearch__get_gte);
	t_test(bsearch__set2_and_get2_ensure_unsigned);
	t_test(bsearch__set2_and_delete1);

	//TODO:
	//delete
	//set_and_out_of_memory
	//delete_and_out_of_memory
	return t_done();
}

