/*
 * Types.h - Error Codes and Global Typedefs
 */


#ifndef TYPES_H
#define TYPES_H

#define ERR_OUT_OF_MEMORY	-10
#define ERR_NOT_FOUND		-11
#define ERR_IN_USE		-12
#define ERR_END_OF_INPUT	-13
#define ERR_OUT_OF_BOUNDS	-14

#define ERR_PARSE_ERROR		-20
#define ERR_SYNTAX_ERROR	-21
#define ERR_PARAMS_ERROR	-22

#define ERR_INVALID_PARAMS	-30
#define ERR_INVALID_AST_TYPE	-31
#define ERR_INVALID_TYPE	-32
#define ERR_INVALID_NAME	-33
#define ERR_INVALID_FUNCTION	-34

#undef  NULL
#define NULL			0

typedef double number_t;

#endif

