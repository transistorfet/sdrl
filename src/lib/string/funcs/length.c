/*
 * Function Name:	length.c
 * Module Requirements:	string type
 * Description:		String Length Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>
 * Description:	Returns the length of the given string.
 */
int sdrl_string_length(sdMachine *mach, sdValue *args)
{
	sdString *str;
	sdType *num_type;

	if (!(num_type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!(str = SDSTRING(sdrl_next_arg(mach, &args, SDRL_BT_STRING, NULL))))
		return(mach->error->err);
	mach->ret = sdrl_make_number(mach->heap, num_type, (number_t) str->len);
	return(0);
}



