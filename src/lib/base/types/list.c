/*
 * Builtin Name:	list.c
 * Module Requirements:	(none)
 * Description:		List Type
 */

#include <sdrl/sdrl.h>


struct sdrl_type *sdrl_base_make_list_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		mach->heap,
		0,
		SDRL_BT_POINTER,
		NULL,
		NULL,
		(sdrl_duplicate_t) sdrl_duplicate_value,
		(sdrl_destroy_t) sdrl_destroy_value
	));
}

