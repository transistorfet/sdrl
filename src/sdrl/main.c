/*
 * Name:	main.c
 * Version:	0.1
 * Description:	SDRL Interpreter
 */

#include <stdio.h>

#include <sdrl.h>

char *infile = NULL;

int parse_cmdline(int, char **);
int print_result(struct sdrl_machine *, int);

main(int argc, char **argv)
{
	struct sdrl_machine *mach;

	if (parse_cmdline(argc, argv)) {
		printf("Usage: %s [infile]\n", argv[0]);
		return(1);
	}

	if (!(mach = sdrl_create_machine()))
		return(-1);
	if (prim_initialize(mach)) {
		printf("Error initializing primatives\n");
		sdrl_destroy_machine(mach);
		return(-1);
	}

	if (!(mach->code = sdrl_parse_file(infile))) {
		printf("Cannot parse file, %s\n", infile);
		sdrl_destroy_machine(mach);
		return(-1);
	}

	print_result(mach, sdrl_evaluate_expr_list(mach, mach->code));

	sdrl_destroy_machine(mach);

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
//		printf("%f", mach->ret->data.number);
//		printf("%s", mach->ret->data.str);
	}

	printf("\n");
	switch (error) {
		case 0:
			printf("Exited Normally\n");
			return(0);
		case ERR_END_OF_INPUT:
			printf("End Of Input Reached\n");
			break;
		case ERR_SYNTAX_ERROR:
			printf("Syntax Error\n");
			break;
		case ERR_INVALID_PARAMS:
			printf("Invalid Parameters\n");
			break;
		case ERR_INVALID_TYPE:
			printf("Invalid Type\n");
			break;
		case ERR_OUT_OF_MEMORY:
			printf("Out Of Memory\n");
			break;
		case ERR_OUT_OF_BOUNDS:
			printf("Out Of Bounds\n");
			break;
		case ERR_NOT_FOUND:
			printf("Binding Not Found\n");
			break;
		case ERR_PARAMS_ERROR:
			printf("Error Evaluating Parameters\n");
			break;
		default:
			printf("Error Number: %d\n", error);
			break;
	}
	return(-1);
}

