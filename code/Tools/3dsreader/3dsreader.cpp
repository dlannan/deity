// 3DSReader.cpp : Defines the entry point for the application.
//

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// Fetch an elf to run
	CFileDialog felf(TRUE);

	// Grab a saved previous path
//	strcpy(felf.m_ofn.lpstrFile,(char *)(LPCTSTR)PrevFile);

	felf.DoModal();

//	strcpy((char *)(LPCTSTR)PrevFile,felf.m_ofn.lpstrFile);
	
	return 0;
}



