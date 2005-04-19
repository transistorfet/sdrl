/*
 * Builtin Name:	tail.c
 * Version:		0.1
 * Module Requirements:	list type
 * Description:		Tail Expression
 */

#include <sdrl.h>

/**
 * tail(<value>, ...)
 * Returns the all of the values passed to it following the first value.
 */
int sdrl_base_tail(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_value *tmp = NULL;

	if (value)
		tmp = sdrl_shift_value(&value);
	mach->ret = value;
	sdrl_destroy_value(mach->heap, tmp);
	return(0);
}





