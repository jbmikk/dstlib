#ifndef GENERICS_H
#define GENERICS_H


// Helper macros for macro abuse && recursion
#define EMPTY()
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__
#define EVAL(...) \
	EXPAND(EXPAND(EXPAND(EXPAND(EXPAND(__VA_ARGS__)))))


#define S(GTYPE, PTYPE) GTYPE##PTYPE

#define _EXPAND_FUNCTION(NAME, ...) \
	NAME(__VA_ARGS__)

#define _PROTOTYPE(BLOCK) ;
#define _IMPLEMENTATION(BLOCK) BLOCK


// Public interface for specifying concrete types from generic ones.

#define TYPEDEF(BASE_TYPE, ...) BASE_TYPE##_TYPEDEF(__VA_ARGS__)

// Invoke type extensions for a type
#define EXTEND_TYPE(BASE_TYPE_A, BASE_TYPE_B, ...) BASE_TYPE_A##__WITH__##BASE_TYPE_B(__VA_ARGS__)


// Definitions for all parts of a type
// -----------------------------------

#define TYPES(BASE_TYPE, ...) \
	BASE_TYPE##_TYPE_LIST(TYPEDEF, __VA_ARGS__)

#define FUNCTIONS(BASE_TYPE, ...) \
	BASE_TYPE##_FUNCTION_LIST(_EXPAND_FUNCTION, __VA_ARGS__, _IMPLEMENTATION)

#define PROTOTYPES(BASE_TYPE, ...) \
	BASE_TYPE##_FUNCTION_LIST(_EXPAND_FUNCTION, __VA_ARGS__, _PROTOTYPE)

// Inner implementation, should only be used by wrappers
#define _INNER_EXTEND_TYPES(BASE_TYPE, ...) \
	BASE_TYPE##_EXTEND_TYPE_LIST(EXTEND_TYPE, __VA_ARGS__)

// Use inside other macros
#define NESTED_EXTEND_TYPES(...) DEFER(_INNER_EXTEND_TYPES)( __VA_ARGS__)

// Wrapper for regular usage
#define EXTEND_TYPES(...) EVAL(_INNER_EXTEND_TYPES( __VA_ARGS__))


// Main type definition macros
// ---------------------------

// Inner implementation, should only be used by wrappers
#define _INNER_DEFINE(BASE_TYPE, ...) \
	TYPES(BASE_TYPE, __VA_ARGS__) \
	NESTED_EXTEND_TYPES(BASE_TYPE, __VA_ARGS__) \
	PROTOTYPES(BASE_TYPE, __VA_ARGS__) \

// Wrapper for nested extensions
#define NESTED_DEFINE(...) DEFER(_INNER_DEFINE)(__VA_ARGS__)

// Wrapper for regular usage
#define DEFINE(...) EVAL( _INNER_DEFINE( __VA_ARGS__))

#endif //GENERICS_H
