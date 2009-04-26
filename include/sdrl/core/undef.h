/*
 * Name:	undef.h
 * Description:	Undefined Type and Value Header File
 */


#ifndef _SDRL_CORE_UNDEF_H
#define _SDRL_CORE_UNDEF_H

#include <sdrl/core/value.h>
#include <sdrl/globals.h>

extern sdType sdUndefTypeDef;
extern sdValue sdUndefValue;

#define sdrl_get_undef()	\
	SDRL_INCREF(&sdUndefValue)

#define sdrl_is_undef(value)	\
	( SDVALUE(value) == &sdUndefValue )

sdValue *sdrl_make_undef(sdHeap *, sdType *);
int sdrl_undef_destroy(sdValue *);

#endif

