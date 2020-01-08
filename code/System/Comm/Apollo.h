// APOLLO module structure layout

// Apollo template based on a simple module format using base classes from ZEUS

#ifndef _APOLLO_H_
#define _APOLLO_H_

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

// Std template gear
#include <vector>

#include "Dll_func.h"

using namespace std;

#include "..\\Types\\String_types.h"
#include "..\\Types\\Crc_types.h"
#include "Comm_types.h"

class ApolloClass : public Comm::BaseClass {
public:
	CRC::TimeClass		uid;	// Unique time stamp id - this is what zeus looks for!
	CRC::ModIdClass		id;		// Apollo identifier - this is the id that is matched to uid.
	Data::StringClass	Sdll;	// DLL name
	DLL	*				dll;

	bool				state;	// The enabled/disabled state of a module

//	Data::PageType		info;	// Fill in the function info and any documentation necessary to
								// describe function usage
	ApolloClass()
	{
	}

	~ApolloClass()
	{
	}
};

typedef vector<ApolloClass*> ApolloVec;

typedef map<unsigned int, ApolloClass *, less<unsigned int>, allocator<ApolloClass *> > ApolloMap;

#endif