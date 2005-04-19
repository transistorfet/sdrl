/*
 * Builtin Name:	form.c
 * Version:		0.1
 * Module Requirements:	(none)
 * Description:		Form (C Form) Type
 */

#include <sdrl.h>

static int sdrl_evaluate_form_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_expr *);

int sdrl_base_load_form_type(struct sdrl_machine *mach)
{
	struct sdrl_type *form;

	form = sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER | SDRL_TBF_PASS_EXPRS, (sdrl_evaluate_t) sdrl_evaluate_form_type, NULL, NULL);	
	return(sdrl_add_binding(mach->type_env, "form", form));
}

/*** Local Functions ***/

/**
 * Evaluate function for the form type.
 */
static int sdrl_evaluate_form_type(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_expr *params)
{
	sdrl_destroy_value(mach->heap, mach->ret);
	mach->ret = NULL;
	return(((int (*)(struct sdrl_machine *, struct sdrl_expr *)) func->data.ptr)(mach, params));
}

