/*
 * Globals.h - Error Codes and Global Typedefs
 */


#ifndef _SDRL_GLOBALS_H
#define _SDRL_GLOBALS_H

#define SDRL_ERR_OUT_OF_MEMORY		-2
#define SDRL_ERR_NOT_FOUND		-3
#define SDRL_ERR_IN_USE			-4
#define SDRL_ERR_END_OF_INPUT		-5
#define SDRL_ERR_OUT_OF_BOUNDS		-6

#define SDRL_ERR_PARSE_ERROR		-8
#define SDRL_ERR_SYNTAX_ERROR		-9
#define SDRL_ERR_PARAMS_ERROR		-10

#define SDRL_ERR_INVALID_PARAMS		-12
#define SDRL_ERR_INVALID_AST_TYPE	-13
#define SDRL_ERR_INVALID_TYPE		-14
#define SDRL_ERR_INVALID_NAME		-15
#define SDRL_ERR_INVALID_FUNCTION	-16

#define SDRL_MAKE_LINENUMBER(line, col) \
	(line << 16 | col)
#define SDRL_GET_LINE_NUMBER(linecol) \
	(linecol >> 16)
#define SDRL_GET_COLUMN_NUMBER(linecol) \
	(linecol & 0xffff)

typedef unsigned int linenumber_t;

typedef double number_t;

/** Increments the count of a reference for values and so on. (destroy decrements) **/
#define SDRL_MAKE_REFERENCE(datum) \
	(++(datum)->refs ? (datum) : NULL)

/** Decrements the count of a reference for values and so on and calls destroy if the count is 0 **/
#define SDRL_DESTROY_REFERENCE(datum, func) \
	( ((datum)->refs == 1) ?		\
		( func(datum), 1 )		\
		: ( --(datum)->refs, 0 ) )

struct sdrl_heap;
struct sdrl_value;
struct sdrl_machine;

typedef void *(*sdrl_create_t)(struct sdrl_machine *, struct sdrl_value *);
typedef int (*sdrl_evaluate_t)(struct sdrl_machine *, ...);
typedef void *(*sdrl_duplicate_t)(struct sdrl_heap *, void *);
typedef int (*sdrl_destroy_t)(struct sdrl_heap *, void *);

#endif

