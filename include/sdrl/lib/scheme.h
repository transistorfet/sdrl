/*
 * Library Name:	scheme.h
 * Version:		0.1
 * Description:		SD Scheme Functions Header
 */


#ifndef _SDRL_LIB_SCHEME_H
#define _SDRL_LIB_SCHEME_H

#include <sdrl.h>

int sdrl_load_scheme(struct sdrl_machine *);

/*** Parsers ***/
struct sdrl_expr *sdrl_scheme_parse_input(struct sdrl_input *, void *);
struct sdrl_expr *sdrl_scheme_parse_expr(struct sdrl_input *, void *);

/*** Types ***/

/*** Forms ***/

/*** Builtins ***/

#endif

