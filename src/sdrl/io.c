/*
 * Name:	io.c
 * Version:	0.1
 * Description:	Input/Output Manager
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "value.h"
#include "types.h"

#define FILE_BLOCK_SIZE		4096
#define MAX_BUFFER		256

struct input_stack *create_input_stack(int size)
{
	struct input_stack *stack;

	if (!(stack = (struct input_stack *) malloc(sizeof(struct input_stack) + (sizeof(struct source) * size))))
		return(NULL);

	stack->isp = 0;
	stack->size = size;
	stack->instack = (struct source *) ((usize_t) stack + sizeof(struct input_stack));

	stack->instack[0].bitflags = 0;
	stack->instack[0].i = 0;
	stack->instack[0].str = "\0";

	return(stack);
}

int destroy_input_stack(struct input_stack *stack)
{
	free(stack);
	return(0);
}

int io_loadfile(struct input_stack *stack, char *file)
{
	FILE *fptr;
	char *buffer;
	int i = 0, size = FILE_BLOCK_SIZE;

	if (!(fptr = fopen(file, "rb")))
		return(ERR_CANNOTOPEN);
	if (!(buffer = (char *) malloc(FILE_BLOCK_SIZE)))
		return(ERR_OUTOFMEMORY);

	while (!feof(fptr)) {
		buffer[i] = fgetc(fptr);
		if (i++ >= size) {
			size += FILE_BLOCK_SIZE;
			if (!(buffer = (char *) realloc(buffer, size)))
				return(ERR_OUTOFMEMORY);
		}
	}
	buffer[--i] = '\0';

	fclose(fptr);
	return(io_stackstring(stack, buffer, IBF_ALLOCATED));
}

int io_stackstring(struct input_stack *stack, char *str, uubyte bitflags)
{
	if (stack->isp + 1 >= stack->size)
		return(-1);
	stack->instack[++stack->isp].bitflags = bitflags;
	stack->instack[stack->isp].i = 0;
	stack->instack[stack->isp].str = str;
	return(0);
}

int io_changeflags(struct input_stack *stack, int offset, uubyte bitflags, uubyte set)
{
	if ((stack->isp - offset) < 0)
		return(ERR_OUTOFBOUNDS);
	if (!(set))
		stack->instack[stack->isp - offset].bitflags &= (~bitflags);
	else
		stack->instack[stack->isp - offset].bitflags |= bitflags;
	return(0);
}

char io_getchar(struct input_stack *stack)
{
	char ret;

	while (stack->isp && (ret = io_getrawchar(stack))) {
		if (ret == '#')
			io_parse_preop(stack);
		else if (!value_iswhitespace(ret))
			return(ret);
	}

	return(0);
}

char io_peekchar(struct input_stack *stack)
{
	int i = 0;
	char ret = 0;

	while ((ret = stack->instack[stack->isp].str[stack->instack[stack->isp].i + i++]) && !(value_iswhitespace(ret))) ;

	return(ret);
}

char io_getrawchar(struct input_stack *stack)
{
	char ret = 0;

	if (!(stack->isp))
		return(0);

	if (!(ret = stack->instack[stack->isp].str[stack->instack[stack->isp].i++])) {
		if (stack->instack[stack->isp].bitflags & IBF_ALLOCATED)
			free(stack->instack[stack->isp].str);
		if (!(stack->instack[stack->isp].bitflags & IBF_ENDOFINPUT))
			ret = ' ';

		if (stack->instack[stack->isp].bitflags & IBF_REPEAT) {
			stack->instack[stack->isp].i = 1;
			ret = stack->instack[stack->isp].str[0];
		}
		else
			stack->isp--;
	}

	return(ret);
}

int io_ungetchar(struct input_stack *stack, int i)
{
	stack->instack[stack->isp].i -= i;
	if (stack->instack[stack->isp].i < 0) {
		stack->instack[stack->isp].i = 0;
		return(-1);
	}
	return(0);
}

int io_pushnull(struct input_stack *stack)
{
	if (stack->isp + 1 >= stack->size)
		return(-1);
	stack->instack[++stack->isp].i = 0;
	stack->instack[stack->isp].bitflags = IBF_ENDOFINPUT;
	stack->instack[stack->isp].str = stack->instack[0].str;
	return(0);
}

char *io_getprevchars(struct input_stack *stack, int i)
{
	if ((stack->instack[stack->isp].i - i) < 0)
		return(NULL);
	return(&stack->instack[stack->isp].str[stack->instack[stack->isp].i - i]);
}

int io_parse_preop(struct input_stack *stack)
{
	char ch;
	int i = -1;
	char name[MAX_BUFFER];

	switch (io_getrawchar(stack)) {
		case '*':
			while (ch = io_getrawchar(stack)) {
				if (!(ch))
					return(ERR_NOINPUT);
				while (ch == '*') {
					if ((ch = io_getrawchar(stack)) && (ch == '#'))
						return(0);
				}
			}
			break;
		case '<':
			while (((name[++i] = io_getrawchar(stack)) != '>') && (i < MAX_BUFFER))
				if (!(name[i]))
					return(ERR_NOINPUT);
			if (i >= MAX_BUFFER)
				return(ERR_OUTOFBOUNDS);
			name[i] = '\0';
			#ifdef WIN32
				io_convertslashes(name);
			#endif
			io_loadfile(stack, name);
			break;
		case '#':
		default:
			while ((ch = io_getrawchar(stack)) != '\n')
				if (!(ch))
					return(ERR_NOINPUT);
			break;
	}	
	return(0);
}

char *io_convertslashes(char *buf)
{
	int i = -1;

	while (buf[++i] != '\0') {
		if (buf[i] == '/')
			buf[i] = '\\';
	}
	return(buf);
}

