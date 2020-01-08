// ApolloDLL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "ApolloDLL.h"
#include "ApolloDLLaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE ApolloDLLDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("APOLLODLL.AWX Initializing!\n");
		
		// Extension DLL one-time initialization
		AfxInitExtensionModule(ApolloDLLDLL, hInstance);

		// Insert this DLL into the resource chain
		new CDynLinkLibrary(ApolloDLLDLL);

		// Register this custom AppWizard with MFCAPWZ.DLL
		SetCustomAppWizClass(&ApolloDLLaw);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("APOLLODLL.AWX Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(ApolloDLLDLL);
	}
	return 1;   // ok
}
