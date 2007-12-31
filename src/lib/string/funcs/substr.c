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
	number_t ret = 0;
	struct sdrl_type *number, *string;

/*
	// TODO this is just here to prevent trouble
	if (!value)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(number = sdrl_find_binding(mach->type_env, "number")) || !(string = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if ((value->type != string) || (value->next && value->next->type != number) || (value->next->next && value->next->next->type != number))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	if ((value->next->data.num >= 0) && (value->next->data.num < strlen(value->data.str)))
		ret = value->data.str[ (int) value->next->data.num ];
	mach->ret = sdrl_make_value(mach->heap, number, (sdrl_data_t) ret, 0, NULL);
*/
	return(0);
}



