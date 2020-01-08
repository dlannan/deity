// cMeshViewer 
// Apollo module for DEiTY
// David Lannan                  copyright 2001

#include <stdio.h>
#include <windows.h>
#include <string.h>

// DEiTY related
#include "..\\..\\System\\System.h"

#include "cMeshViewer.h"

extern "C" DLLEXPORT void * DLLENTRY SetZeusFunc(void *func)
{
	// cast the func
	Function *basefunc = (Function *) func;
	cMeshViewer::apollo.sendpacketfunc.name = basefunc->name;
	cMeshViewer::apollo.sendpacketfunc.proc = basefunc->proc;

	cMeshViewer::PreInit();	// Use for function submissions

	return (void *)(ApolloClass *)&cMeshViewer::apollo;
}


