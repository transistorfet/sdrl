/*
 * Form Name:		lexblock.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as an lexblock value
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type lexblock containing the code passed to it.
 */
int sdrl_base_lexblock(sdMachine *mach, sdArray *args)
{
	sdValue *func;

	// Remove the function value from the args
	func = sdrl_array_shift(args);
	SDRL_DECREF(func);
	if (sdLexBlockTypeDef.create)
		mach->ret = sdLexBlockTypeDef.create(mach, &sdLexBlockTypeDef, args);
	return(0);
}

