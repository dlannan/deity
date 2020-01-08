
#ifndef _cTerrain_H_
#define _cTerrain_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

namespace cTerrain 
{
	extern ApolloClass	apollo;

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
