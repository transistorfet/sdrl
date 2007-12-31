/*
 * Builtin Name:	substr.c
 * Module Requirements:	string type
 * Description:		Substring Extraction Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>

/**
 * Args:	<string>, <number>, [<number>]
 * Description:	Returns a number corresponding to the ASCII value of the
 *		character at the given position indexed from 0 of the given
 *		string.
 */
int sdrl_string_substr(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_value *str, *from, *to;
	struct sdrl_type *num_type, *str_type;

	if (!value)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(num_type = sdrl_find_binding(mach->type_env, "number"))
	    || !(str_type = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!(str = sdrl_next_arg_checked(mach, &value, str_type))
	    || !(from = sdrl_next_arg_checked(mach, &value, num_type))
	    || (!(to = sdrl_next_arg_optional(mach, &value, num_type)) && value))
		return(mach->error->err);
	// TODO you need a function sdrl_next_arg_optional but how will you make it easy to check for a
	//	failed condition?  you can't check the return because that might be NULL.  You could assume
	//	that if there is an error then it must have been our error.  Perhaps you should make a point
	//	of keeping the mach->error cleared (in the machine) unless the error is handled somehow so
	//	that we can make this assumption?

/*
	if ((value->next->data.num >= 0) && (value->next->data.num < strlen(value->data.str)))
		ret = value->data.str[ (int) value->next->data.num ];
	mach->ret = sdrl_make_value(mach->heap, number, (sdrl_data_t) ret, 0, NULL);
*/
	return(0);
}



