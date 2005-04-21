/*
 * Builtin Name:	head.c
 * Version:		0.1
 * Module Requirements:	list type
 * Description:		Head Expression
 */

#include <sdrl/sdrl.h>

/**
 * head(<value>, ...)
 * Returns the first value passed to it.
 */
int sdrl_base_head(struct sdrl_machine *mach, struct sdrl_value *value)
{
	if (value)
		mach->ret = sdrl_shift_value(&value);
	sdrl_destroy_value(mach->heap, value);
	return(0);
}




