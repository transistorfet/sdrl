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
	struct sdrl_value *str, *index;
	struct sdrl_type *num_type, *str_type;

	if (!(num_type = sdrl_find_binding(mach->type_env, "number"))
	    || !(str_type = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!(str = sdrl_next_arg_checked(mach, &value, str_type))
	    || !(index = sdrl_next_arg_checked(mach, &value, num_type)))
		return(mach->error->err);

	if ((index->data.num >= 0) && (index->data.num < strlen(str->data.str)))
		ret = str->data.str[ (int) index->data.num ];
	mach->ret = sdrl_make_value(mach->heap, num_type, (sdrl_data_t) ret, 0, NULL);
	return(0);
}



