// ZEUS Profiling cpp

#include "profile.h"
#include "..\\System.h"

// Info:
// Every profile instance will generate a profile map for each seperate module
// The profilemap is then accessible by Zeus for display and saving.

namespace Profile
{
	map<const char *,Sample *> ProfileMap;		// List of sample<->name mappings
	LARGE_INTEGER	countsPerSec;

	// This is the once only profile object per module
	static ProfInitClass		prof;

	// Global Function
	Sample *GetSample(const char *label)
	{
		map<const char *,Profile::Sample *>::iterator pi = Profile::ProfileMap.find(label);
		if(pi != Profile::ProfileMap.end())
		{
			return pi->second;
		}
		else
		{
			return NULL;
		}
	}

	const char *DebugHeader()
	{
		static Data::StringClass	data;
		char						buff[512];
		data.Clear();	// Make sure any previous use is gone

		sprintf(buff,"%-30s%12s%12s%12s%12s%12s%12s\n","Profile Name",
				"Count","Start","Acc.(s)","Max.(s)","Min.(s)","Last (s)");
			data = buff;

		return data.String();
	}


	const char *DebugSample(const char *l, Sample *s)
	{
		static Data::StringClass	data;
		char						buff[512];

		data.Clear();	// Make sure any previous use is gone

		if(s != NULL)
		{
			sprintf(buff,"%-30s %11d %011u %011d %011d %011d %011d\n",
					l,
					s->count,
					s->start.LowPart,
					s->acc.LowPart,
					s->max.LowPart,
					s->min.LowPart,
					s->last.LowPart );
			data = buff;
		}

		return data.String();
	}

	ProfInitClass::ProfInitClass()
	{
		// Retrieve the PC resolution in counts per second
		QueryPerformanceFrequency(&countsPerSec);
	}

	ProfInitClass::~ProfInitClass()
	{
		// Empty the allocated Sample structures
		Profile::ProfileMap.clear();
	}

	ProfileClass::ProfileClass(const char *name)
	{
		map<const char *, Sample *>::iterator si = ProfileMap.find(name);
		if(si == ProfileMap.end())
		{
			sample = new Sample;
			sample->count = 0;
			sample->acc.QuadPart = 0;
			sample->max.QuadPart = 0;
			sample->min.QuadPart = 0;
			sample->last.QuadPart = 0;
		}
		else
		{
			sample = (*si).second;
		}

		sample->active = 1;
		QueryPerformanceCounter(&sample->start);
		sample->count++;

		ProfileMap[name] = sample;
	}

	ProfileClass::~ProfileClass()
	{
		if(sample->active == 0) return;

		sample->active = 0;

		LARGE_INTEGER	pc;
		QueryPerformanceCounter(&pc);

		LARGE_INTEGER last;
		last.QuadPart = pc.QuadPart - sample->start.QuadPart;
		if(last.QuadPart > sample->max.QuadPart) sample->max.QuadPart = last.QuadPart;
		if((sample->min.QuadPart == 0) && (last.QuadPart > 0)) sample->min.QuadPart = last.QuadPart;
		if((sample->min.QuadPart > last.QuadPart) && (last.QuadPart > 0)) sample->min.QuadPart = last.QuadPart;
		sample->acc.QuadPart = sample->acc.QuadPart + last.QuadPart;
		sample->last.QuadPart = last.QuadPart;
	}

}