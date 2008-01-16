/*
 * Type Name:		list.c
 * Module Requirements:	(none)
 * Description:		List Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

struct sdrl_type *sdrl_base_make_list_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		sizeof(struct sdrl_reference),
		0,
		SDRL_BT_REFERENCE,
		(sdrl_create_t) sdrl_base_create_list,
		(sdrl_destroy_t) sdrl_heap_free,
		(sdrl_duplicate_t) sdrl_duplicate_reference,
		NULL
	));
}

struct sdrl_value *sdrl_base_create_list(struct sdrl_machine *mach, struct sdrl_type *type, struct sdrl_value *args)
{
	return(sdrl_make_reference(mach->heap, type, args));
}

