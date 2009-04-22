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
int sdrl_string_substr(sdMachine *mach, sdArray *args)
{
	int i = 0, j, len;
	char buffer[SDRL_STRING_SIZE];

	if ((args->last < 2 || args->last > 3)
	    || args->items[1]->type->basetype != SDRL_BT_STRING
	    || args->items[2]->type->basetype != SDRL_BT_NUMBER
	    || (args->last == 3 && args->items[3]->type->basetype != SDRL_BT_NUMBER))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));

	len = SDSTRING(args->items[1])->len;
	j = (int) SDNUMBER(args->items[2])->num;
	if (j < 0)
		j = 0;
	else if (j >= len)
		j = len;
	if (args->last == 3 && ((j + (int) SDNUMBER(args->items[3])->num) < len))
		len = j + (int) SDNUMBER(args->items[3])->num;
	for (;(i < SDRL_STRING_SIZE) && (j < len);i++, j++)
		buffer[i] = SDSTRING(args->items[1])->str[j];
	buffer[i] = '\0';
	mach->ret = sdrl_make_string(mach->heap, args->items[1]->type, buffer, i);
	return(0);
}



