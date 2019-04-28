#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "bmap.h"
#include "rtree.h"


typedef struct BTest {
	void *data;
} BTest;

DEFINE(BMap, char, struct BTest, BTest, btest)

FUNCTIONS(BMap, char, struct BTest, BTest, btest)

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

void bmap__set_positive_and_negative(){
	BMapEntryBTest *a2, *b2;
	BMapEntryBTest d1, d2;

	bmap_btest_insert(&fixture.bmap, -3, (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 2, (BTest){ &d2 });
	a2 = bmap_btest_get(&fixture.bmap, -3);
	b2 = bmap_btest_get(&fixture.bmap, 2);

	t_assert(bmap_btest_count(&fixture.bmap) == 2);
	t_assert(a2 != NULL);
	t_assert(a2->btest.data == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);
}

void bmap__iterate_positive_and_negative(){
	BMapEntryBTest *a2, *b2, *b3, *b4;
	BMapEntryBTest d1, d2;
	BMapCursorBTest cur;

	bmap_btest_insert(&fixture.bmap, -3, (BTest){ &d1 });
	bmap_btest_insert(&fixture.bmap, 2, (BTest){ &d2 });
	a2 = bmap_btest_get(&fixture.bmap, -3);
	b2 = bmap_btest_get(&fixture.bmap, 2);

	t_assert(bmap_btest_count(&fixture.bmap) == 2);
	t_assert(a2 != NULL);
	t_assert(a2->btest.data == (void *)&d1);
	t_assert(b2 != NULL);
	t_assert(b2->btest.data == (void *)&d2);

	bmap_cursor_btest_init(&cur, &fixture.bmap);
	bool first = bmap_cursor_btest_next(&cur);
	b3 = bmap_cursor_btest_current(&cur);
	bool second = bmap_cursor_btest_next(&cur);
	b4 = bmap_cursor_btest_current(&cur);
	bool third = bmap_cursor_btest_next(&cur);

	bmap_cursor_btest_dispose(&cur);

	t_assert(first);
	t_assert(second);
	t_assert(!third);

	t_assert(b3 != NULL);
	t_assert(b3->btest.data == (void *)&d1);
	t_assert(b4 != NULL);
	t_assert(b4->btest.data == (void *)&d2);
}

int main(int argc, char** argv) {
	t_init();
	t_test(bmap__set_and_get);
	t_test(bmap__set2_and_get2);
	t_test(bmap__set_positive_and_negative);
	t_test(bmap__iterate_positive_and_negative);
	return t_done();
}

