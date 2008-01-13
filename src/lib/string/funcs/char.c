/*
 * Function Name:	char.c
 * Module Requirements:	string type
 * Description:		Character Extraction Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<string>, <number>
 * Description:	Returns a number corresponding to the ASCII value of the
 *		character at the given position indexed from 0 of the given
 *		string.
 */
int sdrl_string_char(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t ret = 0;
	struct sdrl_string *str;
	struct sdrl_number *index;
	struct sdrl_type *num_type, *str_type;

	if (!(num_type = sdrl_find_binding(mach->type_env, "number"))
	    || !(str_type = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!(str = (struct sdrl_string *) sdrl_next_arg_checked(mach, &args, str_type))
	    || !(index = (struct sdrl_number *) sdrl_next_arg_checked(mach, &args, num_type)))
		return(mach->error->err);

	if ((index->num >= 0) && (index->num < str->len))
		ret = str->str[ (int) index->num ];
	mach->ret = sdrl_make_number(mach->heap, num_type, ret);
	return(0);
}



