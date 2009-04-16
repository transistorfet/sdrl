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
int sdrl_io_print(sdMachine *mach, sdValue *args)
{
	sdValue *cur;

	cur = args;
	while (cur) {
		switch (cur->type->basetype) {
			case SDRL_BT_NUMBER:
				printf("%f", SDNUMBER(cur)->num);
				break;
			case SDRL_BT_STRING:
				printf("%s", SDSTRING(cur)->str);
				break;
			case SDRL_BT_POINTER:
				printf("0x%x", (unsigned int) SDPOINTER(cur)->ptr);
				break;
			case SDRL_BT_DATA:
				printf("0x%x", (unsigned int) cur);
				break;
			default:
				break;
		}
		cur = cur->next;
	}
	mach->ret = SDRL_INCREF(args);
	return(0);
}


