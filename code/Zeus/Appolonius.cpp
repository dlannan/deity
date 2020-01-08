// APPOLONIUS
// The mighty debugger - wisest of all mortals :-)
// Will also have memory management abilities later - heap manager to be used by all modules.

#include "..\\System\\System.h"
#include "Appolonius.h"
#include "Zeus_debug.h"
#include "lua.h"

namespace ZEUS
{
	extern Timer::Clock	masterClock;
}

namespace APPOLONIUS
{
	// Currently loaded apollo class modules
	ApolloMap			*apolloList;

	// Supplimentary Apollo list - controllers housing modules can 
	// use Apollonius to help debug them by submitting their list
	ApolloMap			*supList;

	Data::PageType		log;
	File::FileClass		out;

	Data::StringClass	logname;		// Logname and path

	// TimeStamp info
	unsigned int	timestamp = 0;
	unsigned long	accumulator = 0;	// Accumulated millisecond counter - we stop when the debugger stops

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
			char			sbuf[20];

			// TODO - clean that messy shit up.
			ZEUS::masterClock.Update();
			sprintf(sbuf,"%06d.%06d",(int)ZEUS::masterClock.Sec(),
									(int)ZEUS::masterClock.Us() - ((int)ZEUS::masterClock.Sec() * 1000000) );

			*sptr << "[";
			*sptr << sbuf;
			*sptr << "] ";
		}
	}


	void Init(Packet::PackClass	*in)
	{
		ASSERT(API::RELEASE, "RELEASE VERSION WITH DEBUGGING ON!");

		// Sent from Zeus as the default apollolist
		apolloList = (ApolloMap *)in->GetData(D_ULONG);
		Data::StringClass	*sptr = new Data::StringClass;
	
		// By default timestamp is on
		timestamp = TRUE;
		log.push_back(*sptr);
	}

	void SetLogFile(Packet::PackClass	*in)
	{
		out.Open(in->GetData(D_STRING),FILE_WRITE);
	}

	void Kill(Packet::PackClass	*in)
	{
		if(out.handle)
		{
			WriteLog(in);
			out.Close();
		}
	}

	void Disable(Packet::PackClass	*in)
	{
	}

	void Log(Packet::PackClass	*in)
	{
		const char *Line = in->GetData(D_STRING);
		Out(Line);	// Only support one line at a time!
		ZEUS::Debug::Out(Line);
	}

	void Break(Packet::PackClass	*in)
	{
	}

	void Start(Packet::PackClass	*in)
	{
	}

	void Stop(Packet::PackClass	*in)
	{
	}

	void Cycle(Packet::PackClass	*in)
	{
	}

	void Report(Packet::PackClass	*in)
	{
	}

	void WriteLog(Packet::PackClass	*in)
	{
		// Empty log? Not likely!
		if(!log.empty())
		{
			out << &log;
		}
	}

	void SupApolloMap(Packet::PackClass	*in)
	{
		// Should do some pointer checks here
		supList = (ApolloMap *)in->GetData(D_ULONG);
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
				*sptr << "\n";
				sptr = new Data::StringClass;
				log.insert(log.end(),*sptr);

				return;
			}
			else
			{
				for(int i=0;i<(npos-txt);i++)
					*sptr << txt[i];

				*sptr << "\n";
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