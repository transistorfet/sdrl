/*
 * Types.h - General Standard Data Types
 */


#ifndef TYPES_H
#define TYPES_H

#include "error.h"

#define _USE32

#ifndef TRUE
	#define FALSE		0
	#define TRUE		1		/* !FALSE */
#endif

#undef  NULL
#define NULL			0

	typedef unsigned char uubyte;
	typedef unsigned short uuword;
	typedef unsigned long uulong;
	typedef unsigned long long uuquad;

	typedef signed char usbyte;
	typedef signed short usword;
	typedef signed long uslong;
	typedef signed long long usquad;

typedef uulong _u_size_t;
typedef _u_size_t usize_t;

#endif

