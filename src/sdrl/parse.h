/*
 * Name:	parse.h
 * Version:	0.1
 * Description:	Parser Header
 */


#ifndef PARSE_H
#define PARSE_H

#include "expr.h"
#include "input.h"
#include "types.h"

struct sdrl_expr *sdrl_parse_file(char *);
struct sdrl_expr *sdrl_parse_input(struct sdrl_input *);

#endif

