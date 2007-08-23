/*
 * Builtin Name:	print.c
 * Module Requirements:	stdio.h
 * Description:		Print Expression
 */

#include <sdrl/sdrl.h>
#include <stdio.h>

/**
 * Args:	<value>, ...
 * Description:	Prints out each of the values in the list and returns the list.
 */
int sdrl_base_print(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_value *cur;

	cur = value;
	while (cur) {
		switch (SDRL_BASE_TYPE(cur->type)) {
			case SDRL_BT_NUMBER:
				printf("%f", cur->data.number);
				break;
			case SDRL_BT_STRING:
				printf("%s", cur->data.str);
				break;
			case SDRL_BT_POINTER:
				printf("0x%x", (unsigned int) cur->data.ptr);
				break;
			default:
				break;
		}
		cur = cur->next;
	}
	mach->ret = SDRL_MAKE_REFERENCE(value);
	return(0);
}


