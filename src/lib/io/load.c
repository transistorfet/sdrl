/*
 * Loader Name:		load.c
 * Module Requirements:	libio
 * Description:		I/O Library Loader
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>

/**
 * Load all of the forms, builtins and types of the base library into the given machine.
 */
int sdrl_load_io(struct sdrl_machine *mach)
{
	struct sdrl_type *builtin;

	if (!(builtin = sdrl_find_binding(mach->type_env, "builtin")))
		return(-1);

	sdrl_bind_function_m(mach, builtin, "print", sdrl_io_print);

	sdrl_bind_function_m(mach, builtin, "open", sdrl_io_open);

	return(0);
}


