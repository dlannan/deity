
#include "..\\Types\\String_types.h"
#include "logging.h"

namespace Debugging
{
	// This is a temp String class for passing to Apolonius packets
	static	Data::PageType	log;

	static unsigned int		timestamp = 0;
	static unsigned long	accumulator = 0;	// Accumulated millisecond counter - we stop when the debugger stops
	static LARGE_INTEGER	countsPerSec;
	static LARGE_INTEGER	startTime;

	static Data::StringClass	logname;		// Logname and path

	class Log
	{
	public:
		Log()
		{
			Data::StringClass	*sptr = new Data::StringClass;
			log.push_back(*sptr);
		}

		~Log()
		{
			log.clear();
		}
	};

	static Log				moduleLog;

	void WriteLog()
	{
	}

	unsigned int SetTimeStamp(unsigned int ts)
	{
		unsigned int old = timestamp;
		timestamp = ts;
		return old;
	}

	void TimeStamp()
	{
		if(!timestamp) return;
		// We are adding text to a new line - timestamp?
		Data::StringClass *sptr = &log.back();
		if(sptr->Empty())
		{
			LARGE_INTEGER	pc;
			char			sbuf[20];

			// Calc Time since start
			QueryPerformanceCounter(&pc);

			// WARNING: using floats here can induce loss of data, some finite timings may be inaccurate
			float secs = ((float)( pc.QuadPart - startTime.QuadPart ) / countsPerSec.QuadPart);
			unsigned long millisec = (unsigned long)(1000000.0f * secs);
			accumulator += millisec;

			sprintf(sbuf,"%06d.%06d",accumulator/1000000,accumulator%1000000);

			*sptr << "[";
			*sptr << sbuf;
			*sptr << "]|";
		}
	}

	void Out(char *txt)
	{
		if(!txt) return;
		char *npos;
		
		TimeStamp();
		Data::StringClass *sptr = &log.back();

		if(npos = strchr(txt,'\n'))
		{
			if(txt[0]=='\n')
			{
				sptr = new Data::StringClass;
				log.insert(log.end(),*sptr);

				return;
			}
			else
			{
				for(int i=0;i<(npos-txt);i++)
					*sptr << txt[i];

				sptr = new Data::StringClass;
				log.insert(log.end(),*sptr);
			
//					Debug::Out( &txt[i+1] );
			}
		}
		else
		{
			*sptr << txt;
		}
	}

	void Out(const char *txt)
	{
		Out((char *)txt);
	}

	void Out(Data::StringClass *str)
	{
		Out(str->String());
	}

	void Out(Data::StringClass str)
	{
		Out(str.String());
	}

	void Out(char txt)
	{
		TimeStamp();
		Data::StringClass *sptr = &log.back();
		*sptr << txt;
	}

	void Out(int txt)
	{
		TimeStamp();
		Data::StringClass *sptr = &log.back();
		*sptr << txt;
	}

	void Out(long txt)
	{
		TimeStamp();
		Data::StringClass *sptr = &log.back();
		*sptr << txt;
	}

	void Out(float txt)
	{
		TimeStamp();
		Data::StringClass *sptr = &log.back();
		*sptr << txt;
	}

	void Out(double txt)
	{
		TimeStamp();
		Data::StringClass *sptr = &log.back();
		*sptr << txt;
	}

	void Out(unsigned char txt)
	{
		TimeStamp();
		Data::StringClass *sptr = &log.back();
		*sptr << txt;
	}

	void Out(unsigned int txt)
	{
		TimeStamp();
		Data::StringClass *sptr = &log.back();
		*sptr << txt;
	}

	void Out(unsigned long txt)
	{
		TimeStamp();
		Data::StringClass *sptr = &log.back();
		*sptr << txt;
	}
}						   

