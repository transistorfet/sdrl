/*
 * Builtin Name:	print.c
 * Version:		0.1
 * Module Requirements:	stdio.h
 * Description:		Print Expression
 */

#include <sdrl.h>
#include <stdio.h>

/**
 * print(<value>, ...)
 * Prints out each of the values in the list and returns the list.
 */
int sdrl_base_print(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_value *cur;

	cur = value;
	while (cur) {
		switch (sdrl_base_type_m(cur->type)) {
			case SDRL_BT_NUMBER:
				printf("%f", cur->data.number);
				break;
			case SDRL_BT_STRING:
				printf("%s", cur->data.str);
				break;
			case SDRL_BT_POINTER:
				printf("%d", cur->data.ptr);
				break;
			default:
				break;
		}
		cur = cur->next;
	}
	mach->ret = value;
	return(0);
}


