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

sdValue *sdrl_base_lexblock_create(sdMachine *, sdType *, sdArray *);
void sdrl_base_lexblock_destroy(sdLexBlock *);
sdValue *sdrl_base_lexblock_duplicate(sdMachine *, sdLexBlock *);
int sdrl_base_lexblock_evaluate(sdMachine *, sdArray *);

#endif

