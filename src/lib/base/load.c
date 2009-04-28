/*
 * Loader Name:		load.c
 * Module Requirements:	libbase
 * Description:		Base Library Loader
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Load all of the forms, funcs and types of the base library into the given machine.
 */
int sdrl_load_base(sdMachine *mach)
{
	sdrl_env_add(mach->type_env, "dynblock", &sdDynBlockTypeDef);
	sdrl_env_add(mach->type_env, "form", &sdFormTypeDef);
	sdrl_env_add(mach->type_env, "func", &sdFuncTypeDef);
	sdrl_env_add(mach->type_env, "lexblock", &sdLexBlockTypeDef);
	sdrl_env_add(mach->type_env, "list", &sdListTypeDef);

	/** Bind forms */
	SDRL_BIND_FUNCTION(mach, &sdFormTypeDef, "code", sdrl_base_code);
	SDRL_BIND_FUNCTION(mach, &sdFormTypeDef, "dynblock", sdrl_base_dynblock);
	SDRL_BIND_FUNCTION(mach, &sdFormTypeDef, "lexblock", sdrl_base_lexblock);

	/** Bind functions */
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "set", sdrl_base_set);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "setlist", sdrl_base_setlist);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "if", sdrl_base_if);

	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "$", sdrl_base_resolve);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "%", sdrl_base_lookup);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "@", sdrl_base_array);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "list", sdrl_base_list);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "head", sdrl_base_head);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "tail", sdrl_base_tail);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "null", sdrl_base_null);

	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "+", sdrl_base_add);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "-", sdrl_base_subtract);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "*", sdrl_base_multiply);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "/", sdrl_base_divide);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "mod", sdrl_base_modulo);

	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "=", sdrl_base_equals);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "!=", sdrl_base_not_equals);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, ">", sdrl_base_greater_than);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "<", sdrl_base_less_than);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, ">=", sdrl_base_greater_than_equals);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "<=", sdrl_base_less_than_equals);

	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "and", sdrl_base_and);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "or", sdrl_base_or);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "not", sdrl_base_not);

	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "new", sdrl_base_new);
	SDRL_BIND_FUNCTION(mach, &sdFuncTypeDef, "defenv", sdrl_base_defenv);

	return(0);
}


