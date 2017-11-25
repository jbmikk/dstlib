#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "bsearch.h"
#include "radixtree.h"

typedef struct {
	Bsearch bsearch;
	char *str1;
	char *str2;
	char *str3;
	char *str4;
	char *str5;
} BSearchFixture;

BSearchFixture fixture;

void t_setup(){
	bsearch_init(&fixture.bsearch);
}
void t_teardown(){
	bsearch_dispose(&fixture.bsearch);
}

void bsearch__set_and_get(){
	BsearchEntry *a1, *a2;
	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a2 = bsearch_get(&fixture.bsearch, 'a');
	t_assert(a1 != NULL);
	t_assert(a2 != NULL);
	t_assert(a1 == a2);
}

void bsearch__set2_and_get2(){
	BsearchEntry *a1, *a2, *b1, *b2;
	BsearchEntry d1, d2;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;
	b1 = bsearch_insert(&fixture.bsearch, 'b');
	b1->node.data = (void *) &d2;
	a2 = bsearch_get(&fixture.bsearch, 'a');
	b2 = bsearch_get(&fixture.bsearch, 'b');

	t_assert(fixture.bsearch.count == 2);
	t_assert(a2 != NULL);
	t_assert(a2->node.data == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->node.data == (void *)&d2);
}


void bsearch__not_set_same_key_twice()
{
	BsearchEntry *a1, *a2, *b1;
	BsearchEntry d1;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;
	b1 = bsearch_insert(&fixture.bsearch, 'a');
	a2 = bsearch_get(&fixture.bsearch, 'a');

	t_assert(fixture.bsearch.count == 1);
	t_assert(a1 != NULL);
	t_assert(b1 == NULL);
	t_assert(a2 != NULL);
	t_assert(a2->node.data == (void *)&d1);
}


void bsearch__set_third_in_the_middle()
{
	BsearchEntry *a1, *a2, *b1, *b2, *c1, *c2;
	BsearchEntry d1, d2, d3;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;
	c1 = bsearch_insert(&fixture.bsearch, 'c');
	c1->node.data = (void *) &d3;
	b1 = bsearch_insert(&fixture.bsearch, 'b');
	b1->node.data = (void *) &d2;

	a2 = bsearch_get(&fixture.bsearch, 'a');
	b2 = bsearch_get(&fixture.bsearch, 'b');
	c2 = bsearch_get(&fixture.bsearch, 'c');

	t_assert(fixture.bsearch.count == 3);

	t_assert(a2 != NULL);
	t_assert(a2->node.data == (void *)&d1);

	t_assert(b2 != NULL);
	t_assert(b2->node.data == (void *)&d2);

	t_assert(c2 != NULL);
	t_assert(c2->node.data == (void *)&d3);
}


void bsearch__get_gte(){
	BsearchEntry *in1, *in2, *out1, *out2, *out3, *out4, *out5;
	BsearchEntry d1, d2;

	in1 = bsearch_insert(&fixture.bsearch, 'b');
	in1->node.data = (void *) &d1;
	in2 = bsearch_insert(&fixture.bsearch, 'd');
	in2->node.data = (void *) &d2;
	out1 = bsearch_get_gte(&fixture.bsearch, 'b');
	out2 = bsearch_get_gte(&fixture.bsearch, 'd');
	out3 = bsearch_get_gte(&fixture.bsearch, 'a');
	out4 = bsearch_get_gte(&fixture.bsearch, 'c');
	out5 = bsearch_get_gte(&fixture.bsearch, 'e');

	t_assert(out1 != NULL);
	t_assert(out1->node.data == (void *)&d1);
	t_assert(out2 != NULL);
	t_assert(out2->node.data == (void *)&d2);
	t_assert(out3 != NULL);
	t_assert(out3->node.data == (void *)&d1);
	t_assert(out4 != NULL);
	t_assert(out4->node.data == (void *)&d2);
	t_assert(out5 == NULL);
}

void bsearch__get_lte(){
	BsearchEntry *in1, *in2, *out1, *out2, *out3, *out4, *out5;
	BsearchEntry d1, d2;

	in1 = bsearch_insert(&fixture.bsearch, 'b');
	in1->node.data = (void *) &d1;
	in2 = bsearch_insert(&fixture.bsearch, 'd');
	in2->node.data = (void *) &d2;
	out1 = bsearch_get_lte(&fixture.bsearch, 'b');
	out2 = bsearch_get_lte(&fixture.bsearch, 'd');
	out3 = bsearch_get_lte(&fixture.bsearch, 'a');
	out4 = bsearch_get_lte(&fixture.bsearch, 'c');
	out5 = bsearch_get_lte(&fixture.bsearch, 'e');

	t_assert(out1 != NULL);
	t_assert(out1->node.data == (void *)&d1);
	t_assert(out2 != NULL);
	t_assert(out2->node.data == (void *)&d2);
	t_assert(out3 == NULL);
	t_assert(out4 != NULL);
	t_assert(out4->node.data == (void *)&d1);
	t_assert(out5 != NULL);
	t_assert(out5->node.data == (void *)&d2);
}


void bsearch__set2_and_get2_ensure_unsigned(){

	bsearch_insert(&fixture.bsearch, 10);
	bsearch_insert(&fixture.bsearch, 128);

	t_assert(fixture.bsearch.count == 2);
	t_assert(((BsearchEntry*)fixture.bsearch.entries)->key == 10);
}

void bsearch__set2_and_delete1(){
	BsearchEntry *a1, *a2, *b1, *b2;
	BsearchEntry d1, d2;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;
	b1 = bsearch_insert(&fixture.bsearch, 'b');
	b1->node.data = (void *) &d2;

	bsearch_delete(&fixture.bsearch, 'a');

	a2 = bsearch_get(&fixture.bsearch, 'a');
	b2 = bsearch_get(&fixture.bsearch, 'b');

	t_assert(fixture.bsearch.count == 1);
	t_assert(a2 == NULL);
	t_assert(b2 != NULL);
	t_assert(b2->node.data == (void *)&d2);
}

void bsearch__set3_and_delete1(){
	BsearchEntry *a1, *a2, *b1, *b2, *c1, *c2;
	BsearchEntry d1, d2, d3;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;
	b1 = bsearch_insert(&fixture.bsearch, 'b');
	b1->node.data = (void *) &d2;
	c1 = bsearch_insert(&fixture.bsearch, 'c');
	c1->node.data = (void *) &d3;

	bsearch_delete(&fixture.bsearch, 'b');

	a2 = bsearch_get(&fixture.bsearch, 'a');
	b2 = bsearch_get(&fixture.bsearch, 'b');
	c2 = bsearch_get(&fixture.bsearch, 'c');

	t_assert(fixture.bsearch.count == 2);

	t_assert(a2 != NULL);
	t_assert(a2->node.data == (void *)&d1);

	t_assert(b2 == NULL);

	t_assert(c2 != NULL);
	t_assert(c2->node.data == (void *)&d3);
}

void bsearch_cursor__iterate_zero(){
	BsearchCursor cur;

	bsearch_cursor_init(&cur, &fixture.bsearch);
	bool first = bsearch_cursor_next(&cur);

	bsearch_cursor_dispose(&cur);

	t_assert(!first);
}

void bsearch_cursor__iterate_one(){
	BsearchEntry *a1, *b1;
	BsearchEntry d1;
	BsearchCursor cur;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;

	bsearch_cursor_init(&cur, &fixture.bsearch);
	bool first = bsearch_cursor_next(&cur);
	b1 = bsearch_cursor_current(&cur);
	bool second = bsearch_cursor_next(&cur);

	bsearch_cursor_dispose(&cur);

	t_assert(first);
	t_assert(!second);

	t_assert(b1 != NULL);
	t_assert(b1->node.data == (void *)&d1);
}

void bsearch_cursor__iterate_two(){
	BsearchEntry *a1, *a2, *b1, *b2;
	BsearchEntry d1, d2;
	BsearchCursor cur;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;
	a2 = bsearch_insert(&fixture.bsearch, 'b');
	a2->node.data = (void *) &d2;

	bsearch_cursor_init(&cur, &fixture.bsearch);
	bool first = bsearch_cursor_next(&cur);
	b1 = bsearch_cursor_current(&cur);
	bool second = bsearch_cursor_next(&cur);
	b2 = bsearch_cursor_current(&cur);
	bool third = bsearch_cursor_next(&cur);

	bsearch_cursor_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(!third);

	t_assert(b1 != NULL);
	t_assert(b1->node.data == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->node.data == (void *)&d2);
}

void bsearch_cursor__iterate_revert_zero(){
	BsearchCursor cur;

	bsearch_cursor_init(&cur, &fixture.bsearch);
	bsearch_cursor_revert(&cur);
	bool first = bsearch_cursor_next(&cur);

	bsearch_cursor_dispose(&cur);

	t_assert(!first);
}

void bsearch_cursor__iterate_revert_one(){
	BsearchEntry *a1, *b1;
	BsearchEntry d1;
	BsearchCursor cur;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;

	bsearch_cursor_init(&cur, &fixture.bsearch);
	bsearch_cursor_revert(&cur);
	bool first = bsearch_cursor_next(&cur);
	b1 = bsearch_cursor_current(&cur);
	bool second = bsearch_cursor_next(&cur);

	bsearch_cursor_dispose(&cur);

	t_assert(first);
	t_assert(!second);

	t_assert(b1 != NULL);
	t_assert(b1->node.data == (void *)&d1);
}

void bsearch_cursor__iterate_revert_two(){
	BsearchEntry *a1, *a2, *b1, *b2;
	BsearchEntry d1, d2;
	BsearchCursor cur;

	a1 = bsearch_insert(&fixture.bsearch, 'a');
	a1->node.data = (void *) &d1;
	a2 = bsearch_insert(&fixture.bsearch, 'b');
	a2->node.data = (void *) &d2;

	bsearch_cursor_init(&cur, &fixture.bsearch);
	bsearch_cursor_revert(&cur);
	bool first = bsearch_cursor_next(&cur);
	b1 = bsearch_cursor_current(&cur);
	bool second = bsearch_cursor_next(&cur);
	b2 = bsearch_cursor_current(&cur);
	bool third = bsearch_cursor_next(&cur);

	bsearch_cursor_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(!third);

	t_assert(b1 != NULL);
	t_assert(b1->node.data == (void *)&d2);
	t_assert(b2 != NULL);
	t_assert(b2->node.data == (void *)&d1);
}

int main(int argc, char** argv) {
	t_init();
	t_test(bsearch__set_and_get);
	t_test(bsearch__set2_and_get2);
	t_test(bsearch__not_set_same_key_twice);
	t_test(bsearch__set_third_in_the_middle);
	t_test(bsearch__get_gte);
	t_test(bsearch__get_lte);
	t_test(bsearch__set2_and_get2_ensure_unsigned);
	t_test(bsearch__set2_and_delete1);
	t_test(bsearch__set3_and_delete1);
	t_test(bsearch_cursor__iterate_zero);
	t_test(bsearch_cursor__iterate_one);
	t_test(bsearch_cursor__iterate_two);
	t_test(bsearch_cursor__iterate_revert_zero);
	t_test(bsearch_cursor__iterate_revert_one);
	t_test(bsearch_cursor__iterate_revert_two);
	//TODO:
	//delete
	//set_and_out_of_memory
	//delete_and_out_of_memory
	return t_done();
}

