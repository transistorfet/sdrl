/*
 * Name:	parse.h
 * Version:	0.1
 * Description:	Parser Header
 */


#ifndef _SDRL_PARSE_H
#define _SDRL_PARSE_H

#include "expr.h"
#include "input.h"
#include "globals.h"

struct sdrl_expr *sdrl_parse_file(char *);
struct sdrl_expr *sdrl_parse_string(char *, int);
struct sdrl_expr *sdrl_parse_input(struct sdrl_input *);
struct sdrl_expr *sdrl_parse_expr(struct sdrl_input *);

int sdrl_add_parse_rule(char *, char *, struct sdrl_expr *);

#endif

