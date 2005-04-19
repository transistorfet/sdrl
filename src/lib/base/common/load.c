/*
 * Loader Name:		load.c
 * Version:		0.1
 * Module Requirements:	libbase
 * Description:		Base Library Loader
 */

#include <sdrl.h>
#include <lib/base.h>

/**
 * Load all of the forms, builtins and types of the base library into the given machine.
 */
int sdrl_load_base(struct sdrl_machine *mach)
{
	struct sdrl_type *form, *builtin;

	sdrl_base_load_expr_type(mach);
	sdrl_base_load_form_type(mach);
	sdrl_base_load_builtin_type(mach);
	sdrl_base_load_list_type(mach);

	if (!(form = sdrl_find_binding(mach->type_env, "form")))
		return(-1);
	if (!(builtin = sdrl_find_binding(mach->type_env, "builtin")))
		return(-1);

	sdrl_bind_function_m(mach, form, "code", sdrl_base_code);
	sdrl_bind_function_m(mach, builtin, "set", sdrl_base_set);
	sdrl_bind_function_m(mach, builtin, "if", sdrl_base_if);

	sdrl_bind_function_m(mach, builtin, "$", sdrl_base_resolve);
	sdrl_bind_function_m(mach, builtin, "@", sdrl_base_array);
	sdrl_bind_function_m(mach, builtin, "list", sdrl_base_list);
	sdrl_bind_function_m(mach, builtin, "unlist", sdrl_base_unlist);
	sdrl_bind_function_m(mach, builtin, "head", sdrl_base_head);
	sdrl_bind_function_m(mach, builtin, "tail", sdrl_base_tail);
	sdrl_bind_function_m(mach, builtin, "null", sdrl_base_null);

	sdrl_bind_function_m(mach, builtin, "+", sdrl_base_add);
	sdrl_bind_function_m(mach, builtin, "-", sdrl_base_subtract);
	sdrl_bind_function_m(mach, builtin, "*", sdrl_base_multiply);
	sdrl_bind_function_m(mach, builtin, "/", sdrl_base_divide);

	sdrl_bind_function_m(mach, builtin, "=", sdrl_base_equals);
	sdrl_bind_function_m(mach, builtin, "!=", sdrl_base_not_equals);
	sdrl_bind_function_m(mach, builtin, ">", sdrl_base_greater_than);
	sdrl_bind_function_m(mach, builtin, "<", sdrl_base_less_than);
	sdrl_bind_function_m(mach, builtin, ">=", sdrl_base_greater_than_equals);
	sdrl_bind_function_m(mach, builtin, "<=", sdrl_base_less_than_equals);

	sdrl_bind_function_m(mach, builtin, "and", sdrl_base_and);
	sdrl_bind_function_m(mach, builtin, "or", sdrl_base_or);
	sdrl_bind_function_m(mach, builtin, "not", sdrl_base_not);

	sdrl_bind_function_m(mach, builtin, "print", sdrl_base_print);

	return(0);
}


