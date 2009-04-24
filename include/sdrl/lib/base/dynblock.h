/*
 * Header Name:	dynblock.h
 * Description:	Dynblock Type Header
 */

#ifndef _SDRL_LIB_BASE_DYBLOCK_H
#define _SDRL_LIB_BASE_DYBLOCK_H

#include <sdrl/sdrl.h>

sdType *sdrl_base_make_dynblock(sdMachine *);
int sdrl_base_evaluate_dynblock(sdMachine *, sdArray *);

#endif

