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
int sdrl_load_base(struct sdrl_machine *mach)
{
	struct sdrl_type *form, *func;

	sdrl_base_load_expr_type(mach, "expr");
	sdrl_base_load_form_type(mach, "form");
	sdrl_base_load_func_type(mach, "func");
	sdrl_base_load_list_type(mach, "list");
	sdrl_base_load_env_type(mach, "env");

	if (!(form = sdrl_find_binding(mach->type_env, "form")))
		return(-1);
	if (!(func = sdrl_find_binding(mach->type_env, "func")))
		return(-1);

	SDRL_BIND_FUNCTION(mach, form, "code", sdrl_base_code);
	SDRL_BIND_FUNCTION(mach, func, "set", sdrl_base_set);
	SDRL_BIND_FUNCTION(mach, func, "if", sdrl_base_if);

	SDRL_BIND_FUNCTION(mach, func, "$", sdrl_base_resolve);
	SDRL_BIND_FUNCTION(mach, func, "%", sdrl_base_lookup);
	SDRL_BIND_FUNCTION(mach, func, "@", sdrl_base_array);
	SDRL_BIND_FUNCTION(mach, func, "list", sdrl_base_list);
	SDRL_BIND_FUNCTION(mach, func, "unlist", sdrl_base_unlist);
	SDRL_BIND_FUNCTION(mach, func, "head", sdrl_base_head);
	SDRL_BIND_FUNCTION(mach, func, "tail", sdrl_base_tail);
	SDRL_BIND_FUNCTION(mach, func, "null", sdrl_base_null);

	SDRL_BIND_FUNCTION(mach, func, "+", sdrl_base_add);
	SDRL_BIND_FUNCTION(mach, func, "-", sdrl_base_subtract);
	SDRL_BIND_FUNCTION(mach, func, "*", sdrl_base_multiply);
	SDRL_BIND_FUNCTION(mach, func, "/", sdrl_base_divide);

	SDRL_BIND_FUNCTION(mach, func, "=", sdrl_base_equals);
	SDRL_BIND_FUNCTION(mach, func, "!=", sdrl_base_not_equals);
	SDRL_BIND_FUNCTION(mach, func, ">", sdrl_base_greater_than);
	SDRL_BIND_FUNCTION(mach, func, "<", sdrl_base_less_than);
	SDRL_BIND_FUNCTION(mach, func, ">=", sdrl_base_greater_than_equals);
	SDRL_BIND_FUNCTION(mach, func, "<=", sdrl_base_less_than_equals);

	SDRL_BIND_FUNCTION(mach, func, "and", sdrl_base_and);
	SDRL_BIND_FUNCTION(mach, func, "or", sdrl_base_or);
	SDRL_BIND_FUNCTION(mach, func, "not", sdrl_base_not);

	SDRL_BIND_FUNCTION(mach, func, "defenv", sdrl_base_defenv);

	//SDRL_BIND_FUNCTION(mach, func, "print", sdrl_base_print);

	return(0);
}


