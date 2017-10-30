#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "test.h"

void t_setup(){
}

void t_teardown(){
}

DEFINE_STACK(int, Int, int);

void stack_push_pop(){

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
	t_test(stack_push_pop);
	t_test(stack_dispose_many_nodes);
	return t_done();
}

