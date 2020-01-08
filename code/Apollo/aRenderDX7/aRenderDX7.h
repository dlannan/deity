
#ifndef _aRenderDX7_H_
#define _aRenderDX7_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

extern ApolloClass	apollo;

namespace aRenderDX7 
{
	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
