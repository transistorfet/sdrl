/*
 * Name:	error.c
 * Description:	Error Generating and Reporting Functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <sdrl/core/heap.h>
#include <sdrl/core/value.h>
#include <sdrl/core/error.h>
#include <sdrl/globals.h>

#define NUM_ERRORS		14
#define ERROR_STRING_SIZE	1024

sdType sdErrorTypeDef = {
	&sdValueTypeDef,
	"error",
	sizeof(sdError),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_error_destroy,
	(sdrl_duplicate_t) sdrl_error_duplicate,
	NULL
};

static char *error_msgs[] = {
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

sdError sdMemoryError = {
	{ 1, &sdErrorTypeDef },
	0,
	SDRL_EBF_STATIC,
	SDRL_ES_FATAL,
	SDRL_ERR_OUT_OF_MEMORY,
	"Out Of Memory"
};

/**
 * Allocate and initialize an error report.  This function is guarenteed to
 * return a pointer to an error message.  If the function fails, a pointer to
 * the static memory error will be returned.
 */
sdError *sdrl_make_error(sdHeap *heap, sdType *type, linenumber_t line, short severity, int err, const char *msg, ...)
{
	va_list va;
	int len = -1;
	sdError *error;
	char buffer[ERROR_STRING_SIZE];

	if (err == SDRL_ERR_OUT_OF_MEMORY)
		return(&sdMemoryError);
	va_start(va, msg);
	if (msg && (len = vsnprintf(buffer, ERROR_STRING_SIZE, msg, va)) >= ERROR_STRING_SIZE)
		return(&sdMemoryError);
	if (!(error = (sdError *) sdrl_heap_alloc(heap, type->size + len + 1)))
		return(&sdMemoryError);
	SDVALUE(error)->refs = 1;
	SDVALUE(error)->type = type;
	error->line = line;
	error->bitflags = 0;
	error->severity = severity;
	error->err = err;

	if (msg) {
		error->msg = (char *) (error + 1);
		strcpy(error->msg, buffer);
	}
	else if ((err < 0) && (err > (-1 * NUM_ERRORS)))
		error->msg = error_msgs[ (err * -1) - 1 ];
	else
		error->msg = NULL;
	return(error);
}

/**
 * Free the resources of an error report.
 */
void sdrl_error_destroy(sdError *error)
{
	if (!error)
		return;
	if (SDRL_BF_IS_SET(error, SDRL_EBF_STATIC))
		SDVALUE(error)->refs = 1;
	else
		sdrl_heap_free(error);
}
 
/**
 * Duplicate the given error report.
 */
sdError *sdrl_error_duplicate(sdHeap *heap, sdError *org)
{
	sdError *error;

	if (!(error = (sdError *) sdrl_heap_alloc(heap, SDVALUE(org)->type->size)))
		return(&sdMemoryError);
	SDVALUE(error)->refs = 1;
	SDVALUE(error)->type = SDVALUE(org)->type;
	error->line = org->line;
	error->bitflags = org->bitflags;
	error->severity = org->severity;
	error->err = org->err;
	error->msg = org->msg;
	return(error);
}

