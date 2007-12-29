/*
 * Name:	error.c
 * Description:	Error Generating and Reporting Functions
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/error.h>
#include <sdrl/globals.h>

#define NUM_ERRORS	14

static const char *error_msgs[] = {
	"General error",
	"Out of memory",
	"Parse Error",
	"Syntax Error",
	"Unexpected end of input",
	"Invalid AST type",
	"Invalid name",
	"Invalid function",
	"Invalid type",
	"Invalid arguments",
	"Binding not found",
	"Binding in use",
	"Index out of bounds",
	"Divide by zero"
};

static struct sdrl_error memory_error = { 0, SDRL_EBF_STATIC, 1, SDRL_ERR_OUT_OF_MEMORY, "Out Of Memory" };

/**
 * Allocate and initialize an error report.
 */
struct sdrl_error *sdrl_make_error(linenumber_t line, short severity, int err, const char *msg)
{
	struct sdrl_error *error;

	if (err == SDRL_ERR_OUT_OF_MEMORY)
		return(&memory_error);
	if (!(error = (struct sdrl_error *) malloc(sizeof(struct sdrl_error))))
		return(&memory_error);
	error->line = line;
	error->bitflags = 0;
	error->severity = severity;
	error->err = err;

	if (msg)
		error->msg = msg;
	else if ((err < 0) && (err > (-1 * NUM_ERRORS)))
		error->msg = error_msgs[ (err * -1) - 1 ];
	else
		error->msg = NULL;
	return(error);
}

/**
 * Free the resources of an error report.
 */
int sdrl_destroy_error(struct sdrl_error *error)
{
	if (error && !SDRL_BF_IS_SET(error, SDRL_EBF_STATIC))
		free(error);
	return(0);
}
 
/**
 * Duplicate the given error report.
 */
struct sdrl_error *sdrl_duplicate_error(struct sdrl_error *org)
{
	struct sdrl_error *error;

	if (!(error = (struct sdrl_error *) malloc(sizeof(struct sdrl_error))))
		return(&memory_error);
	error->line = org->line;
	error->bitflags = org->bitflags;
	error->severity = org->severity;
	error->err = org->err;
	error->msg = org->msg;
	return(error);
}


