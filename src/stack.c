#include "stack.h"

#include "cmemory.h"

void stack_init(Stack *stack)
{
	stack->top = NULL;
}

StackNode *stack_push(Stack *stack, unsigned int size)
{
	StackNode *top = c_malloc_n(size); \
	top->next = stack->top;
	stack->top = top;
	return top;
}

void stack_pop(Stack *stack)
{
	StackNode *top = stack->top;
	stack->top = top->next;
	c_delete(top);
}

void stack_dispose(Stack *stack)
{
	while(stack->top) {
		stack_pop(stack);
	}
}
