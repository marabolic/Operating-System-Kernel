/*
 * syncPrintf.cpp
 *
 *  Created on: May 29, 2019
 *      Author: OS1
 */




#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "dos.h"

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	#endif
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	#endif
		return res;
}
