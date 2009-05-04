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
	print_result(mach, sdrl_eval(mach, code));

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

