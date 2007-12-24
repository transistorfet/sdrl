/*
 * Builtin Name:	tail.c
 * Module Requirements:	list type
 * Description:		Tail Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the all of the values passed to it following the first value.
 */
int sdrl_base_tail(struct sdrl_machine *mach, struct sdrl_value *value)
{
	if (value && value->next)
		mach->ret = SDRL_MAKE_REFERENCE(value->next);
	else
		mach->ret = NULL;
	return(0);
}





