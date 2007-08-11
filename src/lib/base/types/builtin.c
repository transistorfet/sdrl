/*
 * Builtin Name:	builtin.c
 * Module Requirements:	(none)
 * Description:		Builtin (C Function) Type
 */

#include <sdrl/sdrl.h>

static int sdrl_evaluate_builtin_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

int sdrl_base_load_builtin_type(struct sdrl_machine *mach, char *name)
{
	struct sdrl_type *builtin;

	builtin =  sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER, NULL, (sdrl_evaluate_t) sdrl_evaluate_builtin_type, NULL, NULL);
	return(sdrl_add_binding(mach->type_env, name, builtin));
}

/*** Local Functions ***/

/**
 * Evaluate function for the builtin type.
 */
static int sdrl_evaluate_builtin_type(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_value *args)
{
	sdrl_destroy_value(mach->heap, mach->ret);
	mach->ret = NULL;
	return(((int (*)(struct sdrl_machine *, struct sdrl_value *)) func->data.ptr)(mach, args));
}


