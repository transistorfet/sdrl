/*
 * Function Name:	compare.c
 * Module Requirements:	string type
 * Description:		Compare String Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/string.h>

/**
 * Args:	<string>, <string>, ...
 * Description:	Returns 1 if the given strings match exactly or 0 otherwise.
 */
int sdrl_string_compare(sdMachine *mach, sdArray *args)
{
	int i;

	if (args->last < 2 || !sdrl_value_isa(args->items[1], &sdStringTypeDef))
		return(sdrl_set_args_error(mach));
	for (i = 2; i <= args->last; i++) {
		if (!sdrl_value_isa(args->items[i], &sdStringTypeDef))
			return(sdrl_set_type_error(mach, &sdStringTypeDef, args->items[i]->type));
		if ((SDSTRING(args->items[1])->len != SDSTRING(args->items[i])->len)
		    || strncmp(SDSTRING(args->items[1])->str, SDSTRING(args->items[i])->str, SDSTRING(args->items[1])->len)) {
			mach->ret = SDVALUE(sdrl_make_number(mach, &sdNumberTypeDef, 0));
			return(0);
		}
	}
	mach->ret = SDVALUE(sdrl_make_number(mach, &sdNumberTypeDef, 1));
	return(0);
}



