/*
 * Name:	parse.c
 * Version:	0.2
 * Description:	Parser
 */

#include <stdlib.h>

#include "expr.h"
#include "input.h"
#include "parse.h"
#include "globals.h"


/**
 * Parse the input file and return the expression tree
 */
struct sdrl_expr *sdrl_parse_file(char *filename, sdrl_parser_t parser, void *param)
{
	struct sdrl_expr *expr;
	struct sdrl_input *input;

	if (!(input = sdrl_create_input()))
		return(NULL);
	if (sdrl_add_file(input, filename)) {
		sdrl_destroy_input(input);
		return(NULL);
	}

	expr = parser(input, param);

	sdrl_destroy_input(input);
	return(expr);
}

/**
 * Parse the input string and return the expression tree
 */
struct sdrl_expr *sdrl_parse_input_string(char *str, int size, sdrl_parser_t parser, void *param)
{
	struct sdrl_expr *expr;
	struct sdrl_input *input;

	if (!(input = sdrl_create_input()))
		return(NULL);
	if (sdrl_add_string(input, str, size)) {
		sdrl_destroy_input(input);
		return(NULL);
	}

	expr = parser(input, param);

	sdrl_destroy_input(input);
	return(expr);
}


