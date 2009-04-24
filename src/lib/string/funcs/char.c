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
int sdrl_string_char(sdMachine *mach, sdArray *args)
{
	number_t ret = 0;

	if (args->last != 2 || args->items[1]->type->basetype != SDRL_BT_STRING
	    || args->items[2]->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_args_error(mach));
	if ((SDNUMBER(args->items[2])->num >= 0) && (SDNUMBER(args->items[2])->num < SDSTRING(args->items[1])->len))
		ret = SDSTRING(args->items[1])->str[ (int) SDNUMBER(args->items[2])->num ];
	mach->ret = sdrl_make_number(mach->heap, args->items[2]->type, ret);
	return(0);
}



