/*
 * Name:	type.h
 * Description:	Type Manager Header
 */


#ifndef _SDRL_CORE_TYPE_H
#define _SDRL_CORE_TYPE_H

#include <sdrl/globals.h>

#define SDRL_VARIABLE_SIZE	-1

#define SDRL_BT_NUMBER		0x01
#define SDRL_BT_STRING		0x02
#define SDRL_BT_POINTER		0x03
#define SDRL_BT_DATA		0x04

#define SDRL_TBF_PASS_EXPRS	0x0001		/** Pass the expressions (unevaluated) to evaluate function */

struct sdrl_type {
	int size;
	short bitflags;
	short basetype;
	sdrl_create_t create;
	sdrl_destroy_t destroy;
	sdrl_duplicate_t duplicate;
	sdrl_evaluate_t evaluate;
};

struct sdrl_type *sdrl_make_type(int, short, short, sdrl_create_t, sdrl_destroy_t, sdrl_duplicate_t, sdrl_evaluate_t);
int sdrl_destroy_type(struct sdrl_type *);

#endif

