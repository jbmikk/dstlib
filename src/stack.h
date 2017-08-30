#ifndef STACK_H
#define STACK_H

#include "structs.h"

typedef struct _StackNode {
	void *data;
	struct _StackNode *next;
} StackNode;

typedef struct _Stack {
	StackNode *top;
} Stack;


void stack_init(Stack *stack);
StackNode *stack_push(Stack *stack, void *ptr);
void stack_pop(Stack *stack);
void stack_dispose(Stack *stack);

#endif //STACK_H
