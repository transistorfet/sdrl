/*
 * Builtin Name:	array.c
 * Version:		0.1
 * Module Requirements:	(none)
 * Description:		Array Creation Expression
 */

#include <sdrl.h>

/**
 * @(<value>, ...)
 * Returns the values passed to it (as a single list).
 */
int sdrl_base_array(struct sdrl_machine *mach, struct sdrl_value *value)
{
	mach->ret = value;
	return(0);
}




