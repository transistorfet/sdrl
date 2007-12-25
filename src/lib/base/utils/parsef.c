/*
 * Module Name:	parsef.c
 * Description:	Parse a File
 */

#include <stdio.h>
#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/base/input.h>

/**
 * Parse a file and return an expression given its name and the parser to use.
 */
struct sdrl_expr *sdrl_base_parse_file(const char *filename, sdrl_parser_t parser, void *param)
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


