/*
 * Form Name:		lblock.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as an lblock value
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type lblock containing the code passed to it.
 */
int sdrl_base_lblock(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	void *ptr;
	struct sdrl_type *type;
	struct sdrl_value *value;

	if (!(type = sdrl_find_binding(mach->type_env, "lblock")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!(value = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "expr"), (sdrl_data_t) (void *) expr, 0, NULL)))
		return(SDRL_ERROR(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
	if (type->create && (ptr = type->create(mach, value)))
		mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) (void *) ptr, 0, NULL);
	sdrl_destroy_value(mach->heap, value);
	return(0);
}

