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

// TODO should i change the name of this?
typedef double number_t;

typedef struct sdEnv sdEnv;
typedef struct sdCont sdCont;
typedef struct sdHeap sdHeap;
typedef struct sdExpr sdExpr;
typedef struct sdType sdType;
typedef struct sdArray sdArray;
typedef struct sdError sdError;
typedef struct sdEvent sdEvent;
typedef struct sdValue sdValue;
typedef struct sdInput sdInput;
typedef struct sdMachine sdMachine;

typedef sdValue *(*sdrl_create_t)(sdMachine *mach, sdType *type, sdArray *args);
typedef int (*sdrl_destroy_t)(sdValue *value);
typedef sdValue *(*sdrl_duplicate_t)(sdMachine *mach, sdValue *value);
typedef int (*sdrl_evaluate_t)(sdMachine *mach, sdArray *args);

// TODO should that sdInput be there???
typedef sdExpr *(*sdrl_parser_t)(sdMachine *, sdInput *);

typedef int (*sdrl_func_t)(sdMachine *, sdArray *);

#endif

