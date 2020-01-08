
#ifndef _cCrossEyed_H_
#define _cCrossEyed_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

namespace cCrossEyed 
{
	extern ApolloClass	apollo;

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
