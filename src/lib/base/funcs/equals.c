/*
 * Function Name:	equals.c
 * Module Requirements:	number type
 * Description:		Equals Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if all number values are equal to eachother, 0 otherwise.
 */
int sdrl_base_equals(sdMachine *mach, sdArray *args)
{
	int i;
	number_t comp, num, result = 1;

/*
	if (!args)
		return(sdrl_set_args_error(mach));
	if (args->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_type_error(mach));
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type != args->type)
			return(sdrl_set_type_error(mach));
		if (SDRL_NUMBER(cur)->num != SDNUMBER(args)->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
*/

	SDRL_TRY(comp = sdrl_check_number(mach, args->items[1], SDRL_BT_NUMBER, NULL));
	for (i = 1; i <= args->last; i++) {
		SDRL_TRY(num = sdrl_check_number(mach, args->items[i], SDRL_BT_NUMBER, NULL));
		if (comp != num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);

    FAIL:
	return(mach->error->err);
}


