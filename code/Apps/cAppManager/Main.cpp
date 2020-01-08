// cAppManager 
// Apollo module for DEiTY
// David Lannan                  copyright 2001

#include <stdio.h>
#include <windows.h>
#include <string.h>

// DEiTY related
#include "..\\..\\System\\System.h"

#include "cAppManager.h"

extern "C" DLLEXPORT void * DLLENTRY SetZeusFunc(void *func)
{
	// cast the func
	Function *basefunc = (Function *) func;
	cAppManager::apollo.sendpacketfunc.name = basefunc->name;
	cAppManager::apollo.sendpacketfunc.proc = basefunc->proc;

	cAppManager::PreInit();	// Use for function submissions

	return (void *)(ApolloClass *)&cAppManager::apollo;
}


