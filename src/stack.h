#ifndef Stack_H
#define Stack_H

#include <stdbool.h>
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


#define StackNode_TYPEDEF(TYPE, UPPER, LOWER) \
typedef struct S(StackNode, UPPER) { \
	StackNode stack_node; \
	TYPE data; \
} S(StackNode, UPPER);

#define Stack_TYPEDEF(TYPE, UPPER, LOWER) \
typedef struct S(Stack, UPPER) { \
	Stack stack; \
} S(Stack, UPPER);

#define Stack_INIT(TYPE, UPPER, LOWER, BODY) \
void stack_##LOWER##_init(struct S(Stack, UPPER) *stack) BODY({ \
	stack_init(&stack->stack); \
})

#define Stack_PUSH(TYPE, UPPER, LOWER, BODY) \
void stack_##LOWER##_push(struct S(Stack, UPPER) *stack, TYPE data) BODY({ \
	struct S(StackNode, UPPER) *node = (struct S(StackNode, UPPER) *)stack_push( \
		&stack->stack, \
		sizeof(struct S(StackNode, UPPER)) \
	); \
	node->data = data; \
})

#define Stack_POP(TYPE, UPPER, LOWER, BODY) \
void stack_##LOWER##_pop(struct S(Stack, UPPER) *stack) BODY({ \
	stack_pop(&stack->stack); \
})

#define Stack_TOP(TYPE, UPPER, LOWER, BODY) \
TYPE stack_##LOWER##_top(struct S(Stack, UPPER) *stack) BODY({ \
	return ((S(StackNode, UPPER) *)(stack->stack.top))->data; \
})

#define Stack_IS_EMPTY(TYPE, UPPER, LOWER, BODY) \
bool stack_##LOWER##_is_empty(struct S(Stack, UPPER) *stack) BODY({ \
	return !stack->stack.top; \
})

#define Stack_DISPOSE(TYPE, UPPER, LOWER, BODY) \
void stack_##LOWER##_dispose(struct S(Stack, UPPER) *stack) BODY({ \
	stack_dispose(&stack->stack); \
})


// Lists

#define Stack_EXTEND_TYPE_LIST(_, ...)

#define Stack_TYPE_LIST(_, ...) \
	_(StackNode, __VA_ARGS__) \
	_(Stack, __VA_ARGS__)

#define Stack_FUNCTION_LIST(_, ...) \
	_(Stack_INIT, __VA_ARGS__) \
	_(Stack_PUSH, __VA_ARGS__) \
	_(Stack_POP, __VA_ARGS__) \
	_(Stack_TOP, __VA_ARGS__) \
	_(Stack_IS_EMPTY, __VA_ARGS__) \
	_(Stack_DISPOSE, __VA_ARGS__)


#endif //Stack_H
