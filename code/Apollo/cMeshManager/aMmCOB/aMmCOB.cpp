
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "aMmCOB.h"
#include "..\\..\\..\\System\\System.h"
#include "..\\MeshClass.h"
#include "Loader.h"

ApolloClass	apollo;

namespace aMmCOB
{
	// Function declarations
	void LoadMesh(Packet::PackClass *p);

	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aMmCOB::Init,"Init");
		apollo.SubmitFunction(aMmCOB::Kill,"Kill");

		apollo.SubmitFunction(aMmCOB::LoadMesh,"LoadMesh");
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
	}

	void LoadMesh(Packet::PackClass *p)
	{
		MeshType *mesh = (MeshType *)(void *)p->GetData(D_ULONG);
		LoadCOB(mesh);
	}
}
