/*
 * Name:	error.c
 * Description:	Error Generating and Reporting Functions
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/heap.h>
#include <sdrl/core/value.h>
#include <sdrl/core/error.h>
#include <sdrl/globals.h>

#define NUM_ERRORS	14

sdType sdErrorTypeDef = {
	&sdValueTypeDef,
	sizeof(sdError),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_error_destroy,
	(sdrl_duplicate_t) sdrl_error_duplicate,
	NULL
};

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

static sdError memory_error = { { 1, NULL }, 0, SDRL_EBF_STATIC, 1, SDRL_ERR_OUT_OF_MEMORY, "Out Of Memory" };

/**
 * Allocate and initialize an error report.
 */
sdError *sdrl_make_error(sdHeap *heap, linenumber_t line, short severity, int err, const char *msg)
{
	sdError *error;

	if (err == SDRL_ERR_OUT_OF_MEMORY)
		return(&memory_error);
	if (!(error = (sdError *) sdrl_heap_alloc(heap, sizeof(sdError))))
		return(&memory_error);
	SDVALUE(error)->refs = 1;
	// TODO fix this later
	SDVALUE(error)->type = NULL;
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
int sdrl_error_destroy(sdError *error)
{
	if (error && !SDRL_BF_IS_SET(error, SDRL_EBF_STATIC))
		sdrl_heap_free(error);
	return(0);
}
 
/**
 * Duplicate the given error report.
 */
sdError *sdrl_error_duplicate(sdHeap *heap, sdError *org)
{
	sdError *error;

	if (!(error = (sdError *) sdrl_heap_alloc(heap, sizeof(sdError))))
		return(&memory_error);
	SDVALUE(error)->refs = 1;
	SDVALUE(error)->type = SDVALUE(org)->type;
	error->line = org->line;
	error->bitflags = org->bitflags;
	error->severity = org->severity;
	error->err = org->err;
	error->msg = org->msg;
	return(error);
}

