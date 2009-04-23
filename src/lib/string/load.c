/*
 * Loader Name:		load.c
 * Module Requirements:	libstring
 * Description:		String Library Loader
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/string.h>

/**
 * Load all of the forms, funcs and types into the given machine.
 */
int sdrl_load_string(sdMachine *mach)
{
	sdType *func;

	if (!(func = sdrl_env_find(mach->type_env, "func")))
		return(-1);

	SDRL_BIND_FUNCTION(mach, func, "char", sdrl_string_char);
	SDRL_BIND_FUNCTION(mach, func, "length", sdrl_string_length);
	SDRL_BIND_FUNCTION(mach, func, "substr", sdrl_string_substr);
	SDRL_BIND_FUNCTION(mach, func, "tostring", sdrl_string_tostring);

	return(0);
}


