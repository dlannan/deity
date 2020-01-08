
#ifndef _cMeshManager_H_
#define _cMeshManager_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

extern ApolloClass	apollo;

namespace cMeshManager 
{
	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
