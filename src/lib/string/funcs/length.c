/*
 * Builtin Name:	length.c
 * Module Requirements:	string type
 * Description:		String Length Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>

/**
 * Args:	<value>
 * Description:	Returns the length of the given string.
 */
int sdrl_string_length(struct sdrl_machine *mach, struct sdrl_value *value)
{
	number_t len = 0;
	struct sdrl_type *number, *string;

	if (!value && (value && value->next))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(number = sdrl_find_binding(mach->type_env, "number")) || !(string = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (value->type != string)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	len = strlen(value->data.str);
	mach->ret = sdrl_make_value(mach->heap, number, (sdrl_data_t) len, 0, NULL);
	return(0);
}



