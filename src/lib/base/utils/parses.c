/*
 * Module Name:	parses.c
 * Description:	Parse a String
 */

#include <stdio.h>
#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/base/input.h>

/**
 * Parse a string and return an expression given its name and the parser to use.
 */
struct sdrl_expr *sdrl_base_parse_string(struct sdrl_machine *mach, sdrl_parser_t parser, const char *str, int size)
{
	struct sdrl_expr *expr;
	struct sdrl_input *input;

	if (!(input = sdrl_create_input()))
		return(NULL);
	if (sdrl_add_string(input, str, size)) {
		sdrl_destroy_input(input);
		return(NULL);
	}
	expr = parser(mach, input);
	sdrl_destroy_input(input);
	return(expr);
}


