/*
 * Function Name:	print.c
 * Module Requirements:	stdio.h
 * Description:		Print Expression
 */

#include <stdio.h>

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Prints out each of the values in the list and returns the list.
 */
int sdrl_io_print(sdMachine *mach, sdArray *args)
{
	int i;

	for (i = 1; i <= args->last; i++) {
		if (!args->items[i])
			continue;
		switch (args->items[i]->type->basetype) {
			case SDRL_BT_NUMBER:
				printf("%f", SDNUMBER(args->items[i])->num);
				break;
			case SDRL_BT_STRING:
				printf("%s", SDSTRING(args->items[i])->str);
				break;
			case SDRL_BT_POINTER:
				printf("0x%x", (unsigned int) SDPOINTER(args->items[i])->ptr);
				break;
			case SDRL_BT_DATA:
				printf("0x%x", (unsigned int) args->items[i]);
				break;
			default:
				break;
		}
	}
	mach->ret = SDVALUE(SDRL_INCREF(args));
	return(0);
}


