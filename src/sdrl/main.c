/*
 * Name:	main.c
 * Description:	SDRL Interpreter
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/string.h>

char *infile = NULL;

int parse_cmdline(int, char **);
int print_result(struct sdrl_machine *, int);

int main(int argc, char **argv)
{
	struct sdrl_expr *code;
	struct sdrl_machine *mach;

	if (parse_cmdline(argc, argv)) {
		printf("Usage: %s [infile]\n", argv[0]);
		return(1);
	}

	if (!(mach = sdrl_create_machine()))
		return(-1);
	if (sdrl_load_base(mach)
	    || sdrl_load_string(mach)
	    || sdrl_load_io(mach)) {
		printf("Error initializing primatives\n");
		sdrl_destroy_machine(mach);
		return(-1);
	}
	if (!(code = sdrl_base_parse_file(infile, (sdrl_parser_t) sdrl_base_parse_lambda_input, NULL))) {
		printf("Cannot parse file, %s\n", infile);
		sdrl_destroy_machine(mach);
		return(-1);
	}

	//sdrl_base_display_expr(code);
	sdrl_add_binding(mach->env, "*globals*", sdrl_make_reference(mach->heap, sdrl_find_binding(mach->type_env, "env"), SDRL_VALUE(mach->env)));
	print_result(mach, sdrl_evaluate(mach, code));

	sdrl_destroy_machine(mach);
	sdrl_destroy_expr(code);

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

int print_result(struct sdrl_machine *mach, int exitcode)
{
	if (mach->error) {
		printf("\n");
		sdrl_base_display_error(mach->error);
	}
	else if (exitcode != 0)
		printf("\nInternal Error %d\n", exitcode);
	return(exitcode);
}

