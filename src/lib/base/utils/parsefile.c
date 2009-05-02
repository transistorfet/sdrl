/*
 * Module Name:	parsefile.c
 * Description:	Parse a File
 */

#include <stdio.h>
#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/base/input.h>

/**
 * Parse a file and return an expression given its name and the parser to use.
 */
sdExpr *sdrl_base_parse_file(sdMachine *mach, const char *filename)
{
	sdExpr *expr;
	sdInput *input;

	if (!mach->parser)
		return(NULL);
	if (!(input = sdrl_make_input()))
		return(NULL);
	if (sdrl_add_file(input, filename)) {
		sdrl_input_destroy(input);
		return(NULL);
	}
	expr = mach->parser(mach, input);
	sdrl_input_destroy(input);
	if (mach->preproc)
		expr = mach->preproc(mach, expr);
	return(expr);
}


