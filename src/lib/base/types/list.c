/*
 * Builtin Name:	list.c
 * Version:		0.1
 * Module Requirements:	(none)
 * Description:		List Type
 */

#include <sdrl/sdrl.h>


int sdrl_base_load_list_type(struct sdrl_machine *mach, char *name)
{
	struct sdrl_type *list;

	list = sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER, NULL, NULL, (sdrl_duplicate_t) sdrl_duplicate_value, (sdrl_destroy_t) sdrl_destroy_value);
	return(sdrl_add_binding(mach->type_env, name, list));
}

