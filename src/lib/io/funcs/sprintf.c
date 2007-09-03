/*
 * Builtin Name:	sprintf.c
 * Module Requirements:	string.h
 * Description:		Print Fomatted String
 */

#include <string.h>

#include <sdrl/sdrl.h>

/**
 * Args:	<format>, ...
 * Description:	Returns a string created from the format string ath the values in the list.
 */
int sdrl_io_sprintf(struct sdrl_machine *mach, struct sdrl_value *value)
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
	mach->ret = value;
	return(0);
}


