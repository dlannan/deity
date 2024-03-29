// cCrossEyed 
// Apollo module for DEiTY
// David Lannan                  copyright 2001

#include <stdio.h>
#include <windows.h>
#include <string.h>

// DEiTY related
#include "..\\..\\System\\System.h"

#include "cCrossEyed.h"

extern "C" DLLEXPORT void * DLLENTRY SetZeusFunc(void *func)
{
	// cast the func
	Function *basefunc = (Function *) func;
	cCrossEyed::apollo.sendpacketfunc.name = basefunc->name;
	cCrossEyed::apollo.sendpacketfunc.proc = basefunc->proc;

	cCrossEyed::PreInit();	// Use for function submissions

	return (void *)(ApolloClass *)&cCrossEyed::apollo;
}


