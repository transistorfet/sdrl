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
	sdType *str_type;

	if (!(str_type = sdrl_env_find(mach->type_env, "string")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));

	for (i = 1; i <= args->last; i++) {
		if (args->items[i]->type->basetype == SDRL_BT_NUMBER)
			buffer[j++] = (unsigned char) SDNUMBER(args->items[i])->num;
		else if (args->items[i]->type->basetype == SDRL_BT_STRING) {
			strncpy(&buffer[j], SDSTRING(args->items[i])->str, SDRL_STRING_SIZE - j - 1);
			j += SDSTRING(args->items[i])->len;
		}
		else
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (j >= SDRL_STRING_SIZE - 1)
			break;
	}
	if (j >= SDRL_STRING_SIZE)
		j = SDRL_STRING_SIZE - 1;
	buffer[j] = '\0';
	mach->ret = sdrl_make_string(mach->heap, str_type, buffer, j);
	return(0);
}



