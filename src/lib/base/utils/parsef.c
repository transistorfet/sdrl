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
sdExpr *sdrl_base_parse_file(sdMachine *mach, sdrl_parser_t parser, const char *filename)
{
	sdExpr *expr;
	sdInput *input;

	if (!(input = sdrl_create_input()))
		return(NULL);
	if (sdrl_add_file(input, filename)) {
		sdrl_destroy_input(input);
		return(NULL);
	}
	expr = parser(mach, input);
	sdrl_destroy_input(input);
	return(expr);
}


