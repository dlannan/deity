#ifndef _APPOLONIUS_H_
#define _APPOLONIUS_H_

#include "..\\System\\System.h"

// APPOLONIUS
// The mighty debugger - wisest of all mortals :-)
// Will also have memory management abilities later - heap manager to be used by all modules.

namespace APPOLONIUS
{
	void Init(Packet::PackClass	*in);
	void Kill(Packet::PackClass	*in);

	void Disable(Packet::PackClass	*in);
	void Log(Packet::PackClass	*in);
	void SetLogFile(Packet::PackClass	*in);
	void Break(Packet::PackClass	*in);
	void Start(Packet::PackClass	*in);
	void Stop(Packet::PackClass	*in);
	void Cycle(Packet::PackClass	*in);
	void Report(Packet::PackClass	*in);

	// Function declarations
	void WriteLog(Packet::PackClass	*in);
	void Out(char *txt);
	void Out(const char *txt);
	void Out(Data::StringClass *str);
	void Out(Data::StringClass str);
	void Out(char txt);
	void Out(int txt);
	void Out(long txt);
	void Out(float txt);
	void Out(double txt);
	void Out(unsigned char txt);
	void Out(unsigned int txt);
	void Out(unsigned long txt);
}

#endif