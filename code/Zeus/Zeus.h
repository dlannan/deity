// ZEUS Classes

#ifndef _ZEUS_H_
#define _ZEUS_H_

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

// Module definitions
#include "..\\System\\System.h"
#include "ZEUS_debug.h"

namespace ZEUS
{
//-----------------------------------------------------------------------------------------------
//
// These can be externally accessed

	// This is the interface for all objects in zeus.. to the outside world..
//	extern Comm::BaseClass	comm;

	// This is the function ZEUS 'gives' to all modules and controllers
//	void recv(Packet::PackClass *packet);

	void Prep(char **argv, int argc);
	void Process(char **argv, int argc);
	void Kill();

	extern HINSTANCE			instance;
//	extern Packet::PacketVec	inputbuffer;

}

#endif