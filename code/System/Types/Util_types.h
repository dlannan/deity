#ifndef _UTIL_TYPES_H_
#define _UTIL_TYPES_H_

#include <vector>

// Memory utility
// For tracking leaks, memusage and general memory problems
namespace Memory
{
	// Use this for pointers across modules, keeps indexes to used references
	class Reaper
	{
	private:
		vector<void *>	refs;
		void *addr;
		
	public:
		// You must create a reaper with an address for referencing
		Reaper(void *addr)
		{
		}

		~Reaper()
		{
		}
	};
}

#endif
