/*
 * Builtin Name:	array.c
 * Module Requirements:	(none)
 * Description:		Array Creation Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the values passed to it (as a single list).
 */
int sdrl_base_array(struct sdrl_machine *mach, struct sdrl_value *value)
{
	mach->ret = value;
	return(0);
}




