/*
 * Loader Name:		load.c
 * Module Requirements:	libio
 * Description:		I/O Library Loader
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>
#include <sdrl/lib/base.h>

/**
 * Load all of the forms, funcs and types of the base library into the given machine.
 */
int sdrl_load_io(sdMachine *mach)
{

	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "print", sdrl_io_print);

	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "open", sdrl_io_open);

	return(0);
}


