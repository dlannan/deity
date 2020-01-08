// ZEUS debug section. 
// The debug section of ZEUS is platform dependant!! 

// Win32 windows API debugging to be implemented first.
// Important note - this is NOT the same as module debugging, this
//					is the equivalent of kernel debugging.

#ifndef _ZEUS_DEBUG_H_
#define _ZEUS_DEBUG_H_

#include "..\\System\System.h"
#include "commctrl.h"

namespace ZEUS
{
	namespace Debug
	{
		//extern Data::PageType	debuglog;	// Here we have a String class vector.. page of info

		void InitDebug();
		void KillDebug();

		bool CycleCount(void);
		void CycleCountDec(void);

		HWND InitWindow(void);

		unsigned int SetTimeStamp(unsigned int ts);
//		void SetApolloList(ApolloVec *alist);
		void SetApolloList(ApolloMap *alist);
		void SetInputBuffer(Packet::PackClass *buff, int sz);

		void Out(Data::StringClass *str);
		void Out(Data::StringClass str);
		void Out(const char *txt);
		void Out(char *txt);
		void Out(char txt);
		void Out(unsigned char txt);
		void Out(int txt);
		void Out(unsigned int txt);
		void Out(long txt);
		void Out(unsigned long txt);
		void Out(float txt);
		void Out(double txt);
	}
}
#endif