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
int sdrl_string_substr(sdMachine *mach, sdValue *args)
{
	int i = 0, j, len;
	sdString *str;
	char buffer[SDRL_STRING_SIZE];
	sdNumber *from, *to;

	if (!args)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(str = SDSTRING(sdrl_next_arg(mach, &args, SDRL_BT_STRING, NULL)))
	    || !(from = SDNUMBER(sdrl_next_arg(mach, &args, SDRL_BT_NUMBER, NULL)))
	    || (!(to = SDNUMBER(sdrl_next_optional_arg(mach, &args, SDRL_BT_NUMBER, NULL))) && args))
		return(mach->error->err);

	len = str->len;
	j = (int) from->num;
	if (j < 0)
		j = 0;
	else if (j >= len)
		j = len;
	if (to && ((j + (int) to->num) < len))
		len = j + (int) to->num;
	for (;(i < SDRL_STRING_SIZE) && (j < len);i++, j++)
		buffer[i] = str->str[j];
	buffer[i] = '\0';
	mach->ret = sdrl_make_string(mach->heap, SDVALUE(str)->type, buffer, i);
	return(0);
}



