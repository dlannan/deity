// ZEUS in DEiTY
// by David Lannan

// copyright June 2001

#include <windows.h>
#include <string.h>

#include "..\\System\\System.h"

#include "ZEUS.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ZEUS::instance = hInstance;

	// Inits the modules and zeus
	ZEUS::Prep(__argv,__argc);

	// Begins the process loop for Zeus, Pretty much everything must
	// wait for the return of this function
	ZEUS::Process(__argv,__argc);

	// Zeus kills everything - even "the game" you created..
	ZEUS::Kill();

	return 0;
}