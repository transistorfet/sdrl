/*
 * Globals.h - Error Codes and Global Typedefs
 */


#ifndef _SDRL_CORE_GLOBALS_H
#define _SDRL_CORE_GLOBALS_H

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

#define sdrl_make_linenumber_m(line, col) \
	(line << 16 | col)
#define sdrl_get_line_number_m(linecol) \
	(line >> 16)
#define sdrl_get_column_number_m(linecol) \
	(col & 0xffff)

typedef unsigned int linenumber_t;

typedef double number_t;

/** Increments the count of a reference for values and so on. (destroy decrements) **/
#define sdrl_make_reference_m(datum) \
	(++datum->refs ? datum : NULL)

/** Decrements the count of a reference for values and so on and calls destroy if the count is 0 **/
#define sdrl_destroy_reference_m(datum, func) \
	((datum->refs == 1) ? func(datum) : --datum->refs)

struct sdrl_heap;
struct sdrl_machine;

typedef int (*sdrl_evaluate_t)(struct sdrl_machine *, ...);
typedef void *(*sdrl_duplicate_t)(struct sdrl_heap *, void *);
typedef int (*sdrl_destroy_t)(struct sdrl_heap *, void *);

#endif

