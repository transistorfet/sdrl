/*
 * Function Name:	tostring.c
 * Module Requirements:	string type
 * Description:		Convert numbers and strings to a single string
 */

#include <string.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/string.h>

/**
 * Args:	<string>|<number>, ...
 * Description:	Returns a string corresponding to the concatenation of the
 *		given numbers representing ASCII character and given strings.
 */
int sdrl_string_tostring(sdMachine *mach, sdArray *args)
{
	int i, j = 0;
	char buffer[SDRL_STRING_SIZE];

	for (i = 1; i <= args->last; i++) {
		if (sdrl_value_isa(args->items[1], &sdNumberTypeDef))
			buffer[j++] = (unsigned char) SDNUMBER(args->items[i])->num;
		else if (sdrl_value_isa(args->items[1], &sdStringTypeDef)) {
			strncpy(&buffer[j], SDSTRING(args->items[i])->str, SDRL_STRING_SIZE - j - 1);
			j += SDSTRING(args->items[i])->len;
		}
		else
			return(sdrl_set_type_error(mach, NULL, args->items[i]->type));
		if (j >= SDRL_STRING_SIZE - 1)
			break;
	}
	if (j >= SDRL_STRING_SIZE)
		j = SDRL_STRING_SIZE - 1;
	buffer[j] = '\0';
	mach->ret = SDVALUE(sdrl_make_string(mach, &sdStringTypeDef, buffer, j));
	return(0);
}



