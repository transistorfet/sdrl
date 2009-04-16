/*
 * Name:	type.h
 * Description:	Type Manager Header
 */


#ifndef _SDRL_CORE_TYPE_H
#define _SDRL_CORE_TYPE_H

#include <sdrl/globals.h>

#define SDRL_BT_NUMBER		0x0001
#define SDRL_BT_STRING		0x0002
#define SDRL_BT_REFERENCE	0x0003
#define SDRL_BT_POINTER		0x0004
#define SDRL_BT_EXPRESSION	0x0005
#define SDRL_BT_ENVIRONMENT	0x0006
#define SDRL_BT_DATA		0x0007

#define SDRL_BT_USER_BASETYPE	0x1000

#define SDRL_TBF_PASS_EXPRS	0x0001		/** Pass the (unevaluated) expressions to evaluate function */

struct sdType {
	int size;
	short bitflags;
	short basetype;
	sdrl_create_t create;
	sdrl_destroy_t destroy;
	sdrl_duplicate_t duplicate;
	sdrl_evaluate_t evaluate;
};

sdType *sdrl_make_type(int, short, short, sdrl_create_t, sdrl_destroy_t, sdrl_duplicate_t, sdrl_evaluate_t);
int sdrl_destroy_type(sdType *);

/**
 * Notes on sdrl_type:
 *	- The 'size' field is meant to be the fixed allocation size of the type
 *	  not including memory allocated seperately and not including variable
 * 	  data such as strings that might be allocated along with the value
 *	  itself.  All value creating functions should, if given a type struct,
 *	  use the 'size' field as the base size plus any additional memory
 *	  needed.
 *	- The 'basetype' field is a number corresponding to a generic type.
 *	  Any value with a given 'basetype' number must be castable to the
 *	  corresponding struct.  If the value is uncastable to one of the
 *	  predefined basetypes then SDRL_BT_DATA or a value greater than
 *	  SDRL_BT_USER_BASETYPE may be used.  This field is intended for use in
 *	  quick and dirty type checking.
 */

#endif

