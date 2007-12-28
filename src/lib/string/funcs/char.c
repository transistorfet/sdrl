/*
 * Builtin Name:	char.c
 * Module Requirements:	string type
 * Description:		Character Extraction Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>

/**
 * Args:	<string>, <number>
 * Description:	Returns a number corresponding to the ASCII value of the
 *		character at the given position indexed from 0 of the given
 *		string.
 */
int sdrl_string_char(struct sdrl_machine *mach, struct sdrl_value *value)
{
	number_t ret = 0;
	struct sdrl_type *number, *string;

	if (!(value && value->next))
		return(SDRL_ERR_INVALID_PARAMS);
	if (!(number = sdrl_find_binding(mach->type_env, "number")) || !(string = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERR_NOT_FOUND);
	if ((value->type != string) || (value->next->type != number))
		return(SDRL_ERR_INVALID_TYPE);
	if ((value->next->data.number >= 0) && (value->next->data.number < strlen(value->data.str)))
		ret = value->data.str[ (int) value->next->data.number ];
	mach->ret = sdrl_make_value(mach->heap, number, (sdrl_data_t) ret, 0, NULL);
	return(0);
}



