#ifndef STACK_H
#define STACK_H

#include "structs.h"

#include "stddef.h"

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

#define STACK_NODE_TYPE(NAME) StackNode##NAME
#define STACK_TYPE(NAME) Stack##NAME

#define STACK_NODE_STRUCT(TYPE, UPPER, LOWER) \
typedef struct STACK_NODE_TYPE(UPPER) { \
	StackNode stack_node; \
	TYPE data; \
} STACK_NODE_TYPE(UPPER);

#define STACK_STRUCT(TYPE, UPPER, LOWER) \
typedef struct STACK_TYPE(UPPER) { \
	Stack stack; \
} STACK_TYPE(UPPER);

#define STACK_INIT(TYPE, UPPER, LOWER) \
void stack_##LOWER##_init(struct STACK_TYPE(UPPER) *stack) { \
	stack_init(&stack->stack); \
}

#define STACK_PUSH(TYPE, UPPER, LOWER) \
void stack_##LOWER##_push(struct STACK_TYPE(UPPER) *stack, TYPE data) { \
	struct STACK_NODE_TYPE(UPPER) *node = (struct STACK_NODE_TYPE(UPPER) *)stack_push( \
		&stack->stack, \
		sizeof(struct STACK_NODE_TYPE(UPPER)) \
	); \
	node->data = data; \
}

#define STACK_POP(TYPE, UPPER, LOWER) \
void stack_##TYPE##_pop(struct STACK_TYPE(UPPER) *stack) { \
	stack_pop(&stack->stack); \
}

#define STACK_DISPOSE(TYPE, UPPER, LOWER) \
void stack_##LOWER##_dispose(struct STACK_TYPE(UPPER) *stack) { \
	stack_dispose(&stack->stack); \
}

#define DEFINE_STACK(TYPE, UPPER, LOWER) \
	STACK_NODE_STRUCT(TYPE, UPPER, LOWER) \
	STACK_STRUCT(TYPE, UPPER, LOWER) \
	STACK_INIT(TYPE, UPPER, LOWER) \
	STACK_PUSH(TYPE, UPPER, LOWER) \
	STACK_POP(TYPE, UPPER, LOWER) \
	STACK_DISPOSE(TYPE, UPPER, LOWER)


#endif //STACK_H
