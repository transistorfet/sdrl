/*
 * Name:	scheme.h
 * Version:	0.1
 * Description:	Scheme Functions Header
 */


#ifndef _SDRL_LIB_SCHEME_H
#define _SDRL_LIB_SCHEME_H

#include <sdrl.h>

int scheme_initialize(struct sdrl_machine *);

int scheme_define(struct sdrl_machine *, struct sdrl_expr *);

#endif

