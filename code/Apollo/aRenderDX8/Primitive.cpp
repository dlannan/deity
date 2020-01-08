// 2D & 3D Primitve Types

// All built specifically for an interface system - ie menus.

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include "aRenderDX8.h"
#include "Primitive.h"
#include "Utils.h"

extern ApolloClass		apollo;

namespace aRenderDX8
{
	#define			MAX_RECT_VERTICES		512 * 4
	#define			MAX_CIRCLE_VERTICES		64 * 10
	#define			MAX_LINE_VERTICES		512 * 2

	#define			MAX_CUBE_VERTICES		64 * 8
	#define			MAX_SPHERE_VERTICES		16 * 100
	#define			MAX_TUBE_VERTICES		16 * 30
	#define			MAX_PLANE_VERTICES		64 * 4


	PrimType	rect;
	PrimType	circle;
	PrimType	line;
	PrimType	cube;

	PrimType	sphere;
	PrimType	tube;
	PrimType	plane;

	void SubmitPrimFunctions()
	{
		apollo.SubmitFunction(aRenderDX8::PrimFontChar,"PrimFontChar");
		
		apollo.SubmitFunction(aRenderDX8::PrimCircle,"PrimCircle");
		apollo.SubmitFunction(aRenderDX8::PrimSquare,"PrimSquare");
		apollo.SubmitFunction(aRenderDX8::PrimEllipse,"PrimEllipse");
		apollo.SubmitFunction(aRenderDX8::PrimTriangle,"PrimTriangle");
		apollo.SubmitFunction(aRenderDX8::PrimRectangle,"PrimRectangle");
		apollo.SubmitFunction(aRenderDX8::PrimLine,"PrimLine");
		apollo.SubmitFunction(aRenderDX8::PrimPoint,"PrimPoint");
		apollo.SubmitFunction(aRenderDX8::PrimArc,"PrimArc");

		apollo.SubmitFunction(aRenderDX8::PrimCube,"PrimCube");
		apollo.SubmitFunction(aRenderDX8::PrimPyramid,"PrimPyramid");
		apollo.SubmitFunction(aRenderDX8::PrimSphere,"PrimSphere");
		apollo.SubmitFunction(aRenderDX8::PrimCone,"PrimCone");
		apollo.SubmitFunction(aRenderDX8::PrimTube,"PrimTube");
		apollo.SubmitFunction(aRenderDX8::PrimPlane,"PrimPlane");
	}

	void PrepPrimRectangle()
	{
		HRESULT			err;

		// Rectangle, Square, and Triangle vertex list 
		rect.v = new VertexPtrType;

		rect.v->mode = RENDER_TRILIST;	// default tri render mode

		rect.v->vbuff.shader = GetShaderType(VERTEX_XYZ | VERTEX_COLOR | VERTEX_TEX1);
		rect.v->vbuff.size = GetShaderSize(VERTEX_XYZ | VERTEX_COLOR | VERTEX_TEX1);
		rect.v->vbuff.max = MAX_RECT_VERTICES;
		
		rect.v->vptr = new VertexType[rect.v->vbuff.max];
		rect.v->verts = rect.v->vbuff.max;

		rect.v->length = 0;
		rect.v->index = 0;

		// Set the matrices to Identity
		Matrix ttrans; memset(&ttrans,0,sizeof(Matrix));
		ttrans.right.x = 1.0f; ttrans.up.y = 1.0f; ttrans.view.z = 1.0f; ttrans.pos.w = 1.0f;
		rect.v->orient = ttrans;
		rect.v->trans = ttrans;

		// Vertex buffer.. pretty straight forward really..
		err = g_pd3dDevice->CreateVertexBuffer(	rect.v->vbuff.max*rect.v->vbuff.size, D3DUSAGE_WRITEONLY,
														rect.v->vbuff.shader , D3DPOOL_DEFAULT, 
														&(rect.v->vbuff.dxptr) );
		ASSERT( FAILED(err), "Cannot create vertex buffer.");

		unsigned int val = CRC::Int(vlist.size()+1);

		vlist[val] = rect.v; // prim_rectangle
		rect.id = val;
	}

	void PrepPrimMeshes()
	{	
		PrepPrimRectangle();
	}

	// 2D primitives
	void PrimFontChar(Packet::PackClass *p)
	{
		// Place a char into the font vlist
		int index = p->GetData(D_INT);

		// Put it in the Renderlist!
		ASSERT(vlist.find(index) == vlist.end(),"Bad Render Mesh index");

		map<int,RenderPtrType *>::iterator rptr = rlist.find(vlist[index]->mat);
		// Has this font been added?
		if(rptr == rlist.end())
		{
			rlist[vlist[index]->mat] = new RenderPtrType;
			rlist[vlist[index]->mat]->vlist.push_back(vlist[index]);
			vlist[index]->length = 0;
		}

		vlist[index]->status |= RENDER_STATUS_REQRENDER;

		float x = p->GetData(D_FLOAT);
		float y = p->GetData(D_FLOAT);
		float z = p->GetData(D_FLOAT);

		float sx = p->GetData(D_FLOAT);	// Scaling factors in the x and y direction
		float sy = p->GetData(D_FLOAT);	// - mainly used for text, should be normally set to 1.0f

		float u0 = p->GetData(D_FLOAT);
		float v0 = p->GetData(D_FLOAT);
		float u1 = p->GetData(D_FLOAT);
		float v1 = p->GetData(D_FLOAT);

		unsigned int col = p->GetData(D_UINT);

		VertexPtrType	*vptr = vlist[index];

		// We should make another buffer.. and continue
		if(vptr->length >= vptr->vbuff.max) return;

		VOID* pVerts;
		if(D3D_OK == vptr->vbuff.dxptr->Lock(	vptr->length * vptr->vbuff.size,
												vptr->vbuff.max * vptr->vbuff.size, 
												(BYTE**)&pVerts, D3DLOCK_NOOVERWRITE  ))
		{
			unsigned char *pts = (unsigned char *)pVerts;
			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + sx;((Vertex3 *)pts)->y = y + sy;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + sx;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y + sy;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + sx;((Vertex3 *)pts)->y = y + sy;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			vptr->vbuff.dxptr->Unlock();
			vptr->length+=6;
		}
	}
	
	void PrimCircle(Packet::PackClass *p)
	{
	}

	void PrimSquare(Packet::PackClass *p)
	{
		rect.v->status |= RENDER_STATUS_REQRENDER;

		float x = p->GetData(D_FLOAT);
		float y = p->GetData(D_FLOAT);
		float z = p->GetData(D_FLOAT);

		float s = p->GetData(D_FLOAT);	// Scaling factors in the x and y direction

		float u0 = p->GetData(D_FLOAT);
		float v0 = p->GetData(D_FLOAT);
		float u1 = p->GetData(D_FLOAT);
		float v1 = p->GetData(D_FLOAT);

		unsigned int col = p->GetData(D_UINT);

		VertexPtrType	*vptr = rect.v;

		// We should make another buffer.. and continue
		if(vptr->length >= vptr->vbuff.max) return;

		VOID* pVerts;
		if(D3D_OK == vptr->vbuff.dxptr->Lock(	vptr->length * vptr->vbuff.size,
												vptr->vbuff.max * vptr->vbuff.size, 
												(BYTE**)&pVerts, D3DLOCK_NOOVERWRITE  ))
		{
			unsigned char *pts = (unsigned char *)pVerts;
			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + s;((Vertex3 *)pts)->y = y + s;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + s;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y + s;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + s;((Vertex3 *)pts)->y = y + s;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			vptr->vbuff.dxptr->Unlock();
			vptr->length+=6;
		}
	}

	void PrimEllipse(Packet::PackClass *p)
	{
	}

	void PrimTriangle(Packet::PackClass *p)
	{
	}

	void PrimRectangle(Packet::PackClass *p)
	{
		Profile("DX8::PrimRectangle");

		rect.v->mat = p->GetData(D_INT);

		map<int,RenderPtrType *>::iterator rptr = rlist.find(rect.v->mat);
		// Has this primitve material been added before?
		if(rptr == rlist.end())
		{
			rlist[rect.v->mat] = new RenderPtrType;
			rlist[rect.v->mat]->vlist.push_back(rect.v);
			rect.v->length = 0;
		}
		else if(( rect.v->status & RENDER_STATUS_REQRENDER ) == 0)
		{
			rlist[rect.v->mat]->vlist.push_back(rect.v);
			rect.v->length = 0;
		}
		
		rect.v->status |= RENDER_STATUS_REQRENDER;

		float x = p->GetData(D_FLOAT);
		float y = p->GetData(D_FLOAT);
		float z = p->GetData(D_FLOAT);

		float sx = p->GetData(D_FLOAT);	// Scaling factors in the x and y direction
		float sy = p->GetData(D_FLOAT);	// - mainly used for text, should be normally set to 1.0f

		float u0 = p->GetData(D_FLOAT);
		float v0 = p->GetData(D_FLOAT);
		float u1 = p->GetData(D_FLOAT);
		float v1 = p->GetData(D_FLOAT);

		unsigned int col = p->GetData(D_UINT);

		VertexPtrType	*vptr = rect.v;

		// We should make another buffer.. and continue
		if(vptr->length >= vptr->vbuff.max) return;

		VOID* pVerts;
		if(D3D_OK == vptr->vbuff.dxptr->Lock(	vptr->length * vptr->vbuff.size,
												vptr->vbuff.max * vptr->vbuff.size, 
												(BYTE**)&pVerts, D3DLOCK_NOOVERWRITE  ))
		{
			unsigned char *pts = (unsigned char *)pVerts;
			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + sx;((Vertex3 *)pts)->y = y + sy;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + sx;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v0;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x;((Vertex3 *)pts)->y = y + sy;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u0;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			((Vertex3 *)pts)->x = x + sx;((Vertex3 *)pts)->y = y + sy;((Vertex3 *)pts)->z = z; 
			pts += sizeof(Vertex3);
			((ColorChar4 *)pts)->rgba = col;
			pts += sizeof(ColorChar4);
			((Vector2 *)pts)->u = u1;((Vector2 *)pts)->v = v1;
			pts += sizeof(Vector2);

			vptr->vbuff.dxptr->Unlock();
			vptr->length+=6;
		}
	}

	void PrimLine(Packet::PackClass *p)
	{
	}

	void PrimPoint(Packet::PackClass *p)
	{
	}

	void PrimArc(Packet::PackClass *p)
	{
	}

	// 3D primitives
	void PrimCube(Packet::PackClass *p)
	{
	}

	void PrimPyramid(Packet::PackClass *p)
	{
	}

	void PrimSphere(Packet::PackClass *p)
	{
	}

	void PrimCone(Packet::PackClass *p)
	{
	}

	void PrimTube(Packet::PackClass *p)
	{
	}

	void PrimPlane(Packet::PackClass *p)
	{
	}

}