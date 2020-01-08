#ifndef _HOMER_H_
#define _HOMER_H_

#include "..\\System\\System.h"

// HOMER
// Scripting system - first great literary person.

namespace HOMER
{
	void Init(void);
	void Kill(void);

	void LoadScript(Packet::PackClass *in);
	void SaveScript(Packet::PackClass *in);
	void EditScript(Packet::PackClass *in);
	void Interactive(Packet::PackClass *in);
	void StartRecScript(Packet::PackClass *in);
	void StopRecScript(Packet::PackClass *in);
}

// This is the interface for all objects in zeus.. to the outside world..
extern	ApolloClass	apollo;

#endif