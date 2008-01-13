/*
 * Function Name:	substr.c
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
int sdrl_string_substr(struct sdrl_machine *mach, struct sdrl_value *args)
{
	int i = 0, j, len;
	struct sdrl_string *str;
	char buffer[STRING_SIZE];
	struct sdrl_number *from, *to;
	struct sdrl_type *num_type, *str_type;

	if (!args)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(num_type = sdrl_find_binding(mach->type_env, "number"))
	    || !(str_type = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!(str = (struct sdrl_string *) sdrl_next_arg_checked(mach, &args, str_type))
	    || !(from = (struct sdrl_number *) sdrl_next_arg_checked(mach, &args, num_type))
	    || (!(to = (struct sdrl_number *) sdrl_next_arg_optional(mach, &args, num_type)) && args))
		return(mach->error->err);

	len = str->len;
	j = (int) from->num;
	if (j < 0)
		j = 0;
	else if (j >= len)
		j = len;
	if (to && ((j + (int) to->num) < len))
		len = j + (int) to->num;
	for (;(i < STRING_SIZE) && (j < len);i++, j++)
		buffer[i] = str->str[j];
	buffer[i] = '\0';
	mach->ret = sdrl_make_string(mach->heap, str_type, buffer, i);
	return(0);
}



