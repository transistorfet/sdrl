/*
 * Header Name:	form.h
 * Description:	C Form Type Header
 */

#ifndef _SDRL_LIB_BASE_FORM_H
#define _SDRL_LIB_BASE_FORM_H

#include <sdrl/sdrl.h>

sdType *sdrl_base_make_form_type(sdMachine *);
int sdrl_base_evaluate_form(sdMachine *, sdArray *);

#endif

