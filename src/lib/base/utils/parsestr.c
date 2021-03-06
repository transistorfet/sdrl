/*
 * Module Name:	parsestr.c
 * Description:	Parse a String
 */

#include <stdio.h>
#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/base/input.h>

/**
 * Parse a string and return an expression given its name and the parser to use.
 */
sdExpr *sdrl_base_parse_string(sdMachine *mach, const char *str, int size)
{
	sdExpr *expr;
	sdInput *input;

	if (!mach->parser)
		return(NULL);
	if (!(input = sdrl_make_input()))
		return(NULL);
	if (sdrl_add_string(input, str, size)) {
		sdrl_input_destroy(input);
		return(NULL);
	}
	expr = mach->parser(mach, input);
	sdrl_input_destroy(input);
	if (mach->preproc)
		expr = mach->preproc(mach, expr);
	return(expr);
}


