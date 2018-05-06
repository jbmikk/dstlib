#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "bmap.h"
#include "rtree.h"


typedef struct BTest {
	void *data;
} BTest;

DEFINE(BMap, unsigned int, struct BTest, BTest, btest)

FUNCTIONS(BMap, unsigned int, struct BTest, BTest, btest)

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

void bmap__set_257_elements(){
	BMapEntryBTest d1;

	unsigned int a = 0;

	for(; a < 257; a++) {
		bmap_btest_insert(&fixture.bmap, a, (BTest){ &d1 });
	}

	t_assert(bmap_btest_count(&fixture.bmap) == 257);
}


int main(int argc, char** argv) {
	t_init();
	t_test(bmap__set_and_get);
	t_test(bmap__set2_and_get2);
	t_test(bmap__not_set_same_key_twice);
	t_test(bmap__set2_and_get2_ensure_unsigned);
	t_test(bmap__set2_and_delete1);
	t_test(bmap__set_257_elements);
	return t_done();
}

