/*
 * Name:	main.c
 * Description:	SDRL Interpreter
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>
#include <sdrl/lib/base.h>

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
	if (sdrl_load_base(mach) || sdrl_load_io(mach)) {
		printf("Error initializing primatives\n");
		sdrl_destroy_machine(mach);
		return(-1);
	}
	if (!(code = sdrl_base_parse_file(infile, (sdrl_parser_t) sdrl_base_parse_lambda_input, NULL))) {
		printf("Cannot parse file, %s\n", infile);
		sdrl_destroy_machine(mach);
		return(-1);
	}

	sdrl_base_display_expr(code);
sdrl_add_binding(mach->env, "*globals*", sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "env"), (sdrl_data_t) (void *) mach->env, 0, NULL));
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
					// parse flags
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

int print_result(struct sdrl_machine *mach, int error)
{
	if (mach->ret) {
//		prim_print(mach, mach->ret);
//		printf("%f", mach->ret->data.number);
//		printf("%s", mach->ret->data.str);
	}

	printf("\n");
	if (mach->error) {
		//printf("Error %d,%d: %s\n", SDRL_GET_LINE_NUMBER(mach->error->line), SDRL_GET_COLUMN_NUMBER(mach->error->line), mach->error->msg);
		sdrl_base_display_error(mach->error);
	}
	else {
		switch (error) {
			case 0:
				printf("Exited Normally\n");
				return(0);
			case SDRL_ERR_END_OF_INPUT:
				printf("End Of Input Reached\n");
				break;
			case SDRL_ERR_SYNTAX_ERROR:
				printf("Syntax Error\n");
				break;
			case SDRL_ERR_INVALID_PARAMS:
				printf("Invalid Parameters\n");
				break;
			case SDRL_ERR_INVALID_TYPE:
				printf("Invalid Type\n");
				break;
			case SDRL_ERR_OUT_OF_MEMORY:
				printf("Out Of Memory\n");
				break;
			case SDRL_ERR_OUT_OF_BOUNDS:
				printf("Out Of Bounds\n");
				break;
			case SDRL_ERR_NOT_FOUND:
				printf("Binding Not Found\n");
				break;
			case SDRL_ERR_PARAMS_ERROR:
				printf("Error Evaluating Parameters\n");
				break;
			default:
				printf("Error Number: %d\n", error);
				break;
		}
	}
	return(-1);
}

