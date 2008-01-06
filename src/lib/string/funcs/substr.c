/*
 * Builtin Name:	substr.c
 * Module Requirements:	string type
 * Description:		Substring Extraction Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/string.h>

/**
 * Args:	<string>, <number>, [<number>]
 * Description:	Returns a string corresponding to the substring starting at
 *		the given offset and either of a length, if given, or to
 *		the end of the string otherwise.
 */
int sdrl_string_substr(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int i = 0, j, len;
	char buffer[STRING_SIZE];
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

	len = strlen(str->data.str);
	j = (int) from->data.num;
	if (j < 0)
		j = 0;
	else if (j >= len)
		j = len;
	if (to && ((j + (int) to->data.num) < len))
		len = j + (int) to->data.num;
	for (;(i < STRING_SIZE) && (j < len);i++, j++)
		buffer[i] = str->data.str[j];
	buffer[i] = '\0';
	mach->ret = sdrl_make_value(mach->heap, str_type, (sdrl_data_t) buffer, i, NULL);
	return(0);
}



