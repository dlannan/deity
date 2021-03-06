        ��  ��                  �      �� ��     0 	        �4   V S _ V E R S I O N _ I N F O     ���               ?                        >   S t r i n g F i l e I n f o      0 4 0 9 0 4 B 0        C o m p a n y N a m e     P   F i l e D e s c r i p t i o n     A P O L L O D L L   A p p W i z a r d   6   F i l e V e r s i o n     1 ,   0 ,   0 ,   1     4 
  I n t e r n a l N a m e   A P O L L O D L L   F   L e g a l C o p y r i g h t   C o p y r i g h t   �   2 0 0 1     (    L e g a l T r a d e m a r k s     D   O r i g i n a l F i l e n a m e   A P O L L O D L L . D L L   6   P r o d u c t N a m e     A p o l l o   D L L     :   P r o d u c t V e r s i o n   1 ,   0 ,   0 ,   1     D    V a r F i l e I n f o     $    T r a n s l a t i o n     	�8  D   T E M P L A T E   N E W P R O J . I N F         0	        $$// newproj.inf = template for list of template files
$$//  format is 'sourceResName' \t 'destFileName'
$$//    The source res name may be preceded by any combination of '=', '-',  '!', '?', ':', '#', and/or '*'
$$//       '=' => the resource is binary
$$//       '-' => the file should not be added to the project (all files are added to the project by default)
$$//       '!' => the file should be marked exclude from build
$$//       '?' => the file should be treated as a help file
$$//       ':' => the file should be treated as a resource
$$//       '#' => the file should be treated as a template (implies '!')
$$//       '*' => bypass the custom AppWizard's resources when loading
$$//	if name starts with / => create new subdir





MAIN.CPP	Main.cpp
ROOT.CPP	$$root$$.cpp
ROOT.H	$$root$$.h


�   D   T E M P L A T E   C O N F I R M . I N F         0	        Apollo DLL generates a template for making DEiTY
based DLL modules.

Be sure to read all comments in the included files.

Enjoy.
 U  <   T E M P L A T E   M A I N . C P P       0	        // $$root$$ 
// Apollo module for DEiTY
// David Lannan                  copyright 2001

#include <stdio.h>
#include <windows.h>
#include <string.h>

// DEiTY related
#include "..\\..\\System\\System.h"

#include "$$root$$.h"

extern "C" DLLEXPORT void * DLLENTRY SetZeusFunc(void *func)
{
	// cast the func
	Function *basefunc = (Function *) func;
	$$root$$::apollo.sendpacketfunc.name = basefunc->name;
	$$root$$::apollo.sendpacketfunc.proc = basefunc->proc;

	$$root$$::PreInit();	// Use for function submissions

	return (void *)(ApolloClass *)&$$root$$::apollo;
}


   �  <   T E M P L A T E   R O O T . C P P       0	        
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "$$root$$.h"
#include "..\\..\\System\\System.h"

namespace $$root$$
{
	ApolloClass	apollo;

	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction($$root$$::Init,"Init");
		apollo.SubmitFunction($$root$$::Kill,"Kill");
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
	}
}
  8   T E M P L A T E   R O O T . H       0	        
#ifndef _$$root$$_H_
#define _$$root$$_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

namespace $$root$$ 
{
	extern ApolloClass	apollo;

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
 �      �� ��     0	        (       @                                  �  �   �� �   � � ��  ��� ���   �  �   �� �   � � ��  ��� ��������wxx�x���������www�x��������ww����w������������p ��������x�w�� ������x�ww�  �����x�x��wx� ������f��wx���������h���w������������ww�� �������� h��wx ��������f��wx `��������ffh�x �������x������  ������xw��ww�������x��www� ���������w��wp �o�������g��w��������������v �������o�����` ������������x ������������ x��������x����`������x��x��� x������xxh�x���x�������w�������w������xx���hxww������x���g�w��������wx��h�w��������xx���ww����������ww�w��������������������                                                                                                              �                        �� ��     0	                 �   