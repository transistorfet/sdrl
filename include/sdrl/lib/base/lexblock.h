/*
 * Header Name:	lexblock.h
 * Description:	Lexblock Type Header
 */

#ifndef _SDRL_LIB_BASE_LEXBLOCK_H
#define _SDRL_LIB_BASE_LEXBLOCK_H

#include <sdrl/sdrl.h>

#define SDLEXBLOCK(ptr)		( (sdLexBlock *) (ptr) )

extern sdType sdLexBlockTypeDef;

typedef struct sdLexBlock sdLexBlock;

// TODO should you make this inherit from expr which would allow for awesomeness (?? howso?)
struct sdLexBlock {
	sdValue value;
	sdExpr *code;
	sdEnv *env;
};

sdType *sdrl_base_make_lexblock_type(sdMachine *);
sdValue *sdrl_base_create_lexblock(sdMachine *, sdType *, sdArray *);
int sdrl_base_destroy_lexblock(sdLexBlock *);
sdValue *sdrl_base_duplicate_lexblock(sdMachine *, sdLexBlock *);
int sdrl_base_evaluate_lexblock(sdMachine *, sdArray *);

#endif

