// cTerrain 
// Apollo module for DEiTY
// David Lannan                  copyright 2001

#include <stdio.h>
#include <windows.h>
#include <string.h>

// DEiTY related
#include "..\\..\\System\\System.h"

#include "cTerrain.h"

extern "C" DLLEXPORT void * DLLENTRY SetZeusFunc(void *func)
{
	// cast the func
	Function *basefunc = (Function *) func;
	cTerrain::apollo.sendpacketfunc.name = basefunc->name;
	cTerrain::apollo.sendpacketfunc.proc = basefunc->proc;

	cTerrain::PreInit();	// Use for function submissions

	return (void *)(ApolloClass *)&cTerrain::apollo;
}


