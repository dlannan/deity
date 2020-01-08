
#ifndef _cMeshViewer_H_
#define _cMeshViewer_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

namespace cMeshViewer 
{
	extern ApolloClass	apollo;

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
