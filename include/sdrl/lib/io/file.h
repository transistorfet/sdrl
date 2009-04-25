/*
 * Header Name:	file.h
 * Description:	File Type Header
 */

#ifndef _SDRL_LIB_BASE_FILE_H
#define _SDRL_LIB_BASE_FILE_H

#include <sdrl/sdrl.h>

#define SDFILE(ptr)		( (sdFile *) (ptr) )

extern sdType sdFileTypeDef;

typedef struct sdFile sdFile;

struct sdFile {
	sdValue value;
	// TODO finish
};

sdValue *sdrl_io_create_file(sdMachine *, sdType *, sdArray *);
int sdrl_io_destroy_file(sdValue *);

#endif

