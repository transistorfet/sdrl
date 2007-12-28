/*
 * Module Name:	disperr.c
 * Description:	Error Message Display
 */

#include <stdio.h>
#include <sdrl/sdrl.h>

#define NUM_ERRORS	16

static const char *disperr_msgs[NUM_ERRORS] = {
	"General error",
	"Out of memory",
	"Binding not found:",
	"In use",
	"Unexpected end of input",
	"Index out of bounds",
	"",
	"Parse Error",
	"Syntax Error",
	"Error with parameters",
	"",
	"Invalid parameters",
	"Invalid AST type",
	"Invalid type",
	"Invalid name",
	"Invalid function"
};

/**
 * Print an error message based on the error report.
 */
int sdrl_base_display_error(struct sdrl_error *error)
{
	int err;

	err = (error->err * -1) - 1;
	if ((err < 0) || (err > NUM_ERRORS))
		return(0);
	else if (error->msg)
		printf("Error %d,%d: %s %s\n", SDRL_GET_LINE_NUMBER(error->line), SDRL_GET_COLUMN_NUMBER(error->line), disperr_msgs[err], error->msg);
	else
		printf("Error %d,%d: %s\n", SDRL_GET_LINE_NUMBER(error->line), SDRL_GET_COLUMN_NUMBER(error->line), disperr_msgs[err]);
	return(0);
}


