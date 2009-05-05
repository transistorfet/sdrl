/*
 * Library Name:	io.h
 * Description:		I/O Functions Header
 */


#ifndef _SDRL_LIB_IO_H
#define _SDRL_LIB_IO_H

#include <sdrl/sdrl.h>

#include <sdrl/lib/io/file.h>

int sdrl_load_io(sdMachine *, sdEnv *);

/*** Functions ***/
int sdrl_io_print(sdMachine *, sdArray *);

int sdrl_io_open(sdMachine *, sdArray *);

#endif

