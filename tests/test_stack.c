#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "test.h"

void t_setup(){
}

void t_teardown(){
}

void stack_push_pop(){

	int value = 1000;

	Stack stack;

	stack_init(&stack);
	t_assert(stack.top == NULL);

	stack_push(&stack, &value);
	t_assert(stack.top != NULL);
	t_assert(*(int *)stack.top->data == 1000);

	stack_pop(&stack);
	t_assert(stack.top == NULL);
}

void stack_dispose_many_nodes(){

	int value = 1000;
	int value2 = 2000;
	int value3 = 3000;

	Stack stack;

	stack_init(&stack);

	stack_push(&stack, &value);
	stack_push(&stack, &value2);
	stack_push(&stack, &value3);
	t_assert(stack.top != NULL);
	t_assert(*(int *)stack.top->data == 3000);

	stack_dispose(&stack);
	t_assert(stack.top == NULL);
}

int main(int argc, char** argv){

	t_init();
	t_test(stack_push_pop);
	t_test(stack_dispose_many_nodes);
	return t_done();
}

