
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "aSpaceRouter.h"
#include "..\\..\\System\\System.h"

ApolloClass		apollo;

namespace aSpaceRouter
{
	// Universe is the root node - and has maximum magnitude
	SpatialNode		*universe;	

	// Function declarations
	void AddObjectByIndex(Packet::PackClass *p);
	void AddObjectByName(Packet::PackClass *p);
	void RemoveObject(Packet::PackClass *p);
	void UpdateObject(Packet::PackClass *p);

	void FindByType(Packet::PackClass *p);
	void FindByObject(Packet::PackClass *p);
	void FindByModule(Packet::PackClass *p);

	void RouteByType(Packet::PackClass *p);
	void RouteByObject(Packet::PackClass *p);
	void RouteByModule(Packet::PackClass *p);
	void RouteByRadius(Packet::PackClass *p);
	void RouteByRayTest(Packet::PackClass *p);
	void RouteByFrustum(Packet::PackClass *p);
	void RouteByBBox(Packet::PackClass *p);

	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aSpaceRouter::Init,"Init");
		apollo.SubmitFunction(aSpaceRouter::Kill,"Kill");

		apollo.SubmitFunction(aSpaceRouter::AddObjectByIndex,"AddObjectByIndex");
		apollo.SubmitFunction(aSpaceRouter::AddObjectByName,"AddObjectByName");
		apollo.SubmitFunction(aSpaceRouter::RemoveObject,"RemoveObject");
		apollo.SubmitFunction(aSpaceRouter::UpdateObject,"UpdateObject");

		apollo.SubmitFunction(aSpaceRouter::FindByType,"FindByType");
		apollo.SubmitFunction(aSpaceRouter::FindByModule,"FindByModule");
		apollo.SubmitFunction(aSpaceRouter::FindByObject,"FindByObject");

		apollo.SubmitFunction(aSpaceRouter::RouteByType,"RouteByType");
		apollo.SubmitFunction(aSpaceRouter::RouteByObject,"RouteByObject");
		apollo.SubmitFunction(aSpaceRouter::RouteByModule,"RouteByModule");
		apollo.SubmitFunction(aSpaceRouter::RouteByRadius,"RouteByRadius");
		apollo.SubmitFunction(aSpaceRouter::RouteByRayTest,"RouteByRayTest");
		apollo.SubmitFunction(aSpaceRouter::RouteByFrustum,"RouteByFrustum");
		apollo.SubmitFunction(aSpaceRouter::RouteByBBox,"RouteByBBox");
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
		// Universe is the root node - and has maximum magnitude
		universe = new SpatialNode;
		universe->child1 = NULL;
		universe->child2 = NULL;
		universe->child3 = NULL;
		universe->child4 = NULL;
		universe->child5 = NULL;
		universe->child6 = NULL;
		universe->child7 = NULL;
		universe->child8 = NULL;
		universe->parent = NULL;
		universe->objects.clear();
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		// Destroy all the nodes including data
	}

	// Rebuild the tree hierarchy from this node down.
	void Rebuild(SpatialNode *sn)
	{
	}

	// Place a new object into the space
	void AddObjectByIndex(Packet::PackClass *p)
	{
		// Make some temp vars for the info
		Object *obj = new Object;

		p->GetDataStart();

		obj->ownerModule = p->srccrc.GetMod().String();
		obj->ownerObject.SetId(p->GetData(D_UINT));

		obj->type = p->GetData(D_STRING);
		obj->mag = p->GetData(D_UINT);

		obj->origin.x = p->GetData(D_FLOAT);
		obj->origin.y = p->GetData(D_FLOAT);
		obj->origin.x = p->GetData(D_FLOAT);
		
		obj->min.x = p->GetData(D_FLOAT);
		obj->min.y = p->GetData(D_FLOAT);
		obj->min.z = p->GetData(D_FLOAT);

		obj->max.x = p->GetData(D_FLOAT);
		obj->max.y = p->GetData(D_FLOAT);
		obj->max.z = p->GetData(D_FLOAT);

		obj->radius = p->GetData(D_FLOAT);
	}

	// Place a new object into the space
	void AddObjectByName(Packet::PackClass *p)
	{
		// Make some temp vars for the info
		Object *obj = new Object;

		p->GetDataStart();

		obj->ownerModule = p->srccrc.GetMod().String();
		obj->ownerObject = p->GetData(D_STRING);

		obj->type = p->GetData(D_STRING);
		obj->mag = p->GetData(D_UINT);

		obj->origin.x = p->GetData(D_FLOAT);
		obj->origin.y = p->GetData(D_FLOAT);
		obj->origin.x = p->GetData(D_FLOAT);
		
		obj->min.x = p->GetData(D_FLOAT);
		obj->min.y = p->GetData(D_FLOAT);
		obj->min.z = p->GetData(D_FLOAT);

		obj->max.x = p->GetData(D_FLOAT);
		obj->max.y = p->GetData(D_FLOAT);
		obj->max.z = p->GetData(D_FLOAT);

		obj->radius = p->GetData(D_FLOAT);
	}


	// Remove an object from the space
	void RemoveObject(Packet::PackClass *p)
	{}

	// Update an objects information - this could cause the spatialiser to move its node
	// in the heirarchy
	void UpdateObject(Packet::PackClass *p)
	{}

	// Find a node - should pass a start point in the heirarchy or universe is used - bad.
	void FindByType(Packet::PackClass *p)
	{}

	// Find a node - should pass a start point in the heirarchy or universe is used - bad.
	void FindByObject(Packet::PackClass *p)
	{}

	// Find a node - should pass a start point in the heirarchy or universe is used - bad.
	void FindByModule(Packet::PackClass *p)
	{}

	// Route a packet to a destination based on a Type test
	void RouteByType(Packet::PackClass *p)
	{}

	// Route a packet to a destination based on a Object test
	void RouteByObject(Packet::PackClass *p)
	{}

	// Route a packet to a destination based on a Module test
	void RouteByModule(Packet::PackClass *p)
	{}

	// Route a packet to a destination based on a Radius test
	void RouteByRadius(Packet::PackClass *p)
	{}

	// Route a packet to a destination based on a Raytest test
	void RouteByRayTest(Packet::PackClass *p)
	{}

	// Route a packet to a destination based on a Frustum test
	void RouteByFrustum(Packet::PackClass *p)
	{}

	// Route a packet to a destination based on a BBox test
	void RouteByBBox(Packet::PackClass *p)
	{}
}
