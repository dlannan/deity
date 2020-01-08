
#ifndef _aMmCOB_H_
#define _aMmCOB_H_

#include "..\\..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

extern ApolloClass	apollo;

namespace aMmCOB 
{

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
