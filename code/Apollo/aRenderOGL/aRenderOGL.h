
#ifndef _aRenderOGL_H_
#define _aRenderOGL_H_

//#include <D3DX8.h>
//#include <D3d8.h>
#include <windows.h>
#include <gl/gl.h>

#include <vector>

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!
#include "..\\aMaterial\\aMaterial.h"

extern ApolloClass	apollo;

namespace aRenderOGL 
{
	typedef struct 
	{
		FLOAT	x, y, z;	// The transformed position for the vertex.
//		FLOAT	nx, ny, nz;	// normal
		D3DCOLOR color;		// The vertex color.
		FLOAT	u, v;
	} VertexType;
	typedef VertexType CUSTOMVERTEX ;

	typedef struct 
	{
		void *	vptr;
		int		num;
		int		max;
		int		vertsInBuff;
		CUSTOMVERTEX*		glptr;
		aMaterial::MaterialType*		mat;
	} VertexPtrType;

	typedef struct
	{
		VertexPtrType	*mesh;
		aMaterial::MaterialType *mat;
		D3DXMATRIX		matrix;	// orientation and position matrix

		// Could have all sorts here - blend mode, bumpmapping, texture
	} RenderPtrType;

	enum {	RENDER_TRILIST,
			RENDER_TRIFAN,
			RENDER_TRISTRIP,
			RENDER_POINT,
			RENDER_LINELIST,
			RENDER_LINESTRIP  };

	// All internal data should/must be private, there is NO acces allowed 
	// except through the BaseCommClass interface.. so please protect it!

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();

	// Special functions - these are the functions that recieve packets from external transfers
	void Broadcast(Packet::PackClass *p);
	void InitRender(Packet::PackClass *p);
	void GetRenderDevice(Packet::PackClass *p);
	void AddMesh(Packet::PackClass *p);
	void DoRenderFrame(Packet::PackClass *p);
	void RenderMesh(Packet::PackClass *p);
	void SetCamera(Packet::PackClass *p);
	void SetProjection(Packet::PackClass *p);

	int PrepareMesh(void *,unsigned int,unsigned int,unsigned long);
	void RenderMesh(unsigned int index, float x, float y, float z,unsigned int col);

	void SetCamera(D3DXMATRIX &mat);
	void SetProjection(D3DXMATRIX &mat);
	void SetScreenMode(Packet::PackClass *p);
	void ScreenMode(int width, int height, int depth, int window);

	void InitOpenGL(void);
	void RenderBegin();
	void RenderEnd();
	void DoRenderFrame(void);
}



#endif
