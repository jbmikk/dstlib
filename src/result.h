#ifndef Result_H
#define	Result_H

#include "generics.h"

#define	_Result_TYPE_Ok 0
#define	_Result_TYPE_Error 1

typedef struct _Result {
	char type;
} _Result;

#define _Result_TYPEDEF(TYPE, UPPER, LOWER) \
typedef struct S(_Result, UPPER) { \
	_Result result;	\
	union { \
		int error; \
		TYPE data; \
	}; \
} S(_Result, UPPER);


// Lists

#define _Result_EXTEND_TYPE_LIST(_, ...)

#define _Result_TYPE_LIST(_, ...) \
	_(_Result, __VA_ARGS__)

#define _Result_FUNCTION_LIST(_, ...)

// User macros

#define Result(UPPER) S(_Result, UPPER)

#define ResultOk(UPPER, V) (S(_Result, UPPER)) { \
	.result = (_Result) { .type = _Result_TYPE_Ok }, \
	.data = (V) \
}

#define ResultError(UPPER, E) (S(_Result, UPPER)) { \
	.result = (_Result) { .type = _Result_TYPE_Error }, \
	.error = (E) \
}

#define TypeOf(E) ((E).result.type)

#define Type(B, T) _##B##_TYPE_##T


#endif	//Result_H
