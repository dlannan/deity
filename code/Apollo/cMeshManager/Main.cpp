// cMeshManager 
// Apollo module for DEiTY
// David Lannan                  copyright 2001

#include <stdio.h>
#include <windows.h>
#include <string.h>

// DEiTY related
#include "..\\..\\System\\System.h"
#include "cMeshManager.h"

extern "C" DLLEXPORT void * DLLENTRY SetZeusFunc(void *func)
{
	// cast the func
	Function *basefunc = (Function *) func;
	apollo.sendpacketfunc.name = basefunc->name;
	apollo.sendpacketfunc.proc = basefunc->proc;

	cMeshManager::PreInit();	// Use for function submissions

	return (void *)(ApolloClass *)&apollo;
}


