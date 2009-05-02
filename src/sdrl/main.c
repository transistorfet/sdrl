/*
 * Name:	main.c
 * Description:	SDRL Interpreter
 */

#include <stdio.h>
#include <string.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/string.h>

char *infile = NULL;

int parse_cmdline(int, char **);
int print_result(sdMachine *, int);

static int convert_ast(sdExpr *expr);

int main(int argc, char **argv)
{
	sdExpr *code;
	sdMachine *mach;

	if (parse_cmdline(argc, argv)) {
		printf("Usage: %s [infile]\n", argv[0]);
		return(1);
	}

	if (!(mach = sdrl_make_machine()))
		return(-1);
	if (sdrl_load_base(mach)
	    || sdrl_load_string(mach)
	    || sdrl_load_io(mach)) {
		printf("Error initializing primatives\n");
		sdrl_machine_destroy(mach);
		return(-1);
	}
	sdrl_set_parser(mach, sdrl_base_parse_lambda_input);
	if (!(code = sdrl_base_parse_file(mach, infile))) {
		printf("Cannot parse file, %s\n", infile);
		sdrl_machine_destroy(mach);
		return(-1);
	}

	//sdrl_base_display_expr(code);
	// TODO this creates a cyclical reference which requires garbage collection
	sdrl_env_add(mach->env, "*globals*", SDRL_INCREF(mach->global));
	convert_ast(code);
	print_result(mach, sdrl_evaluate(mach, code));

	sdrl_expr_destroy(code);
	sdrl_machine_destroy(mach);
	return(0);
}

int parse_cmdline(int argc, char **argv)
{
	int i, j;
	for (i = 1;i < argc;i++) {
		if (argv[i][0] == '-') {
			j = 0;
			while (argv[i][++j] != '\0') {
				switch (argv[i][j]) {
					// TODO parse flags
					default:
						printf("Usage: %s [infile]\n", argv[0]);
						return(1);
				}
			}
		}
		else {
			infile = argv[i];
		}
	}

	if (!infile)
		return(-1);
	return(0);

}

int print_result(sdMachine *mach, int exitcode)
{
	if (mach->error) {
		printf("\n");
		sdrl_base_display_error(mach->error);
	}
	else if (exitcode != 0)
		printf("\nInternal Error %d\n", exitcode);
	return(exitcode);
}

static int convert_ast(sdExpr *expr)
{
	sdExpr *call;

	for (; expr; expr = expr->next) {
		if (expr->type == SDRL_ET_CALL && (call = expr->data.expr) && call->type == SDRL_ET_STRING) {
			if (!strcmp(call->data.str, "if")) {
				// TODO check that there are exactly 2 or 3 argument expressions
				// TODO convert call into (if <arg1> (code <arg2>) (code <arg3))
				//if (!(expr = sdrl_make_string_expr(type, SDRL_ET_STRING, line, "$", expr))
				//    || !(expr = sdrl_make_call_expr(type, SDRL_ET_CALL, line, expr, NULL)))
				//	return(NULL);
			}
		}
	}
	return(0);
}

/*
void init_macros(sdMachine *mach)
{
	sdrl_add_macro("if", 2, 3,
		sdrl_make_call_expr(&sdExprTypeDef, SDRL_ET_CALL, 0, sdrl_make_string_expr(&sdExprTypeDef, SDRL_ET_STRING, 0, "if", sdrl_make_string_expr(&sdExprTypeDef, SDRL_ET_STRING, 0, "$1",
			sdrl_make_call_expr(&sdExprTypeDef, SDRL_ET_CALL, 0, sdrl_make_string_expr(&sdExprTypeDef, SDRL_ET_STRING, 0, "code", sdrl_make_string_expr(&sdExprTypeDef, SDRL_ET_STRING, 0, "$2", NULL)),
				sdrl_make_call_expr(&sdExprTypeDef, SDRL_ET_CALL, 0, sdrl_make_string_expr(&sdExprTypeDef, SDRL_ET_STRING, 0, "code", sdrl_make_string_expr(&sdExprTypeDef, SDRL_ET_STRING, 0, "$3", NULL)), NULL)
			), NULL),
		), NULL)
	);
}
*/

//struct sdrl_macro *macros = {
//	{ "if", 2, 3, /* (if $1 (code $2) (code $3)) how do we record this? */ },
//	{ "defun", 2, -1, /* (set $1 (dynblock (set $2 ($ _)) $...) */ }
//};


/*
	(defmacro if (code
		(if $1 (code $2) (code $3))))

	// TODO how do you convert $2 from (n1 n2 ...) to (list n1 n2 ...)
	// $: is assumed to be the remaining arguments
	(defmacro defun (code
		(set $1 (dynblock (setlist $2 ($ _)) $:))))
*/

