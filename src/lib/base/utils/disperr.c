/*
 * Module Name:	disperr.c
 * Description:	Error Message Display
 */

#include <stdio.h>
#include <sdrl/sdrl.h>

/**
 * Print an error message based on the error report.
 */
int sdrl_base_display_error(sdError *error)
{
	if (error->msg)
		printf("Error %d,%d: %s\n", SDRL_GET_LINE_NUMBER(error->line), SDRL_GET_COLUMN_NUMBER(error->line), error->msg);
	else
		printf("Error %d,%d: Unknown error %d\n", SDRL_GET_LINE_NUMBER(error->line), SDRL_GET_COLUMN_NUMBER(error->line), error->err);
	return(0);
}


