/*
 * Globals.h - Error Codes and Global Typedefs
 */


#ifndef _SDRL_GLOBALS_H
#define _SDRL_GLOBALS_H

#define SDRL_ERR_FAILED			-1

#define SDRL_ERR_OUT_OF_MEMORY		-2

#define SDRL_ERR_PARSE_ERROR		-3
#define SDRL_ERR_SYNTAX_ERROR		-4
#define SDRL_ERR_END_OF_INPUT		-5

#define SDRL_ERR_INVALID_AST_TYPE	-6
#define SDRL_ERR_INVALID_NAME		-7
#define SDRL_ERR_INVALID_FUNCTION	-8
#define SDRL_ERR_INVALID_TYPE		-9
#define SDRL_ERR_INVALID_ARGS		-10

#define SDRL_ERR_NOT_FOUND		-11
#define SDRL_ERR_IN_USE			-12
#define SDRL_ERR_OUT_OF_BOUNDS		-13

#define SDRL_ERR_DIVIDE_BY_ZERO		-14


#define SDRL_BF_IS_SET(value, mask)	\
	( (value)->bitflags & mask )
#define SDRL_BF_SET(value, mask)	\
	( (value)->bitflags |= mask )
#define SDRL_BF_CLEAR(value, mask)	\
	( (value)->bitflags &= ~mask )

#define SDRL_MAKE_LINENUMBER(line, col) \
	(line << 16 | col)
#define SDRL_GET_LINE_NUMBER(linecol) \
	(linecol >> 16)
#define SDRL_GET_COLUMN_NUMBER(linecol) \
	(linecol & 0xffff)

typedef unsigned int linenumber_t;

typedef double number_t;

struct sdrl_heap;
struct sdrl_expr;
struct sdrl_type;
struct sdrl_value;
struct sdrl_input;
struct sdrl_machine;

typedef struct sdrl_value *(*sdrl_create_t)(struct sdrl_machine *mach, struct sdrl_type *type, struct sdrl_value *args);
typedef int (*sdrl_destroy_t)(struct sdrl_value *value);
typedef struct sdrl_value *(*sdrl_duplicate_t)(struct sdrl_machine *mach, struct sdrl_value *value);
typedef int (*sdrl_evaluate_t)(struct sdrl_machine *mach, struct sdrl_value *value, struct sdrl_value *args);

typedef struct sdrl_expr *(*sdrl_parser_t)(struct sdrl_input *, void *);

typedef int (*sdrl_func_t)(struct sdrl_machine *, struct sdrl_value *);

#endif

