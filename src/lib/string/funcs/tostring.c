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
int sdrl_string_tostring(struct sdrl_machine *mach, struct sdrl_value *args)
{
	int i = 0;
	struct sdrl_value *cur;
	char buffer[SDRL_STRING_SIZE];
	struct sdrl_type *str_type;

	if (!(str_type = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));

	for (cur = args; cur; cur = cur->next) {
		if (cur->type->basetype == SDRL_BT_NUMBER)
			buffer[i++] = (unsigned char) SDRL_NUMBER(cur)->num;
		else if (cur->type->basetype == SDRL_BT_STRING) {
			strncpy(&buffer[i], SDRL_STRING(cur)->str, SDRL_STRING_SIZE - i - 1);
			i += SDRL_STRING(cur)->len;
		}
		else
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (i >= SDRL_STRING_SIZE - 1)
			break;
	}
	if (i >= SDRL_STRING_SIZE)
		i = SDRL_STRING_SIZE - 1;
	buffer[i] = '\0';
	mach->ret = sdrl_make_string(mach->heap, str_type, buffer, i);
	return(0);
}



