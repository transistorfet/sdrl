/*
 * Header Name:	func.h
 * Description:	C Function Type Header
 */

#ifndef _SDRL_LIB_BASE_FUNC_H
#define _SDRL_LIB_BASE_FUNC_H

#include <sdrl/sdrl.h>

sdType *sdrl_base_make_func_type(sdMachine *);
int sdrl_base_evaluate_func(sdMachine *, sdArray *);

#endif

