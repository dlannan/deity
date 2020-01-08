// Function specification

#ifndef _FUNCTION_TYPES_H_
#define _FUNCTION_TYPES_H_

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include <map>

#include "string_types.h"
#include "Packet_types.h"

using namespace std;

// Static function binding
typedef struct
{
	CRC::StringClass	name;
	void (*proc) (Packet::PackClass *p);
} Function;

struct Fcmp
{
  bool operator()(const unsigned int s1, const unsigned int s2) const
  {
    return (s1 < s2);
  }
};

typedef map<unsigned int, Function *, Fcmp> HFuncTable;


// Static variable binding
template <typename _T>
class Variable 
{
public:
	CRC::StringClass	name;
	void *				addr;
	
	Variable()
	{
		addr = new _T;
	}

	~Variable()
	{
		delete _T;
	}
};

struct Vcmp
{
  bool operator()(const unsigned int s1, const unsigned int s2) const
  {
    return (s1 < s2);
  }
};

typedef map<unsigned int, void *, Vcmp> HVarTable;

#endif