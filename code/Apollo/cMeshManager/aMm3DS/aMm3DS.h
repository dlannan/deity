
#ifndef _aMm3DS_H_
#define _aMm3DS_H_

#include "..\\..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

extern ApolloClass	apollo;

namespace aMm3DS 
{
	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
