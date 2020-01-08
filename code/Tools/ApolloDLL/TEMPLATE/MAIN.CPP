// $$root$$ 
// Apollo module for DEiTY
// David Lannan                  copyright 2001

#include <stdio.h>
#include <windows.h>
#include <string.h>

// DEiTY related
#include "..\\..\\System\\System.h"

#include "$$root$$.h"

extern "C" DLLEXPORT void * DLLENTRY SetZeusFunc(void *func)
{
	// cast the func
	Function *basefunc = (Function *) func;
	$$root$$::apollo.sendpacketfunc.name = basefunc->name;
	$$root$$::apollo.sendpacketfunc.proc = basefunc->proc;

	$$root$$::PreInit();	// Use for function submissions

	return (void *)(ApolloClass *)&$$root$$::apollo;
}


