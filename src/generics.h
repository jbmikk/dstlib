#ifndef GENERICS_H
#define GENERICS_H

#define S(GTYPE, PTYPE) GTYPE##PTYPE

#define _EXPAND_FUNCTION(NAME, ...) \
	NAME(__VA_ARGS__)

#define _PROTOTYPE(BLOCK) ;
#define _IMPLEMENTATION(BLOCK) BLOCK

#define TYPEDEF(BASE_TYPE, ...) BASE_TYPE##_TYPEDEF(__VA_ARGS__)

#define FUNCTIONS(BASE_TYPE, ...) BASE_TYPE##_FUNCTIONS(__VA_ARGS__, _IMPLEMENTATION)

#define PROTOTYPES(BASE_TYPE, ...) BASE_TYPE##_FUNCTIONS(__VA_ARGS__, _PROTOTYPE)

#define DEFINE(BASE_TYPE, ...) BASE_TYPE##_DEFINE(__VA_ARGS__)

#endif //GENERICS_H
