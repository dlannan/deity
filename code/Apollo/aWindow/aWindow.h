
#ifndef _aWindow_H_
#define _aWindow_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

extern ApolloClass	apollo;

namespace aWindow 
{
	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
