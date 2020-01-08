#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_

// Our own mesh format. Mainly optimised for large vertex list rendering

#include "..\\..\\System\\System.h"

	typedef struct{
		float x,y,z;
		unsigned int	col;
		float u,v;
	} vert_struct;

	struct MeshBounds
	{
		Vector4	min;
		Vector4	max;
	};

	struct VertType
	{
		unsigned int	boneId;
		Vector4			pos;
	};

	class VertListType
	{
	public:
		VertType		*verts;
		unsigned int	count;

		VertListType() { verts = NULL; count = 0; }
		~VertListType() { if(verts) delete [] verts; }
	};

	// Facet is always assumed to be a triangle
	struct FacetType		
	{
		// Material index id
		int				matId;		// negative value indicates invalid material!

		unsigned int	col[3];		// Colour per vertex
		Vector4			norm[3];	// Normal per vertex
		Vector2			uv[3];		// Tex coords per vertex
		unsigned int	vert[3];	// indexes into the vertex list
	};

	class FacetListType
	{
	public:
		FacetType		*facets;
		unsigned int	count;

		FacetListType() { facets = NULL; count = 0; }
		~FacetListType() { if(facets) delete [] facets; }
	};
	
	class FacetObject
	{
	private:
		FacetListType		facets;	
		VertListType		verts;

		CRC::StringClass	label;

		int					renderHandle;
		
		// These verts go to the renderer
		VertexType			*renderVerts;		
		unsigned int		renderVertsCount;

	public:
		// Group parent Id - use this for inheriting space changes
		unsigned int	parentId;
		unsigned int	polyId;

		// Position and orientation for the verts
		Matrix44		matrix;
		// Local to world transformation matrix
		Matrix44		trans;	

		// Bounding box for the verts
		MeshBounds		bounds;

		// Any data a user may want to associate with a mesh Object
		// Multiple data sections can be attached to a mesh Object
		// Each data section is added using a label - and can be retrieved the same way.
		//  ie. userdata["MyStuff"] = mystuff;  getmystuff = userdata["MyStuff"];
		map<const char *,void	*>	userdata;

		FacetObject()
		{
			parentId = 0;		// Set to root or Null parent
			userdata.clear();
			
			renderHandle = -1;
			renderVerts = NULL;
			renderVertsCount = 0;
		}

		~FacetObject()
		{
			if(facets.facets) delete [] facets.facets;
			facets.count = 0;

			if(verts.verts) delete [] verts.verts;
			verts.count = 0;

			map<const char *,void *>::iterator i = userdata.begin();
			while(i!=userdata.end())
			{
				if( i->second ) delete i->second;
				i++;
			}
			userdata.clear();
		}

		// Make new space for facets - to be filled out later
		FacetType *NewFacets(int _count)
		{
			ASSERT( _count <= 0, "invalid facets pointer or facet count");
			if(facets.count > 0)
			{
				delete [] facets.facets;
				facets.count = 0;
			}

			facets.count = _count;
			facets.facets = new FacetType[_count];
			return facets.facets;
		}

		// Make new space for verts - to be filled out later
		VertType * NewVerts(int _count)
		{
			ASSERT( _count <= 0, "invalid vert pointer or vert count");
			if(verts.count > 0)
			{
				delete [] verts.verts;
				verts.count = 0;
			}
			
			verts.count = _count;
			verts.verts = new VertType[_count];
			return verts.verts;
		}


		// Make new space for facets - to be filled out later
		FacetType * SetFacets(FacetType *_facets, int _count)
		{
			ASSERT( (_facets == NULL) || (_count <= 0), "invalid facets pointer or facet count");
			if(facets.count > 0)
			{
				delete [] facets.facets;
				facets.count = 0;
			}

			facets.count = _count;
			facets.facets = _facets;
			return _facets;
		}

		// Make new space for verts - to be filled out later
		VertType * SetVerts(VertType *_verts, int _count)
		{
			ASSERT( (_verts == NULL) || (_count <= 0), "invalid vert pointer or vert count");
			if(verts.count > 0)
			{
				delete [] verts.verts;
				verts.count = 0;
			}
			
			verts.count = _count;
			verts.verts = _verts;
			return _verts;
		}


		inline FacetType *GetFacets() { return facets.facets; }
		inline unsigned int GetFacetCount() { return facets.count; }

		inline VertType *GetVerts() { return verts.verts; }
		inline unsigned int GetVertCount() { return verts.count; }

		void SetLabel(const char *_label) {	label.Clear(); label = _label; }
		CRC::StringClass GetLabel()	{ return label;	}

		VertexType *RenderVerts() { return renderVerts; }
		void SetRenderVerts(VertexType *rverts, unsigned int maxverts)
		{
			if( (renderVertsCount > 0) && (renderVerts != NULL) ) 
			{
				delete [] renderVerts;
			}

			renderVerts = rverts;
			renderVertsCount = maxverts;
		}

		inline int GetRenderHandle() { return renderHandle; }
		inline void SetRenderHandle(int handle) { renderHandle = handle; }

	};

	class MeshGroup
	{
	private:
		// Node links
		CRC::StringClass	label;

		unsigned int		gid;
	public:
		// Position and orientation for the group
		Matrix44		matrix;
		// Local to world transformation matrix for the group
		Matrix44		trans;	

		// Bounding box for the group
		MeshBounds		bounds;

		MeshGroup() 
		{ 
			// default facet object
			gid = 0;
		}

		// Deletes all nodes downwards
		~MeshGroup() 
		{ 
		}

		void SetLabel(const char *_label) { ASSERT(_label==NULL,"Invalid label"); label.Clear(); label = _label; }
		CRC::StringClass Label() { return label; }

		void SetGID(unsigned int _id) { gid = _id; }
		unsigned int GID() { return gid; }
	};


	class MeshType 
	{
	private:
		// Group list - gid mapped
		map<unsigned int, MeshGroup *>	grouplist;

		// Object list
		vector<FacetObject *>			objs;
	public:
		// Global mesh name/id
		CRC::StringClass	name;

		MeshType()
		{
			grouplist[0] = NULL;
		}

		~MeshType()
		{
			if(!grouplist.empty())
			{
				map<unsigned int, MeshGroup *>::iterator i = grouplist.begin();
				while(i!=grouplist.end())
				{
					if(i->second) delete i->second;
					i++;
				}
				grouplist.clear();
			}
		}

		MeshGroup *FindGroup(unsigned int gid)
		{
			map<unsigned int, MeshGroup *>::iterator curr = grouplist.find(gid);
			if(curr == grouplist.end())
				return NULL;
			else
				return curr->second;
		}
	
		MeshGroup *NewGroup(unsigned int gid)
		{
			ASSERT(gid == 0, "New Group is a root group?!");
			MeshGroup *newgroup = new MeshGroup;

			// Check to see if this gid hasnt already been added - remove previous group if so
			map<unsigned int, MeshGroup *>::iterator fgroup = grouplist.find(gid);
			if(fgroup != grouplist.end()) delete grouplist[gid];

			grouplist[gid] = newgroup;
			return newgroup;
		}

		MeshGroup *AddGroup(MeshGroup *_mob, unsigned int gid)
		{
			ASSERT(gid == 0, "Add Group is a root group?!");
			ASSERT(_mob == NULL, "invalid mesh object group pointer");

			// Check to see if this gid hasnt already been added - remove previous group if so
			map<unsigned int, MeshGroup *>::iterator fgroup = grouplist.find(gid);
			if(fgroup != grouplist.end()) delete grouplist[gid];
			
			grouplist[gid] = _mob;
			return _mob;
		}

		FacetObject *LastObject() 
		{ 
			return objs.at(objs.size()-1); 
		}

		FacetObject *FirstObject() 
		{ 
			return objs.at(0); 
		}

		FacetObject *NewObject() 
		{ 
			FacetObject *newobj = new FacetObject; 
			objs.push_back(newobj); 
			return newobj; 
		}

		FacetObject *Object(int index) 
		{ 
			if(objs.size() > 0) return objs.at(index); 
			else return NULL;
		}

		vector<FacetObject *> *Objects() 
		{ 
			return &objs; 
		}
	}; 

#endif