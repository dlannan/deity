#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

// 2D & 3D Primitve Types

// All built specifically for an interface system - ie menus.

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include "aRenderDX8.h"

namespace aRenderDX8
{
	extern 	map<int, VertexPtrType *>				vlist;
	extern 	map<int, RenderPtrType *>				rlist;

	extern	IDirect3DDevice8*		g_pd3dDevice;

	// Primitive Type helper structure
	typedef struct
	{
		int				id;		// vlist uid
		VertexPtrType	*v;		// Vertex buffer
		int				*i;		// Index buffer
	}
	PrimType;

	void PrepPrimMeshes();

	void SubmitPrimFunctions();

	// 2D primitives
	void PrimFontChar(Packet::PackClass *p);

	void PrimCircle(Packet::PackClass *p);
	void PrimSquare(Packet::PackClass *p);
	void PrimEllipse(Packet::PackClass *p);
	void PrimTriangle(Packet::PackClass *p);
	void PrimRectangle(Packet::PackClass *p);
	void PrimLine(Packet::PackClass *p);
	void PrimPoint(Packet::PackClass *p);
	void PrimArc(Packet::PackClass *p);

	// 3D primitives
	void PrimCube(Packet::PackClass *p);
	void PrimPyramid(Packet::PackClass *p);
	void PrimSphere(Packet::PackClass *p);
	void PrimCone(Packet::PackClass *p);
	void PrimTube(Packet::PackClass *p);
	void PrimPlane(Packet::PackClass *p);
}

#endif