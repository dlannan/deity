// Utility functions 

#include "..\\..\\System\\System.h"

#include "aRenderDX8.h"
#include "Primitive.h"
#include "RenderVertex.h"
#include "Utils.h"

namespace aRenderDX8
{
	// ****************************************************************************
	// Util Functions - Probably should move to another file

	inline unsigned int GetShaderType(unsigned int format)
	{
		unsigned int type = 0;
		if(format & VERTEX_COLOR)
		{
			type |= RENDER_SHADER_COLOR;
		}
		if(format & VERTEX_XYZ)
		{
			type |= RENDER_SHADER_XYZ;
		}
		if(format & VERTEX_NORMAL)
		{
			type |= RENDER_SHADER_NORMAL;
		}
		if(format & VERTEX_TEX1)
		{
			type |= RENDER_SHADER_TEX1;
		}

		return type;
	}

	inline unsigned int GetShaderSize(unsigned int format)
	{
		unsigned int sz = 0;
		if(format & VERTEX_COLOR)
		{
			sz += sizeof(ColorChar4);
		}
		if(format & VERTEX_XYZ)
		{
			sz += sizeof(Vertex3);
		}
		if(format & VERTEX_NORMAL)
		{
			sz += sizeof(Vertex3);
		}
		if(format & VERTEX_TEX1)
		{
			sz += sizeof(Vector2);
		}

		return sz;
	}

	// Copy the verts into the vert buffer
	void FillVertexBuffer(VertexPtrType *v)
	{
		unsigned int format = v->vbuff.shader;
		VertexType *ptr = v->vptr;

		VOID* pVerts;
		if(D3D_OK == v->vbuff.dxptr->Lock( 0, v->vbuff.size * v->vbuff.max, (BYTE**)&pVerts, D3DLOCK_DISCARD ))
		{
			unsigned char *pts = (unsigned char *)pVerts;
			for(int i=0; i<v->verts; i++)
			{
				ptr = &v->vptr[i];

				if(format & RENDER_SHADER_XYZ)
				{
					*((Vertex3 *)pts) = ptr->p;
					pts += sizeof(Vertex3);
				}
				if(format & RENDER_SHADER_NORMAL)
				{
					*((Vertex3 *)pts) = ptr->n;
					pts += sizeof(Vertex3);
				}
				if(format & RENDER_SHADER_COLOR)
				{
					*((ColorChar4 *)pts) = ptr->c;
					pts += sizeof(ColorChar4);
				}
				if(format & RENDER_SHADER_TEX1)
				{
					*((Vector2 *)pts) = ptr->t;
					pts += sizeof(Vector2);
				}
			}

		v->vbuff.dxptr->Unlock();
		}
	}

	void BuildWindowVerts(VertexPtrType *v)
	{
	}

	void BuildFontVerts(VertexPtrType *v)
	{
		v->mode = RENDER_TRILIST;	// default tri render mode

		v->vbuff.shader = GetShaderType(VERTEX_XYZ | VERTEX_COLOR | VERTEX_TEX1);
		v->vbuff.size = GetShaderSize(VERTEX_XYZ | VERTEX_COLOR | VERTEX_TEX1);
		v->vbuff.max = 512 * 6;					// 512 chars of 6 verts each

		v->vptr = new VertexType[v->vbuff.max];
		v->verts = v->vbuff.max;

		for(int i=0; i<512 * 6; i+=6)
		{
			v->vptr[0+i].p.x = 0.0f;v->vptr[0+i].p.y = 0.0f;v->vptr[0+i].p.z = 0.0f;
			v->vptr[0+i].c.r = 1.0f;v->vptr[0+i].c.g = 1.0f;v->vptr[0+i].c.b = 1.0f;v->vptr[0+i].c.a = 1.0f;
			v->vptr[0+i].t.u = 0.0f;v->vptr[0+i].t.v = 0.0f;

			v->vptr[1+i].p.x = 0.0f;v->vptr[1+i].p.y = 1.0f;v->vptr[1+i].p.z = 0.0f;
			v->vptr[1+i].c.r = 1.0f;v->vptr[1+i].c.g = 1.0f;v->vptr[1+i].c.b = 1.0f;v->vptr[1+i].c.a = 1.0f;
			v->vptr[1+i].t.u = 0.0f;v->vptr[1+i].t.v = 1.0f;

			v->vptr[2+i].p.x = 1.0f;v->vptr[2+i].p.y = 0.0f;v->vptr[2+i].p.z = 0.0f;
			v->vptr[2+i].c.r = 1.0f;v->vptr[2+i].c.g = 1.0f;v->vptr[2+i].c.b = 1.0f;v->vptr[2+i].c.a = 1.0f;
			v->vptr[2+i].t.u = 1.0f;v->vptr[2+i].t.v = 0.0f;

			v->vptr[3+i].p.x = 1.0f;v->vptr[3+i].p.y = 0.0f;v->vptr[3+i].p.z = 0.0f;
			v->vptr[3+i].c.r = 1.0f;v->vptr[3+i].c.g = 1.0f;v->vptr[3+i].c.b = 1.0f;v->vptr[3+i].c.a = 1.0f;
			v->vptr[3+i].t.u = 1.0f;v->vptr[3+i].t.v = 0.0f;

			v->vptr[4+i].p.x = 0.0f;v->vptr[4+i].p.y = 1.0f;v->vptr[4+i].p.z = 0.0f;
			v->vptr[4+i].c.r = 1.0f;v->vptr[4+i].c.g = 1.0f;v->vptr[4+i].c.b = 1.0f;v->vptr[4+i].c.a = 1.0f;
			v->vptr[4+i].t.u = 0.0f;v->vptr[4+i].t.v = 1.0f;

			v->vptr[5+i].p.x = 1.0f;v->vptr[5+i].p.y = 1.0f;v->vptr[5+i].p.z = 0.0f;
			v->vptr[5+i].c.r = 1.0f;v->vptr[5+i].c.g = 1.0f;v->vptr[5+i].c.b = 1.0f;v->vptr[5+i].c.a = 1.0f;
			v->vptr[5+i].t.u = 1.0f;v->vptr[5+i].t.v = 1.0f;
		}
	}
}