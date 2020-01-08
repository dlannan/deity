#ifndef _RENDER_VERTEX_H_
#define _RENDER_VERTEX_H_

// Vertex building systems - use these for vertex submissions to the renderer
#include <D3d8.h>

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

namespace aRenderDX8
{
	// We have a 1000 vert element maximum as the default vertex buffer size
	#define DEFAULT_VERTEX_BUF_SIZE		1000

	// Render Mode
	enum {	
			RENDER_INVALID =		0,
			RENDER_TRILIST	=		1,
			RENDER_TRIFAN =			2,
			RENDER_TRISTRIP =		3,
			RENDER_POINT =			4,
			RENDER_LINELIST =		5,
			RENDER_LINESTRIP =		6
		};

	#define		RENDER_SHADER_COLOR			D3DFVF_DIFFUSE
	#define		RENDER_SHADER_XYZ			D3DFVF_XYZ
	#define		RENDER_SHADER_NORMAL		D3DFVF_NORMAL
	#define		RENDER_SHADER_TEX1			D3DFVF_TEX1
	#define		RENDER_SHADER_TEX2			D3DFVF_TEX2				// Multi-texture
	#define		RENDER_SHADER_TEX3			D3DFVF_TEX3				// Multi-texture
	

	// Status flags - helps for checking vertex rendering state
	#define		RENDER_STATUS_MODIFIED		0x0001		// The vertex data has been modified
	#define		RENDER_STATUS_REQRENDER		0x0002		// A render request is pending
	#define		RENDER_STATUS_RENDERING		0x0004		// Currently in rendering buffer
	#define		RENDER_STATUS_NEWMODE		0x0010		// A mode hase been modified
	#define		RENDER_STATUS_NEWTRANS		0x0020		// trans matrix has been modified
	#define		RENDER_STATUS_NEWORIENT		0x0040		// orient matrix has been modified
	#define		RENDER_STATUS_VERTS			0x0080		// we have verts
	#define		RENDER_STATUS_VPTR			0x0100		// vert pointer is filled

	// Output buffer for DX
	typedef struct
	{
		// dxvert buffer
		IDirect3DVertexBuffer8*		dxptr;			
		unsigned int				max;		// should be DEFAULT_VERTEX_BUF_SIZE

		// Index buffer - more efficient for larger poly scenes
		IDirect3DIndexBuffer8*		dxptrI;
		unsigned int				maxI;

		// How big a vertex element is - the shader determines this
		unsigned int				size;	
		// Shader flags that tell us the make up of the vertbuffer
		unsigned int				shader;
	} VertBufferType;

	// Our vertex stored info
	typedef struct 
	{
		// How the vertexbuffer should be rendered - tris, strips, lines etc
		unsigned int				mode;

		// status mask - various flags - modified, in renderlist, matrix changed, etc
		unsigned int				status;

		// material index in Material module
		int							mat;	

		Matrix						trans;
		Matrix						orient;

		// List of vertices
		VertexType					*vptr;	
		unsigned int				verts;

		// Current render buffer
		VertBufferType				vbuff;
		unsigned int				index;		// where it starts in the vbuff
		unsigned int				length;		// size of elements in vbuff

	} VertexPtrType;

	typedef struct
	{
		vector<VertexPtrType *>		vlist;
	} RenderPtrType;

}
#endif
