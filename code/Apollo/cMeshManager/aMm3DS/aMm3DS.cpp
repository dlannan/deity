
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "aMm3DS.h"
#include "..\\..\\..\\System\\System.h"

ApolloClass	apollo;

namespace aMm3DS
{
	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aMm3DS::Init,"Init");
		apollo.SubmitFunction(aMm3DS::Kill,"Kill");
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
	}
}
