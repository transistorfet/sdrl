/*
 * Name:	library.h
 * Version:	0.1
 * Description:	Library Functions Header
 */


#ifndef LIBRARY_H
#define LIBRARY_H

#include "sdrl/sdrl.h"
#include "sdrl/bindings.h"
#include "sdrl/types.h"

int library_initialize(struct sdrl_machine *);

int library_print(struct sdrl_machine *);
int library_gets(struct sdrl_machine *);
int library_split(struct sdrl_machine *);
int library_strcmp(struct sdrl_machine *);

#endif

