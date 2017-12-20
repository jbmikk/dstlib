#ifndef STACK_H
#define STACK_H

#include "stddef.h"

#include "generics.h"

typedef struct StackNode {
	struct StackNode *next;
} StackNode;

typedef struct Stack {
	StackNode *top;
} Stack;


void stack_init(Stack *stack);
StackNode *stack_push(Stack *stack, unsigned int size);
void stack_pop(Stack *stack);
void stack_dispose(Stack *stack);


#define STACK_NODE_STRUCT(TYPE, UPPER, LOWER) \
typedef struct S(StackNode, UPPER) { \
	StackNode stack_node; \
	TYPE data; \
} S(StackNode, UPPER);

#define STACK_STRUCT(TYPE, UPPER, LOWER) \
typedef struct S(Stack, UPPER) { \
	Stack stack; \
} S(Stack, UPPER);

#define STACK_INIT(TYPE, UPPER, LOWER, BODY) \
void stack_##LOWER##_init(struct S(Stack, UPPER) *stack) BODY({ \
	stack_init(&stack->stack); \
})

#define STACK_PUSH(TYPE, UPPER, LOWER, BODY) \
void stack_##LOWER##_push(struct S(Stack, UPPER) *stack, TYPE data) BODY({ \
	struct S(StackNode, UPPER) *node = (struct S(StackNode, UPPER) *)stack_push( \
		&stack->stack, \
		sizeof(struct S(StackNode, UPPER)) \
	); \
	node->data = data; \
})

#define STACK_POP(TYPE, UPPER, LOWER, BODY) \
void stack_##LOWER##_pop(struct S(Stack, UPPER) *stack) BODY({ \
	stack_pop(&stack->stack); \
})

#define STACK_DISPOSE(TYPE, UPPER, LOWER, BODY) \
void stack_##LOWER##_dispose(struct S(Stack, UPPER) *stack) BODY({ \
	stack_dispose(&stack->stack); \
})

#define DEFINE_STACK(TYPE, UPPER, LOWER) \
	STACK_NODE_STRUCT(TYPE, UPPER, LOWER) \
	STACK_STRUCT(TYPE, UPPER, LOWER) \
	DEFINE_STACK_FUNCTIONS(TYPE, UPPER, LOWER, PROTOTYPE)

#define DEFINE_STACK_FUNCTIONS(TYPE, UPPER, LOWER, BODY) \
	STACK_INIT(TYPE, UPPER, LOWER, BODY) \
	STACK_PUSH(TYPE, UPPER, LOWER, BODY) \
	STACK_POP(TYPE, UPPER, LOWER, BODY) \
	STACK_DISPOSE(TYPE, UPPER, LOWER, BODY)


#endif //STACK_H
