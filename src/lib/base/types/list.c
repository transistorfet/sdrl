/*
 * Type Name:		list.c
 * Module Requirements:	(none)
 * Description:		List Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType *sdrl_base_make_list_type(sdMachine *mach)
{
	return(sdrl_make_type(
		sizeof(sdReference),
		0,
		SDRL_BT_REFERENCE,
		(sdrl_create_t) sdrl_base_create_list,
		(sdrl_destroy_t) sdrl_heap_free,
		(sdrl_duplicate_t) sdrl_duplicate_reference,
		NULL
	));
}

sdValue *sdrl_base_create_list(sdMachine *mach, sdType *type, sdArray *args)
{
	if (args->last != 0)
		return(NULL);
	return(sdrl_make_reference(mach->heap, type, args->items[0]));
}

