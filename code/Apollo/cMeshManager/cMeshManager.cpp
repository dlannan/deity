
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "cMeshManager.h"
#include "..\\..\\System\\System.h"

#include "MeshClass.h"

ApolloClass	apollo;

namespace cMeshManager
{

	// List of all meshes available for use
	map<int, MeshType *>		meshList;

	// Function Headers
	void LoadMesh(Packet::PackClass *p);
	void UnloadMesh(Packet::PackClass *p);
	void StripMesh(Packet::PackClass *p);
	void AddRenderMesh(Packet::PackClass *p);
	void RenderMesh(Packet::PackClass *p);
	void SetMeshTransform(Packet::PackClass *p);
	void MultMeshTransform(Packet::PackClass *p);

	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(cMeshManager::Init,"Init");
		apollo.SubmitFunction(cMeshManager::Kill,"Kill");

		// We get and send meshes from here
		apollo.SubmitFunction(cMeshManager::LoadMesh,"LoadMesh");
		apollo.SubmitFunction(cMeshManager::UnloadMesh,"UnloadMesh");
		
		apollo.SubmitFunction(cMeshManager::StripMesh,"StripMesh");

		// We can call a render here, which submits a mesh for this current frame
		apollo.SubmitFunction(cMeshManager::AddRenderMesh,"AddRenderMesh");
		apollo.SubmitFunction(cMeshManager::RenderMesh,"RenderMesh");

		apollo.SubmitFunction(cMeshManager::SetMeshTransform,"SetMeshTransform");
		apollo.SubmitFunction(cMeshManager::MultMeshTransform,"MultMeshTransform");
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
	}

	// Use this to cleanup anything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		// Clean up the mesh list
		map<int,MeshType *>::iterator i = meshList.begin();
		while(i!=meshList.end())
		{
			delete i->second;
			i++;
		}

		meshList.clear();
	}

	void LoadMesh(Packet::PackClass *p)
	{
		// Get the filename
		MeshType	*mesh = new MeshType;

		mesh->name = p->GetData(D_STRING);

		// Get the file type and see if there is a matching target Module
		// Could have the module name in the packet as well?
		char	ext[10];
		strcpy(ext, mesh->name.String()+mesh->name.Size()-3);

		Data::StringClass  loadername;
		loadername << "MM_";
		loadername << (const char *)ext;
		loadername << ".LoadMesh";

		{
		Packet("MeshManager",loadername.String());
		Padd (unsigned long)mesh;		// The loader module puts everything here
		Psend;
		}

		unsigned int val = CRC::Int(meshList.size()+1);

		meshList[val] = mesh;

		p->Clear();
		(*p) += (int)(val);
	}

	void UnloadMesh(Packet::PackClass *p)
	{
	}

	void StripMesh(Packet::PackClass *p)
	{
	}

	void SetMeshTransform(Packet::PackClass *p)
	{
		// Send the lot - no optimisation at all
		int meshId = p->GetData(D_INT);

		Matrix *temp = (Matrix *)p->GetData(D_ULONG);
		MeshType *mesh = meshList[meshId];

		for(vector<FacetObject *>::iterator i=mesh->Objects()->begin(); i<mesh->Objects()->end();i++)
		{
			// Prep the local->world transform for this mesh
			{
			Packet("MeshManager","Render.SetMeshTransform");
			Padd (int)(*i)->GetRenderHandle();
			Padd (unsigned long)temp;
			Psend;
			}
		}
	}

	void MultMeshTransform(Packet::PackClass *p)
	{
		// Send the lot - no optimisation at all
		int meshId = p->GetData(D_INT);

		Matrix *mul = (Matrix *)p->GetData(D_ULONG);
		MeshType *mesh = meshList[meshId];

		for(vector<FacetObject *>::iterator i=mesh->Objects()->begin(); i<mesh->Objects()->end();i++)
		{
			// Prep the local->world transform for this mesh
			{
			Packet("MeshManager","Render.SetMeshTransform");
			Padd (int)(*i)->GetRenderHandle();
			Padd (unsigned long)&(*i)->trans;
			Psend;
			}

			{
			Packet("MeshManager","Render.MultMeshTransform");
			Padd (int)(*i)->GetRenderHandle();
			Padd (unsigned long)mul;
			Psend;
			}
		}
	}

	void RenderMesh(Packet::PackClass *p)
	{
		// Send the lot - no optimisation at all
		int meshId = p->GetData(D_INT);

		MeshType *mesh = meshList[meshId];

		for(vector<FacetObject *>::iterator i=mesh->Objects()->begin(); i<mesh->Objects()->end();i++)
		{
			Packet("MeshManager","Render.RenderMesh");
			Padd (int)(*i)->GetRenderHandle();
			Padd -0.0f; Padd -0.0f;Padd 0.0f;
			Padd (unsigned int)0xFFFFFFFF;
			Psend;
		}
	}

	// Our mesh structure is currently a full bastard - must fix one day
	void AddRenderMesh(Packet::PackClass *p)
	{
		// We submit the mesh to be rendered in the next frame
		// If we already have the vertices packed and ready to go, just resend them.

		// Send the lot - no optimisation at all
		int meshId = p->GetData(D_INT);

		MeshType *mesh = meshList[meshId];
		VertexType		*renderVerts = NULL;
		unsigned int	vertcounter = 0;
		int				matId = -1;

		// Iterate through objects
		for(vector<FacetObject *>::iterator i=mesh->Objects()->begin(); i<mesh->Objects()->end();i++)
		{
			vertcounter = 0;
			renderVerts = new VertexType[(*i)->GetFacetCount() * 3];

			VertType *verts = (*i)->GetVerts();
			// For every object iterate all facets
			FacetType *facets = (*i)->GetFacets();
			unsigned int count = (*i)->GetFacetCount();
			for(int j=0; j<count; j++)
			{
				for(int k=0; k<3; k++)
				{
					VertexType *v = &renderVerts[vertcounter++];
					v->p.x = verts[facets[j].vert[k]].pos.x;
					v->p.y = verts[facets[j].vert[k]].pos.y;
					v->p.z = verts[facets[j].vert[k]].pos.z;

					v->c = facets[j].col[k];

					v->t.u = facets[j].uv[k].u;
					v->t.v = facets[j].uv[k].v;

					matId = facets[j].matId;
				}
			}

			(*i)->SetRenderVerts(renderVerts,vertcounter);

			// Send to renderer
			{
			Packet("MeshManager","Render.AddMesh");
			Padd (unsigned long)renderVerts;		// vert data
			Padd (unsigned int) vertcounter;			
			Padd (int)matId;							// associated material index - wgaf
			Padd (unsigned int) VERTEX_XYZ | VERTEX_COLOR | VERTEX_TEX1;		
			Psend;

			// Grab the mesh handle
			(*i)->SetRenderHandle(packet.GetDataIndex(0,D_INT));
			}

			// Prep the orient transform for this mesh
			{
			Matrix temp = (*i)->matrix;
			Packet("MeshManager","Render.SetMeshOrient");
			Padd (int)(*i)->GetRenderHandle();
			Padd (unsigned long)&temp;
			Psend;
			}

			// Prep the local->world transform for this mesh
			{
			Matrix temp = (*i)->trans;
			Packet("MeshManager","Render.SetMeshTransform");
			Padd (int)(*i)->GetRenderHandle();
			Padd (unsigned long)&temp;
			Psend;
			}
		}
	}
}
