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

sdValue *sdrl_io_file_create(sdMachine *, sdType *, sdArray *);
void sdrl_io_file_destroy(sdValue *);

#endif

