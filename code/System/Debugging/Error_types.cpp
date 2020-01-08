// Debugging and Error checking tools
//

// All debugging - in debug mode only

#include <stdarg.h>

#include "..\\System.h"
#include "Error_types.h"

#if defined (_DEBUG)

// Windows specific stuff
#include <stdio.h>
#include <windows.h>

namespace Error
{
	// Makes a nice message box - will have stack trace too eventually
	bool CustomAssertFunc( bool expr, char *sexpr, char *desc, int line, char *file, bool *ignore )
	{
		if(expr)
		{
			char info[1024];	// plenty of room for message!
			sprintf(info, "expr:\t%s\nfile:\t%s\nline:\t%d\ndesc:\t%s\n",
								sexpr,file,line,desc);

			// Windows nasty stuff !
			int res = MessageBox( NULL, info, "Error::ASSERT", MB_ABORTRETRYIGNORE );
			if (res == IDIGNORE)
			{
				*ignore = true;
				return false;
			}
			if ( res == IDABORT )
			{
				*ignore = false;
				return true;
			}
			else if ( res == IDRETRY )
			{
				*ignore = false;
				return false;
			}

			return true;
		}

		return false;
	}

	void CustomWarnFunc( char * message, int line, char *file)
	{
		char info[1024];	// plenty of room for message!
		sprintf(info, "file:\t%s\nline:\t%d\nmessage:\t%s\n",
							file,line,message);
		// Windows nasty stuff !
		int res = MessageBox( NULL, info, "Error::WARNING", MB_OK );
	}
}

#endif

extern ApolloClass	apollo;

namespace Error
{
	static unsigned int	windowActive = 0;
	static HWND			win = NULL;
	static HWND			PrintfListBox = NULL;

	void MakePrintfWindow()
	{
		return;	
	}

	void FindCustomPrintf()
	{
	}

	void CustomPrintfFunc( char *format, ...)
	{
//		FindCustomPrintf();
		va_list args;
		char info[1024];	// plenty of room for message!

		va_start(args,format);

		_vsnprintf(info, sizeof(info), format, args);     
//		strcpy(info + vsprintf(info,format,args), "\r\n");

//		sprintf(info,format,args);
//		SendMessage(PrintfListBox, LB_ADDSTRING,0,(LPARAM) (LPCTSTR)info);
//		UpdateWindow( PrintfListBox );      
		{
		Packet("Printf","Zeus.Log");
		Padd info; 
		Psend;
		}
	}

	void CustomPrintfWindowScrollUp()
	{
	}

	void CustomPrintfWindowScrollDown()
	{
	}

	void CustomPrintfWindowToggleHide()
	{
	}

}
