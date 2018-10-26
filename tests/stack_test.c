#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "test.h"

void t_setup(){
}

void t_teardown(){
}

struct TestStruct {
	int value1;
	char value2;
};

DEFINE(Stack, int, Int, int);
FUNCTIONS(Stack, int, Int, int)

DEFINE(Stack, char *, CharPtr, charptr);
FUNCTIONS(Stack, char *, CharPtr, charptr)

DEFINE(Stack, struct TestStruct, TestStruct, teststruct);
FUNCTIONS(Stack, struct TestStruct, TestStruct, teststruct)


void stack_push_pop_int(){

	int value = 1000;

	StackInt stack;

	stack_int_init(&stack);
	t_assert(stack.stack.top == NULL);

	stack_int_push(&stack, value);
	t_assert(stack.stack.top != NULL);
	t_assert(((StackNodeInt *)stack.stack.top)->data == 1000);

	stack_int_pop(&stack);
	t_assert(stack.stack.top == NULL);
}

void stack_top_int(){

	int value = 314;

	StackInt stack;

	stack_int_init(&stack);
	t_assert(stack.stack.top == NULL);

	stack_int_push(&stack, value);
	t_assert(stack.stack.top != NULL);

	int top = stack_int_top(&stack);
	t_assert(top == 314);

	stack_int_dispose(&stack);
}

void stack_is_empty_int(){

	int value = 314;

	StackInt stack;

	stack_int_init(&stack);
	t_assert(stack.stack.top == NULL);

	bool is_empty = stack_int_is_empty(&stack);
	t_assert(is_empty);

	stack_int_push(&stack, value);
	t_assert(stack.stack.top != NULL);

	bool is_not_empty = !stack_int_is_empty(&stack);
	t_assert(is_not_empty);

	stack_int_dispose(&stack);
}

void stack_push_pop_char_ptr()
{
	char value[] = "test-string";

	StackCharPtr stack;

	stack_charptr_init(&stack);
	t_assert(stack.stack.top == NULL);

	stack_charptr_push(&stack, value);
	t_assert(stack.stack.top != NULL);
	t_assert(((StackNodeCharPtr *)stack.stack.top)->data == value);

	stack_charptr_pop(&stack);
	t_assert(stack.stack.top == NULL);
}

void stack_push_pop_struct()
{
	struct TestStruct value = { 123, 'a' };

	StackTestStruct stack;

	stack_teststruct_init(&stack);
	t_assert(stack.stack.top == NULL);

	stack_teststruct_push(&stack, value);
	t_assert(stack.stack.top != NULL);

	struct TestStruct r = ((StackNodeTestStruct *)stack.stack.top)->data;
	t_assert(r.value1 == 123);
	t_assert(r.value2 == 'a');

	stack_teststruct_pop(&stack);
	t_assert(stack.stack.top == NULL);
}


void stack_dispose_many_nodes(){

	int value = 1000;
	int value2 = 2000;
	int value3 = 3000;

	StackInt stack;

	stack_int_init(&stack);

	stack_int_push(&stack, value);
	stack_int_push(&stack, value2);
	stack_int_push(&stack, value3);
	t_assert(stack.stack.top != NULL);
	t_assert(((StackNodeInt *)stack.stack.top)->data == 3000);

	stack_int_dispose(&stack);
	t_assert(stack.stack.top == NULL);
}

int main(int argc, char** argv){

	t_init();
	t_test(stack_push_pop_int);
	t_test(stack_top_int);
	t_test(stack_is_empty_int);
	t_test(stack_push_pop_char_ptr);
	t_test(stack_push_pop_struct);
	t_test(stack_dispose_many_nodes);
	return t_done();
}

