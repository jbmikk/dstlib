#include <stdio.h>
#include <stdlib.h>

#include "ctest.h"
#include "bmap.h"
#include "rtree.h"


typedef struct BTest {
	void *data;
} BTest;

DEFINE(BMap, unsigned char, struct BTest, BTest, btest)

FUNCTIONS(BMap, unsigned char, struct BTest, BTest, btest)

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
	Result(BMapEntryBTestPtr) a1;
	BMapEntryBTest *a2;
	a1 = bmap_btest_insert(&fixture.bmap, 'a', (BTest){NULL});
	a2 = bmap_btest_get(&fixture.bmap, 'a');
	t_assert(TypeOf(a1) == Type(Result, Ok));
	t_assert(a1.data != NULL);
	t_assert(a2 != NULL);
	t_assert(a1.data == a2);
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

void bmap__get_index(){
	BMapEntryBTest d1, d2;

	bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 'b', (BTest){ &d2 });

	int index_a = bmap_btest_get_index(&fixture.bmap, 'a');
	int index_b = bmap_btest_get_index(&fixture.bmap, 'b');
	int index_c = bmap_btest_get_index(&fixture.bmap, 'c');

	t_assert(index_a == 0);
	t_assert(index_b == 1);
	t_assert(index_c == -1);
}


void bmap__not_set_same_key_twice()
{
	Result(BMapEntryBTestPtr) a1, b1;
	BMapEntryBTest *a2;
	BMapEntryBTest d1;

	a1 = bmap_btest_insert(&fixture.bmap, 'a', (BTest){ &d1 });
	b1 = bmap_btest_insert(&fixture.bmap, 'a', (BTest){ NULL });
	a2 = bmap_btest_get(&fixture.bmap, 'a');

	t_assert(bmap_btest_count(&fixture.bmap) == 1);
	t_assert(TypeOf(a1) == Type(Result, Ok));
	t_assert(TypeOf(b1) == Type(Result, Ok));
	t_assert(a1.data != NULL);
	t_assert(b1.data == NULL);
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
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
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

void bmap__append_key_middle(){

	BMapEntryBTest *b1, *b2, *b3;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_insert(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_insert(&fixture.bmap, 128, (BTest){ &dummy2 });
	bmap_btest_m_append(&fixture.bmap, 10, (BTest){ &dummy3 });

	t_assert(bmap_btest_count(&fixture.bmap) == 3);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);

	bool fourth = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy1);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy3);

	t_assert(third);
	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&dummy2);
	t_assert(!fourth);
}

void bmap__append_key_middle_unique(){

	BMapEntryBTest *b1, *b2, *b3;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_m_append(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_append(&fixture.bmap, 128, (BTest){ &dummy2 });
	bmap_btest_m_append(&fixture.bmap, 50, (BTest){ &dummy3 });

	t_assert(bmap_btest_count(&fixture.bmap) == 3);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);

	bool fourth = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy1);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy3);

	t_assert(third);
	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&dummy2);
	t_assert(!fourth);
}

void bmap__append_key_last(){

	BMapEntryBTest *b1, *b2, *b3;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_insert(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_insert(&fixture.bmap, 128, (BTest){ &dummy2 });
	bmap_btest_m_append(&fixture.bmap, 128, (BTest){ &dummy3 });

	t_assert(bmap_btest_count(&fixture.bmap) == 3);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);

	bool fourth = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy1);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy2);

	t_assert(third);
	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&dummy3);
	t_assert(!fourth);
}

void bmap__append_key_new_last(){

	BMapEntryBTest *b1, *b2;
	int dummy1 = 0, dummy2 = 0;

	bmap_btest_insert(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_append(&fixture.bmap, 128, (BTest){ &dummy2 });

	t_assert(bmap_btest_count(&fixture.bmap) == 2);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy1);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy2);

	t_assert(!third);
}

void bmap__append_key_single(){

	BMapEntryBTest *b1, *b2;
	int dummy1 = 0, dummy2 = 0;

	bmap_btest_insert(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_append(&fixture.bmap, 128, (BTest){ &dummy2 });

	t_assert(bmap_btest_count(&fixture.bmap) == 2);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy1);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy2);

	t_assert(!third);
}

void bmap__prepend_key_middle(){

	BMapEntryBTest *b1, *b2, *b3;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_insert(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_insert(&fixture.bmap, 128, (BTest){ &dummy2 });
	bmap_btest_m_prepend(&fixture.bmap, 128, (BTest){ &dummy3 });

	t_assert(bmap_btest_count(&fixture.bmap) == 3);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);

	bool fourth = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy1);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy3);

	t_assert(third);
	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&dummy2);
	t_assert(!fourth);
}

void bmap__prepend_key_middle_unique(){

	BMapEntryBTest *b1, *b2, *b3;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_prepend(&fixture.bmap, 128, (BTest){ &dummy2 });
	bmap_btest_m_prepend(&fixture.bmap, 50, (BTest){ &dummy3 });

	t_assert(bmap_btest_count(&fixture.bmap) == 3);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);

	bool fourth = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy1);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy3);

	t_assert(third);
	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&dummy2);
	t_assert(!fourth);
}

void bmap__prepend_key_last(){

	BMapEntryBTest *b1, *b2, *b3;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_insert(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_insert(&fixture.bmap, 50, (BTest){ &dummy2 });
	bmap_btest_m_prepend(&fixture.bmap, 128, (BTest){ &dummy3 });

	t_assert(bmap_btest_count(&fixture.bmap) == 3);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);

	bool fourth = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy1);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy2);

	t_assert(third);
	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&dummy3);
	t_assert(!fourth);
}

void bmap__prepend_key_first(){

	BMapEntryBTest *b1, *b2;
	int dummy1 = 0, dummy2 = 0;

	bmap_btest_insert(&fixture.bmap, 128, (BTest){ &dummy1 });
	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy2 });

	t_assert(bmap_btest_count(&fixture.bmap) == 2);
	t_assert(bmap_btest_first(&fixture.bmap)->key == 10);
	BMapCursorBTest cur;

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);

	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);

	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy2);

	t_assert(second);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy1);

	t_assert(!third);
}

void bmap__m_get(){

	BMapEntryBTest *b1;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy2 });
	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy3 });

	b1 = bmap_btest_m_get(&fixture.bmap, 10);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy3);
}

void bmap__m_get_not_found(){

	BMapEntryBTest *b1;
	int dummy1 = 0, dummy2 = 0;

	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_prepend(&fixture.bmap, 50, (BTest){ &dummy2 });

	b1 = bmap_btest_m_get(&fixture.bmap, 40);

	t_assert(b1 == NULL);
}

void bmap__m_get_at(){

	BMapEntryBTest *b1, *b2, *b3, *b4, *b5, *b6;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy2 });
	bmap_btest_m_prepend(&fixture.bmap, 10, (BTest){ &dummy3 });

	b1 = bmap_btest_m_get_at(&fixture.bmap, 10, 0);
	b2 = bmap_btest_m_get_at(&fixture.bmap, 10, 1);
	b3 = bmap_btest_m_get_at(&fixture.bmap, 10, 2);
	b4 = bmap_btest_m_get_at(&fixture.bmap, 10, 3);
	b5 = bmap_btest_m_get_at(&fixture.bmap, 40, 3);
	b6 = bmap_btest_m_get_at(&fixture.bmap, 7, 3);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy3);

	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy2);

	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&dummy1);

	t_assert(b4 == NULL);
	t_assert(b5 == NULL);
	t_assert(b6 == NULL);
}

void bmap__m_get_last(){

	BMapEntryBTest *b1, *b2, *b3;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0;

	bmap_btest_m_append(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_append(&fixture.bmap, 10, (BTest){ &dummy2 });
	bmap_btest_m_append(&fixture.bmap, 10, (BTest){ &dummy3 });

	b1 = bmap_btest_m_get_last(&fixture.bmap, 10);
	b2 = bmap_btest_m_get_last(&fixture.bmap, 40);
	b3 = bmap_btest_m_get_last(&fixture.bmap, 7);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&dummy3);

	t_assert(b2 == NULL);
	t_assert(b3 == NULL);
}

void bmap__m_get_last_middle(){

	BMapEntryBTest *b1, *b2, *b3, *b4, *b5;
	int dummy1 = 0, dummy2 = 0, dummy3 = 0, dummy4 = 0, dummy5 = 0;

	bmap_btest_m_append(&fixture.bmap, 10, (BTest){ &dummy1 });
	bmap_btest_m_append(&fixture.bmap, 10, (BTest){ &dummy2 });
	bmap_btest_m_append(&fixture.bmap, 10, (BTest){ &dummy3 });
	bmap_btest_m_append(&fixture.bmap, 12, (BTest){ &dummy4 });
	bmap_btest_m_append(&fixture.bmap, 12, (BTest){ &dummy5 });

	b1 = bmap_btest_m_get_last(&fixture.bmap, 7);
	b2 = bmap_btest_m_get_last(&fixture.bmap, 10);
	b3 = bmap_btest_m_get_last(&fixture.bmap, 11);
	b4 = bmap_btest_m_get_last(&fixture.bmap, 12);
	b5 = bmap_btest_m_get_last(&fixture.bmap, 40);

	t_assert(b1 == NULL);

	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&dummy3);

	t_assert(b3 == NULL);

	t_assert(b4 != NULL);
	t_assert(b4->btest.data == (void *)&dummy5);

	t_assert(b5 == NULL);
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

void bmap_cursor_btest__move_lt__multiple(){
	BMapEntryBTest *b1, *b2, *b3, *b4;
	BMapEntryBTest d1, d2, d3, d4;
	BMapCursorBTest cur;

	bmap_btest_m_append(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_m_append(&fixture.bmap, 'a', (BTest){ &d2 });
	bmap_btest_m_append(&fixture.bmap, 'a', (BTest){ &d3 });
	bmap_btest_m_append(&fixture.bmap, 'b', (BTest){ &d4 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_move_lt(&cur, 'a');
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);
	bool fourth = bmap_cursor_btest_next(&cur);
	b4 = bmap_cursor_btest_current(&cur);
	bool fifth = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(third);
	t_assert(fourth);
	t_assert(!fifth);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);
	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&d3);
	t_assert(b4 != NULL);
	t_assert(b4->btest.data == (void *)&d4);
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

void bmap_cursor_btest__move_gt__multiple(){
	BMapEntryBTest *b1, *b2, *b3, *b4;
	BMapEntryBTest d1, d2, d3, d4;
	BMapCursorBTest cur;

	bmap_btest_m_append(&fixture.bmap, 'a', (BTest){ &d1 });
	bmap_btest_m_append(&fixture.bmap, 'b', (BTest){ &d2 });
	bmap_btest_m_append(&fixture.bmap, 'b', (BTest){ &d3 });
	bmap_btest_m_append(&fixture.bmap, 'b', (BTest){ &d4 });

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bmap_cursor_btest_revert(&cur);
	bmap_cursor_btest_move_gt(&cur, 'b');
	bool first = bmap_cursor_btest_next(&cur);
	b1 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);
	b2 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);
	bool fourth = bmap_cursor_btest_next(&cur);
	b4 = bmap_cursor_btest_current(&cur);
	bool fifth = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(third);
	t_assert(fourth);
	t_assert(!fifth);

	t_assert(b1 != NULL);
	t_assert(b1->btest.data == (void *)&d4);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d3);
	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&d2);
	t_assert(b4 != NULL);
	t_assert(b4->btest.data == (void *)&d1);
}


int main(int argc, char** argv) {
	t_init();
	t_test(bmap__set_and_get);
	t_test(bmap__set2_and_get2);
	t_test(bmap__get_index);
	t_test(bmap__not_set_same_key_twice);
	t_test(bmap__set_third_in_the_middle);
	t_test(bmap__get_gte);
	t_test(bmap__get_lte);
	t_test(bmap__get_gt);
	t_test(bmap__get_lt);
	t_test(bmap__set2_and_get2_ensure_unsigned);
	t_test(bmap__set2_and_delete1);
	t_test(bmap__set3_and_delete1);
	t_test(bmap__append_key_middle);
	t_test(bmap__append_key_middle_unique);
	t_test(bmap__append_key_last);
	t_test(bmap__append_key_new_last);
	t_test(bmap__append_key_single);
	t_test(bmap__prepend_key_middle);
	t_test(bmap__prepend_key_middle_unique);
	t_test(bmap__prepend_key_last);
	t_test(bmap__prepend_key_first);
	t_test(bmap__m_get);
	t_test(bmap__m_get_not_found);
	t_test(bmap__m_get_at);
	t_test(bmap__m_get_last);
	t_test(bmap__m_get_last_middle);
	t_test(bmap_cursor_btest__iterate_zero);
	t_test(bmap_cursor_btest__iterate_one);
	t_test(bmap_cursor_btest__iterate_two);
	t_test(bmap_cursor_btest__iterate_revert_zero);
	t_test(bmap_cursor_btest__iterate_revert_one);
	t_test(bmap_cursor_btest__iterate_revert_two);
	t_test(bmap_cursor_btest__move_lt_first);
	t_test(bmap_cursor_btest__move_lt_mid);
	t_test(bmap_cursor_btest__move_lt__multiple);
	t_test(bmap_cursor_btest__move_gt_last);
	t_test(bmap_cursor_btest__move_gt_mid);
	t_test(bmap_cursor_btest__move_gt__multiple);

	//TODO:
	//delete
	//set_and_out_of_memory
	//delete_and_out_of_memory
	return t_done();
}

