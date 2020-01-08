
#ifndef _cAppManager_H_
#define _cAppManager_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

namespace cAppManager 
{
	extern ApolloClass	apollo;

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
