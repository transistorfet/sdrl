/*
 * Name:	error.c
 * Description:	Error Generating and Reporting Functions
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/error.h>
#include <sdrl/globals.h>

static linenumber_t current_line = 0;
static struct sdrl_error *last_error = NULL;

/**
 * Allocate and initialize an error report.
 */
struct sdrl_error *sdrl_make_error(int bitflags, int err, char *msg)
{
	struct sdrl_error *error;

	if (!(error = (struct sdrl_error *) malloc(sizeof(struct sdrl_error))))
		return(NULL);
	error->line = current_line;
	error->bitflags = bitflags;
	error->err = err;
	error->msg = msg;
	return(error);
}

/**
 * Set the current line number to be used when generating errors.
 */
void sdrl_set_linenumber(linenumber_t line)
{
	current_line = line;
}

/**
 * Free the resources of an error report.
 */
int sdrl_destroy_error(struct sdrl_error *error)
{
	if (error)
		free(error);
	return(0);
}
 
/**
 * Create a new error and set it as the last error to have occured and return the error code.
 */
int sdrl_set_error(int bitflags, int err, char *msg)
{
	if (err == SDRL_ERR_OUT_OF_MEMORY)
		last_error = NULL;
	else
		last_error = sdrl_make_error(bitflags, err, msg);
	return(err);
}

/**
 * Returns the last error (the error that was last set).
 */
struct sdrl_error *sdrl_last_error(void)
{
	return(last_error);
}

/**
 * Free and clear the last error and return the error code or 0 if no error had occured.
 */
int sdrl_clear_error(void)
{
	int err = 0;

	if (last_error) {
		err = last_error->err;
		sdrl_destroy_error(last_error);
		last_error = NULL;
	}
	return(err);
}


