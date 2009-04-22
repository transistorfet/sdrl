/*
 * Library Name:	io.h
 * Description:		I/O Functions Header
 */


#ifndef _SDRL_LIB_IO_H
#define _SDRL_LIB_IO_H

#include <sdrl/sdrl.h>

int sdrl_load_io(sdMachine *);

/*** Functions ***/
int sdrl_io_print(sdMachine *, sdArray *);

int sdrl_io_open(sdMachine *, sdArray *);

#endif

