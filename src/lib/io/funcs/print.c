/*
 * Function Name:	print.c
 * Module Requirements:	stdio.h
 * Description:		Print Expression
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>

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
		else if (sdrl_value_isa(args->items[i], &sdNumberTypeDef))
			printf("%f", SDNUMBER(args->items[i])->num);
		else if (sdrl_value_isa(args->items[i], &sdStringTypeDef))
			printf("%s", SDSTRING(args->items[i])->str);
		//else if (sdrl_value_isa(args->items[i], &sdPointerTypeDef))
		//	printf("0x%x", (unsigned int) SDPOINTER(args->items[i])->ptr);
		//else if (sdrl_value_isa(args->items[i], &sdDataTypeDef))
		//		printf("0x%x", (unsigned int) args->items[i]);
	}
	mach->ret = SDVALUE(SDRL_INCREF(args));
	return(0);
}


