#ifndef _ZEUS_PROFILE_H_
#define _ZEUS_PROFILE_H_

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

// Profiling
// I hope people like this, there are probably alot more things to add. Its very early days yet.
// Simply declare a profile class in the scope you want profiled.
// ie:  Profile("Gamesection")

// There is a small overhead (as with all profiling) and profiling is sorted on the given name.
// Beware of naming profile sections the same name!! Itll stuff up your results!!

#include <windows.h>

// Std template gear
#include <map>
#include <vector>

using namespace std;

// Profiling Class
// Profiling, Debugging, Timing..

#define Profile(a)		Profile::ProfileClass	inst(a);
#define ProfileEnd()	inst.~ProfileClass();


// Macro for dumping to the debugging log file
#define ProfileDumpSample(l)		{											\
									Profile::Sample *s = Profile::GetSample(l);	\
									const char *sc = Profile::DebugSample(l,s);	\
									Packet("This","Zeus.Log");					\
									Padd(sc);									\
									Psend;										\
									}															

#define ProfileDumpAll()			{											\
									const char *h = Profile::DebugHeader();		\
									Packet("This","Zeus.Log");					\
									Padd(h);									\
									Psend;										\
									map<const char *,Profile::Sample *>::iterator pi = Profile::ProfileMap.begin();	\
										for(int i = 0; i<Profile::ProfileMap.size(); i++)	\
										{													\
										if(pi != Profile::ProfileMap.end())					\
										{													\
											ProfileDumpSample(pi->first);					\
										}													\
										pi++;												\
										}													\
									}															


namespace Profile
{
	typedef struct {
		unsigned int	active;
		unsigned int	count;	    // Counter the number of times this sample has been hit
		LARGE_INTEGER	acc;		// Accumulated counts

		LARGE_INTEGER	max;		// Maximum time in sec for one count
		LARGE_INTEGER	min;		// Minimum time in sec for one count
		LARGE_INTEGER	last;		// Last time profile for one count

		LARGE_INTEGER	start;		// The start time of this sample
	} Sample;

	typedef vector<Sample *> SampleVec;

	extern map<const char *,Sample *> ProfileMap;		// List of sample<->name mappings
	extern LARGE_INTEGER	countsPerSec;

	Sample *GetSample(const char *label);
	
	const char *DebugHeader();
	const char *DebugSample(const char *l, Sample *s);

	class ProfInitClass
	{
	public:
		ProfInitClass();
		~ProfInitClass();
	};

	class ProfileClass
	{
	public:
		Sample	*sample;

		ProfileClass(const char *name = "NoName");
		~ProfileClass();
	};

}

#endif