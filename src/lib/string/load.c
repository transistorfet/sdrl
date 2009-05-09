/*
 * Loader Name:		load.c
 * Module Requirements:	libstring
 * Description:		String Library Loader
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/string.h>

/**
 * Load all of the forms, funcs and types into the given machine.
 */
int sdrl_load_string(sdMachine *mach, sdEnv *namespace)
{
	if (!namespace) {
		// TODO create namespace
	}

	SDRL_BIND_FUNCTION(mach, namespace, &sdFuncTypeDef, "char", sdrl_string_char);
	SDRL_BIND_FUNCTION(mach, namespace, &sdFuncTypeDef, "compare", sdrl_string_compare);
	SDRL_BIND_FUNCTION(mach, namespace, &sdFuncTypeDef, "length", sdrl_string_length);
	SDRL_BIND_FUNCTION(mach, namespace, &sdFuncTypeDef, "substr", sdrl_string_substr);
	SDRL_BIND_FUNCTION(mach, namespace, &sdFuncTypeDef, "tostring", sdrl_string_tostring);

	return(0);
}


