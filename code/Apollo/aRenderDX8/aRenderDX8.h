#ifndef _aRenderDX8_H_
#define _aRenderDX8_H_

#include <D3DX8.h>
#include <D3d8.h>
#include <D3dx8math.h>
#include <vector>

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include "..\\..\\System\\System.h"

#include "RenderVertex.h"

// Dont forget to add includes if necessary!

extern ApolloClass	apollo;

namespace aRenderDX8 
{
	// Function declarations
	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();

	// All internal data should/must be private, there is NO acces allowed 
	// except through the BaseCommClass interface.. so please protect it!

	// Special functions - these are the functions that recieve packets from external transfers
	void PrepareFontMesh(Packet::PackClass *in);
	void PrepareWindowMesh(Packet::PackClass *in);

	void SetMeshOrient(Packet::PackClass *in);
	void MultMeshTransform(Packet::PackClass *in);
	void SetMeshTransform(Packet::PackClass *in);
	void SetMeshMaterial(Packet::PackClass *in);

	void Broadcast(Packet::PackClass *p);
	void InitRender(Packet::PackClass *p);
	void GetRenderDevice(Packet::PackClass *p);
	void AddMesh(Packet::PackClass *p);
	void DoRenderFrame(Packet::PackClass *p);
	void RenderMesh(Packet::PackClass *p);
	void SetCamera(Packet::PackClass *p);
	void SetProjection(Packet::PackClass *p);
	void SetMeshTransform(Packet::PackClass *p);

	int PrepareMesh(void *,unsigned int,int, unsigned int);
	void RenderMesh(int index);

	void SetCamera(D3DXMATRIX &mat);
	void SetProjection(D3DXMATRIX &mat);
	void SetScreenMode(Packet::PackClass *p);
	void ReSize(Packet::PackClass *p);
	void ScreenMode(int width, int height, int depth, int window);
	void SetInstance(Packet::PackClass *in);

	void InitDirect3D(void);
	void RenderBegin();
	void RenderEnd();
	void DoRenderFrame(void);
}



#endif
