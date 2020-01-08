#ifndef _TIMER_TYPES_H_
#define _TIMER_TYPES_H_

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

// TimerType is a simple class to provide base counting and timing facilities to 
// the entire engine.

// A timer works from a master clock, which there should be only one of at any one time.
// Our master clock runs in zeus, but this code could be used in any app.

namespace Timer
{
	// An alarm flags when it has been triggered - alarms need constant polling for
	// most accurate timing

	// All values are must be in the same resolution - mixing microseconds with milliseconds
	// will result in obvious problems. :-)
	class Alarm
	{
	private:
		unsigned long	interval;	// interval which the timer should alarm at
		unsigned long	previous;	// previous clock set
		unsigned char	active;

	public:
		CRC::TimeClass	id;

		// Constructor always takes an interval. An interval of 0, is a constant Alarm.
		Alarm(unsigned long i = 0) 
		{ 
			interval = i; previous = 0;
			active = (i>0);
		}
		~Alarm();

		inline void Set(unsigned long i = 0) { interval = i; }
		inline unsigned long Get() { return interval; }

		// Must be passed a clock - can be any resolution 
		inline bool Poll(unsigned long time) 
		{ 
			if(!active) return false;

			long diff = time - previous;
			if( diff >= interval )
			{
				// Problems can occur if the interval is too small, and the timer cant keep up
				// We drop timing intervals if they fall too far behind - over 2 intervals out
				if( diff > (interval + interval) ) 
				{
					previous = time;
				}
				else
				{
					previous += interval;
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		inline void Stop() { active = 0; }
		inline void Start() { active = 1; }
		inline unsigned char Active() { return active; }
	};

	class Counter
	{
	private:
		unsigned long	start;
		unsigned long	end;
		unsigned long	step;
		unsigned long	curr;
		unsigned char	active;

	public:
		CRC::TimeClass	id;

		Counter()
		{
			step = 0; start = 0; end = 0;
			active = 0; curr = start;
		}

		Counter(unsigned long s, unsigned long e, unsigned long st) 
		{
			step = st; start = s; end = e;
			active = 1; curr = start;
		}

		~Counter();

		inline void Set(unsigned long s, unsigned long e, unsigned long st)
		{
			step = st; start = s; end = e;
			curr = start;
		}

		inline unsigned long Get() { return curr; }

		inline bool Poll()
		{
			if(!active) return false;
			// forward counting
			if(curr + step >= end)
			{
				curr = start;
				return true;
			}
			// reverse counting
			else if(curr + step <= start)
			{
				curr = end;
				return true;
			}
			else
			{
				curr += step;
				return false;
			}
		}

		inline void Stop() { active = 0; }
		inline void Start() { active = 1; }
		inline void Reverse() { step = -step; }

		inline unsigned char Active() { return active; }
	};

	class Clock
	{
	private:
		LARGE_INTEGER	time;
		LARGE_INTEGER	start;
		float			countsPerSec;
		float			countsPerMSec;
		float			countsPerUSec;
		
	public:
		// Init clock using perfcounter
		// If there is no perfcounter - you are stuffed. :-)
		Clock()
		{
			LARGE_INTEGER	cps;
			QueryPerformanceFrequency(&cps);
			countsPerSec = 1.0f / (float)cps.QuadPart;
			countsPerMSec = 1000.0f / (float)cps.QuadPart;
			countsPerUSec = 1000000.0f / (float)cps.QuadPart;

			QueryPerformanceCounter(&time);
			start.QuadPart = time.QuadPart;
		}

		~Clock() { }

		// Update the time
		inline void Update()
		{
			QueryPerformanceCounter(&time);
		}

		inline unsigned long Us()
		{
			return (unsigned long)((time.QuadPart - start.QuadPart) * countsPerUSec);
		}

		inline unsigned long Ms()
		{
			return (unsigned long)((time.QuadPart - start.QuadPart) * countsPerMSec);
		}

		inline unsigned long Sec()
		{
			return (unsigned long)((time.QuadPart - start.QuadPart) * countsPerSec);
		}

		// Actual Microseconds value in perfcounter  - slightly quicker (less a sub)
		inline unsigned long _Us()
		{
			return (unsigned long)(time.QuadPart * countsPerUSec);
		}

		// Actual Milliseconds value in perfcounter  - slightly quicker (less a sub)
		inline unsigned long _Ms()
		{
			return (unsigned long)(time.QuadPart * countsPerMSec);
		}

		// Actual seconds value in perfcounter - slightly quicker (less a sub)
		inline unsigned long _Sec()
		{
			return (unsigned long)(time.QuadPart * countsPerSec);
		}

		inline unsigned long	Time() { return (unsigned long)(time.QuadPart - start.QuadPart); }

		inline unsigned long	_Time() { return (unsigned long)time.QuadPart; }
	};

}

#endif