
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "..\\..\\System\\System.h"

#include "aRenderDX7.h"

ApolloClass	apollo;

namespace aRenderDX7
{
	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aRenderDX7::Init,"Init");
		apollo.SubmitFunction(aRenderDX7::Kill,"Kill");
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
