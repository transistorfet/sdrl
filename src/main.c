/*
 * Name:	main.c
 * Version:	0.1
 * Description:	SDRL Interpreter
 */

#include <stdio.h>

#include <sdrl.h>
#include "library.h"

char *infile = NULL;

int parse_cmdline(int, char **);
int print_error(struct sdrl_machine *, int);

main(int argc, char **argv)
{
	struct sdrl_machine *sdrl;

	if (parse_cmdline(argc, argv)) {
		printf("Usage: %s [infile]\n", argv[0]);
		return(1);
	}

	if (!(sdrl = create_sdrl_machine(NULL)))
		return(-1);

	if (library_initialize(sdrl)) {
		destroy_sdrl_machine(sdrl);
		return(-1);
	}

	if (sdrl_loadfile(sdrl, infile)) {
		printf("Cannot open file, %s\n", infile);
		destroy_sdrl_machine(sdrl);
		return(-1);
	}

	print_error(sdrl, sdrl_run(sdrl));

	destroy_sdrl_machine(sdrl);

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

	if (!(infile))
		return(-1);
	return(0);

}

int print_error(struct sdrl_machine *mach, int error)
{
	if (mach->ret) {
		if (mach->ret->type & VT_STRING)
			printf("%s\n", mach->ret->data.str);
		else
			printf("%d\n", mach->ret->data.num);
	}

	switch (error) {
		case 0:
			return(0);
		case ERR_ENDOFINPUT:
			printf("End Of Input Reached\n");
			break;
		case ERR_SYNTAXERROR:
			printf("Syntax Error\n");
			break;
		case ERR_INVALIDPARAMS:
			printf("Invalid Parameters\n");
			break;
		case ERR_INVALIDTYPE:
			printf("Invalid Type\n");
			break;
		case ERR_OUTOFMEMORY:
			printf("Out Of Memory\n");
			break;
		case ERR_OUTOFBOUNDS:
			printf("Out Of Bounds\n");
			break;
		case ERR_NOTFOUND:
			printf("Binding Not Found\n");
			break;
		default:
			printf("Unknown Error\n");
			break;
	}
	return(-1);
}

