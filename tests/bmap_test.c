#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "bmap.h"
#include "radixtree.h"


typedef struct BTest {
	void *data;
} BTest;

DEFINE_BMAP(struct BTest, BTest, btest)

DEFINE_BMAP_FUNCTIONS(struct BTest, BTest, btest, IMPLEMENTATION)

typedef struct {
	BMapBTest bmap;
	char *str1;
	char *str2;
	char *str3;
	char *str4;
	char *str5;
} BSearchFixture;

BSearchFixture fixture;

void t_setup(){
	bmap_btest_init(&fixture.bmap);
}
void t_teardown(){
	bmap_btest_dispose(&fixture.bmap);
}

void bmap__set_and_get(){
	BMapEntryBTest *a1, *a2;
	a1 = bmap_btest_insert(&fixture.bmap, 'a', (BTest){NULL});
	a2 = bmap_btest_get(&fixture.bmap, 'a');
	t_assert(a1 != NULL);
	t_assert(a2 != NULL);
	t_assert(a1 == a2);
}

void bmap__set2_and_get2(){
	BMapEntryBTest *a2, *b2;
	BMapEntryBTest d1, d2;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });
	a2 = bmap_btest_get(&fixture.bmap, 'a');
	b2 = bmap_btest_get(&fixture.bmap, 'b');

	t_assert(bmap_btest_count(&fixture.bmap) == 2);
	t_assert(a2 != NULL);
	t_assert(a2->btest.data == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);
}


void bmap__not_set_same_key_twice()
{
	BMapEntryBTest *a1, *a2, *b1;
	BMapEntryBTest d1;

	a1 = bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	b1 = bmap_btest_insert(&fixture.bmap, 'a', (BTest){ NULL });
	a2 = bmap_btest_get(&fixture.bmap, 'a');

	t_assert(bmap_btest_count(&fixture.bmap) == 1);
	t_assert(a1 != NULL);
	t_assert(b1 == NULL);
	t_assert(a2 != NULL);
	t_assert(a2->btest.data == (void *)&d1);
}


void bmap__set_third_in_the_middle()
{
	BMapEntryBTest *a2, *b2, *c2;
	BMapEntryBTest d1, d2, d3;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'c', (BTest){ &d3 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	a2 = bmap_btest_get(&fixture.bmap, 'a');
	b2 = bmap_btest_get(&fixture.bmap, 'b');
	c2 = bmap_btest_get(&fixture.bmap, 'c');

	t_assert(bmap_btest_count(&fixture.bmap) == 3);

	t_assert(a2 != NULL);
	t_assert(a2->btest.data == (void *)&d1);

	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);

	t_assert(c2 != NULL);
	t_assert(c2->btest.data == (void *)&d3);
}


void bmap__get_gte(){
	BMapEntryBTest *out1, *out2, *out3, *out4, *out5;
	BMapEntryBTest d1, d2;

	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'd', (BTest){ &d2 });
	out1 = bmap_btest_get_gte(&fixture.bmap, 'b');
	out2 = bmap_btest_get_gte(&fixture.bmap, 'd');
	out3 = bmap_btest_get_gte(&fixture.bmap, 'a');
	out4 = bmap_btest_get_gte(&fixture.bmap, 'c');
	out5 = bmap_btest_get_gte(&fixture.bmap, 'e');

	t_assert(out1 != NULL);
	t_assert(out1->btest.data == (void *)&d1);
	t_assert(out2 != NULL);
	t_assert(out2->btest.data == (void *)&d2);
	t_assert(out3 != NULL);
	t_assert(out3->btest.data == (void *)&d1);
	t_assert(out4 != NULL);
	t_assert(out4->btest.data == (void *)&d2);
	t_assert(out5 == NULL);
}

void bmap__get_lte(){
	BMapEntryBTest *out1, *out2, *out3, *out4, *out5;
	BMapEntryBTest d1, d2;

	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'd', (BTest){ &d2 });
	out1 = bmap_btest_get_lte(&fixture.bmap, 'b');
	out2 = bmap_btest_get_lte(&fixture.bmap, 'd');
	out3 = bmap_btest_get_lte(&fixture.bmap, 'a');
	out4 = bmap_btest_get_lte(&fixture.bmap, 'c');
	out5 = bmap_btest_get_lte(&fixture.bmap, 'e');

	t_assert(out1 != NULL);
	t_assert(out1->btest.data == (void *)&d1);
	t_assert(out2 != NULL);
	t_assert(out2->btest.data == (void *)&d2);
	t_assert(out3 == NULL);
	t_assert(out4 != NULL);
	t_assert(out4->btest.data == (void *)&d1);
	t_assert(out5 != NULL);
	t_assert(out5->btest.data == (void *)&d2);
}


void bmap__get_gt(){
	BMapEntryBTest *out1, *out2, *out3, *out4, *out5;
	BMapEntryBTest d1, d2;

	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'd', (BTest){ &d2 });
	out1 = bmap_btest_get_gt(&fixture.bmap, 'b');
	out2 = bmap_btest_get_gt(&fixture.bmap, 'd');
	out3 = bmap_btest_get_gt(&fixture.bmap, 'a');
	out4 = bmap_btest_get_gt(&fixture.bmap, 'c');
	out5 = bmap_btest_get_gt(&fixture.bmap, 'e');

	t_assert(out1 != NULL);
	t_assert(out1->btest.data == (void *)&d2);
	t_assert(out2 == NULL);
	t_assert(out3 != NULL);
	t_assert(out3->btest.data == (void *)&d1);
	t_assert(out4 != NULL);
	t_assert(out4->btest.data == (void *)&d2);
	t_assert(out5 == NULL);
}


void bmap__get_lt(){
	BMapEntryBTest *out1, *out2, *out3, *out4, *out5;
	BMapEntryBTest d1, d2;

	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'd', (BTest){ &d2 });
	out1 = bmap_btest_get_lt(&fixture.bmap, 'b');
	out2 = bmap_btest_get_lt(&fixture.bmap, 'd');
	out3 = bmap_btest_get_lt(&fixture.bmap, 'a');
	out4 = bmap_btest_get_lt(&fixture.bmap, 'c');
	out5 = bmap_btest_get_lt(&fixture.bmap, 'e');

	t_assert(out1 == NULL);
	t_assert(out2 != NULL);
	t_assert(out2->btest.data == (void *)&d1);
	t_assert(out3 == NULL);
	t_assert(out4 != NULL);
	t_assert(out4->btest.data == (void *)&d1);
	t_assert(out5 != NULL);
	t_assert(out5->btest.data == (void *)&d2);
}


void bmap__set2_and_get2_ensure_unsigned(){

	bmap_btest_insert(&fixture.bmap, 10, (BTest){ NULL });
	bmap_btest_insert(&fixture.bmap, 128, (BTest){ NULL });

	t_assert(bmap_btest_count(&fixture.bmap) == 2);
	t_assert(bmap_btest_first(&fixture.bmap)->entry.key == 10);
}

void bmap__set2_and_delete1(){
	BMapEntryBTest *a2, *b2;
	BMapEntryBTest d1, d2;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	bmap_btest_delete(&fixture.bmap, 'a');

	a2 = bmap_btest_get(&fixture.bmap, 'a');
	b2 = bmap_btest_get(&fixture.bmap, 'b');

	t_assert(bmap_btest_count(&fixture.bmap) == 1);
	t_assert(a2 == NULL);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);
}

void bmap__set3_and_delete1(){
	BMapEntryBTest *a2, *b2, *c2;
	BMapEntryBTest d1, d2, d3;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });
	bmap_btest_insert(&fixture.bmap, 'c', (BTest){ &d3 });

	bmap_btest_delete(&fixture.bmap, 'b');

	a2 = bmap_btest_get(&fixture.bmap, 'a');
	b2 = bmap_btest_get(&fixture.bmap, 'b');
	c2 = bmap_btest_get(&fixture.bmap, 'c');

	t_assert(bmap_btest_count(&fixture.bmap) == 2);

	t_assert(a2 != NULL);
	t_assert(a2->btest.data == (void *)&d1);

	t_assert(b2 == NULL);

	t_assert(c2 != NULL);
	t_assert(c2->btest.data == (void *)&d3);
}

void bmap_cursor_btest__iterate_zero(){
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(!first);
}

void bmap_cursor_btest__iterate_one(){
	BMapEntryBTest *b1;
	BMapEntryBTest d1;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(!second);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&d1);
}

void bmap_cursor_btest__iterate_two(){
	BMapEntryBTest *b1, *b2;
	BMapEntryBTest d1, d2;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(!third);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);
}

void bmap_cursor_btest__iterate_revert_zero(){
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_revert(&cur);
	bool first = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(!first);
}

void bmap_cursor_btest__iterate_revert_one(){
	BMapEntryBTest *b1;
	BMapEntryBTest d1;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_revert(&cur);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(!second);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&d1);
}

void bmap_cursor_btest__iterate_revert_two(){
	BMapEntryBTest *b1, *b2;
	BMapEntryBTest d1, d2;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_revert(&cur);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(!third);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&d2);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d1);
}

void bmap_cursor_btest__move_lt_first(){
	BMapEntryBTest *b1, *b2;
	BMapEntryBTest d1, d2;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_move_lt(&cur, 'a');
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(!third);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);
}

void bmap_cursor_btest__move_lt_mid(){
	BMapEntryBTest *b2;
	BMapEntryBTest d1, d2;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_move_lt(&cur, 'b');
	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(second);
	t_assert(!third);

	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);
}

void bmap_cursor_btest__move_gt_last(){
	BMapEntryBTest *b1, *b2;
	BMapEntryBTest d1, d2;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_revert(&cur);
	bmap_cursor_btest_move_gt(&cur, 'b');
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(!third);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&d2);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d1);
}

void bmap_cursor_btest__move_gt_mid(){
	BMapEntryBTest *b2;
	BMapEntryBTest d1, d2;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_revert(&cur);
	bmap_cursor_btest_move_gt(&cur, 'a');
	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(second);
	t_assert(!third);

	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d1);
}


int main(int argc, char** argv) {
	t_init();
	t_test(bmap__set_and_get);
	t_test(bmap__set2_and_get2);
	t_test(bmap__not_set_same_key_twice);
	t_test(bmap__set_third_in_the_middle);
	t_test(bmap__get_gte);
	t_test(bmap__get_lte);
	t_test(bmap__get_gt);
	t_test(bmap__get_lt);
	t_test(bmap__set2_and_get2_ensure_unsigned);
	t_test(bmap__set2_and_delete1);
	t_test(bmap__set3_and_delete1);
	t_test(bmap_cursor_btest__iterate_zero);
	t_test(bmap_cursor_btest__iterate_one);
	t_test(bmap_cursor_btest__iterate_two);
	t_test(bmap_cursor_btest__iterate_revert_zero);
	t_test(bmap_cursor_btest__iterate_revert_one);
	t_test(bmap_cursor_btest__iterate_revert_two);
	t_test(bmap_cursor_btest__move_lt_first);
	t_test(bmap_cursor_btest__move_lt_mid);
	t_test(bmap_cursor_btest__move_gt_last);
	t_test(bmap_cursor_btest__move_gt_mid);
	//TODO:
	//delete
	//set_and_out_of_memory
	//delete_and_out_of_memory
	return t_done();
}

