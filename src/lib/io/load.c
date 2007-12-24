/*
 * Loader Name:		load.c
 * Module Requirements:	libio
 * Description:		I/O Library Loader
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>

/**
 * Load all of the forms, funcs and types of the base library into the given machine.
 */
int sdrl_load_io(struct sdrl_machine *mach)
{
	struct sdrl_type *func;

	if (!(func = sdrl_find_binding(mach->type_env, "func")))
		return(-1);

	SDRL_BIND_FUNCTION(mach, func, "print", sdrl_io_print);

	SDRL_BIND_FUNCTION(mach, func, "open", sdrl_io_open);

	return(0);
}


