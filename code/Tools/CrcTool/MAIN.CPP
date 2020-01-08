// CRC Tool

// Use in MSVC - copy a string to the clipboard, then execute the tool.

#include <windows.h>
#include <string.h>

#include "..\\..\\System\\System.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CRC::StringClass	strcrc;
	Data::StringClass		str;
	Data::StringClass		out;
	HANDLE			clipdatain;
	HGLOBAL			hMem;
	char *			pMem;

	// Nothing in clipboard to convert!
	if(!OpenClipboard(GetActiveWindow())) return 1;

	clipdatain = GetClipboardData(CF_TEXT);
	if(!clipdatain) return 2;
	str << (char *)clipdatain;
	// remove any annoying whitespace
	str - ' ';
	str - '\n';
	str - '\t';
	str - '\r';

	strcrc << str.String();

	out << "0x";
	out << strcrc.Id();
	out << " // ";
	out << strcrc.String();

	hMem = GlobalAlloc(GHND | GMEM_DDESHARE ,strlen(out.String())+1);
	if(hMem) 
	{
		pMem = (char *)GlobalLock(hMem);
		strcpy( pMem, out.String() );
		GlobalUnlock( hMem );
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
	}

	CloseClipboard();
	return 0;
}

