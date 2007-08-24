/*
 * Builtin Name:	defenv.c
 * Module Requirements:	list type
 * Description:		Define Environment Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns a value pointing to a newly created environment.
 */
int sdrl_base_defenv(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_type *type;
	struct sdrl_environment *env;

	if (!(type = sdrl_find_binding(mach->type_env, "env")))
		return(SDRL_ERR_NOT_FOUND);
	else if (type->create && (env = type->create(mach, value))) {
		mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) (void *) env, 0, NULL);
		return(0);
	}
	return(-1);
}




