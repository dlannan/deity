
#ifndef _cJetris_H_
#define _cJetris_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

#include <D3DX8.h>
#include <D3d8.h>

extern ApolloClass	apollo;

namespace cJetris 
{
	// Simple material structure
	typedef struct {
		unsigned int		matid;	// simple index to list of material vector
		D3DMATERIAL8		dxmat;

		// Texture stuff
		Data::StringClass	texfilename;
		D3DTEXTUREADDRESS	dxaddrmode;		// wrap, mirror etc
		IDirect3DTexture8	*dxtex;

	} MaterialType;

	typedef struct {
		Data::StringClass		name;
		unsigned int			index;
	} MaterialIndexType;

	enum RunCodes { NOTHING, INIT, IDLE, STARTUP, LOADING, MENU, GAME, PAUSE, END, SHUTDOWN };


	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();

	void RunMain(Packet::PackClass *p);
	void ShutDown(Packet::PackClass *p);
	void SetMesh(Packet::PackClass *p);
	void LeftMouse(Packet::PackClass *p);
	void NewGame(Packet::PackClass *p);
	void RotatePiece(Packet::PackClass *p);
	void MoveLeft(Packet::PackClass *p);
	void MoveRight(Packet::PackClass *p);
	void DownFast(Packet::PackClass *p);
	void Pause(Packet::PackClass *p);

	void RenderDone(Packet::PackClass *p);
	void InputDone(Packet::PackClass *p);
}

#endif
