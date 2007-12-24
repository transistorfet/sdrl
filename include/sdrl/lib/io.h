/*
 * Library Name:	io.h
 * Description:		I/O SDRL Functions Header
 */


#ifndef _SDRL_LIB_IO_H
#define _SDRL_LIB_IO_H

#include <sdrl/sdrl.h>

int sdrl_load_io(struct sdrl_machine *);

/*** Builtins ***/
int sdrl_io_print(struct sdrl_machine *, struct sdrl_value *);

int sdrl_io_open(struct sdrl_machine *, struct sdrl_value *);

#endif

