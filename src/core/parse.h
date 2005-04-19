/*
 * Name:	parse.h
 * Version:	0.2
 * Description:	Parser Header
 */


#ifndef _SDRL_CORE_PARSE_H
#define _SDRL_CORE_PARSE_H

#include "expr.h"
#include "input.h"
#include "globals.h"

typedef struct sdrl_expr *(*sdrl_parser_t)(struct sdrl_input *, void *);

struct sdrl_expr *sdrl_parse_file(char *, sdrl_parser_t, void *);
struct sdrl_expr *sdrl_parse_string(char *, int, sdrl_parser_t, void *);

#endif

