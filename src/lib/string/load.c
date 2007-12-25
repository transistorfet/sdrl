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
int sdrl_load_string(struct sdrl_machine *mach)
{
	struct sdrl_type *func;

	if (!(func = sdrl_find_binding(mach->type_env, "func")))
		return(-1);

	SDRL_BIND_FUNCTION(mach, func, "length", sdrl_string_length);

	return(0);
}


