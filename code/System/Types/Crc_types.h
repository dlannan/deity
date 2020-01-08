// CRC types

#ifndef _CRC_TYPES_H_
#define _CRC_TYPES_H_

#include<windows.h>
#include "String_types.h"

#include <vector>
#include <string>

//CRC classes.. could make this a template methinks

using namespace std;

namespace CRC
{
	// -------------------------------------------------------------------------------------------------
	// CRC defines
	// Simple error flags definitions
	#define ERROR_OK		0
	#define ERROR_FAIL	   -1

	#define BITMASK(X)		(1L << (X))
	#define MASK32			0xFFFFFFFFL

	// Template CRC class - Most crcs should be built at compile time
	template<class _T> unsigned int CRCMaker(_T _data)
	{
		unsigned int val = 0;
		int   i = 0;
		unsigned long t = 0;
		unsigned long uch = 0;

		const char *cptr = (const char *)_data;
		const unsigned long topbit = BITMASK(32-1);
		
		// Calc the new crc
		for(int c=0; c<255; c++)
		{
			if(!cptr[c]) break;
			uch  = ((cptr[c] >= 'A') && (cptr[c] <= 'Z'))?cptr[c]-('A'-'a'):cptr[c];

			t = uch;
//			for (i=0; i<8; i++)
			{
				uch = (t & 1L)?  uch |=  BITMASK((8-1)-0): uch &= ~BITMASK((8-1)-0); t>>=1;

				uch = (t & 1L)?  uch |=  BITMASK((8-1)-1): uch &= ~BITMASK((8-1)-1); t>>=1;

				uch = (t & 1L)?  uch |=  BITMASK((8-1)-2): uch &= ~BITMASK((8-1)-2); t>>=1;

				uch = (t & 1L)?  uch |=  BITMASK((8-1)-3): uch &= ~BITMASK((8-1)-3); t>>=1;

				uch = (t & 1L)?  uch |=  BITMASK((8-1)-4): uch &= ~BITMASK((8-1)-4); t>>=1;

				uch = (t & 1L)?  uch |=  BITMASK((8-1)-5): uch &= ~BITMASK((8-1)-5); t>>=1;

				uch = (t & 1L)?  uch |=  BITMASK((8-1)-6): uch &= ~BITMASK((8-1)-6); t>>=1;

				uch = (t & 1L)?  uch |=  BITMASK((8-1)-7): uch &= ~BITMASK((8-1)-7); t>>=1;
			}
				
			val ^= (uch << (32-8));
//			for (i=0; i<8; i++)
			{
				val = (val & topbit)? (val << 1) ^ 0x04C11DB7L:val <<= 1;
				val &= ( (const unsigned long) (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1 );
				val = (val & topbit)? (val << 1) ^ 0x04C11DB7L:val <<= 1;
				val &= ( (const unsigned long) (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1 );
				val = (val & topbit)? (val << 1) ^ 0x04C11DB7L:val <<= 1;
				val &= ( (const unsigned long) (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1 );
				val = (val & topbit)? (val << 1) ^ 0x04C11DB7L:val <<= 1;
				val &= ( (const unsigned long) (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1 );
				val = (val & topbit)? (val << 1) ^ 0x04C11DB7L:val <<= 1;
				val &= ( (const unsigned long) (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1 );
				val = (val & topbit)? (val << 1) ^ 0x04C11DB7L:val <<= 1;
				val &= ( (const unsigned long) (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1 );
				val = (val & topbit)? (val << 1) ^ 0x04C11DB7L:val <<= 1;
				val &= ( (const unsigned long) (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1 );
				val = (val & topbit)? (val << 1) ^ 0x04C11DB7L:val <<= 1;
				val &= ( (const unsigned long) (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1 );
			}
		}

		t = val;
		for (i=0; i<32; i++)
		{
			val = (t & 1L)? val |=  BITMASK((32-1)-i): val &= ~BITMASK((32-1)-i); t>>=1;
		}

		val = 0xFFFFFFFF ^ val;
		return val;
	}

	inline unsigned int Str(const char *val) { return CRCMaker<const char *>(val); }
	inline unsigned int Int(unsigned int val) { return CRCMaker<unsigned int *>(&val); }

	// -------------------------------------------------------------------------------------------------
	// CRC Base class
	//
	class BaseCRCClass {
	public:
		unsigned int	val;	// crc register val

		BaseCRCClass(void)
		{
			val = 0x0L;
		}

		~BaseCRCClass(void)
		{
		}


		unsigned long Reflect(unsigned long v,int b)
		/* Returns the value v with the bottom b [0,32] bits reflected. */
		/* Example: reflect(0x3e23L,3) == 0x3e26                        */
		{
			int   i;
			unsigned long t = v;
			for (i=0; i<b; i++)
			{
			if (t & 1L)
			   v|=  BITMASK((b-1)-i);
			else
			   v&= ~BITMASK((b-1)-i);

			t>>=1;

			}
			return v;
		}

		unsigned long Widmask()
		/* Returns a longword whose value is (2^p_cm->cm_width)-1.     */
		/* The trick is to do this portably (e.g. without doing <<32). */
		{
			return (( ((unsigned long)1<<31) - (unsigned long)1) << 1)|(unsigned long)1;
		}

		void Nxt (int ch)
		{
			int   i;
			unsigned long uch  = (unsigned long) ch;
			unsigned long topbit = BITMASK(32-1);

			uch = Reflect(uch,8);
			val ^= (uch << (32-8));
			for (i=0; i<8; i++)
			{
			if (val & topbit)
			  val = (val << 1) ^ 0x04C11DB7L;
			else
			   val <<= 1;
			val &= Widmask();
			}
		}

		unsigned long Crc()
		{
			return 0xFFFFFFFF ^ Reflect(val,32);
		}

		unsigned long Tab(int index)
		{
			int   i;
			unsigned long r;
			unsigned long topbit = BITMASK(32-1);
			unsigned long inbyte = (unsigned long) index;

			inbyte = Reflect(inbyte,8);
			r = inbyte << (32-8);
			for (i=0; i<8; i++)
			if (r & topbit)
				r = (r << 1) ^ 0x04C11DB7L;
			else
				r<<=1;
			r = Reflect(r,32);
			return r & Widmask();
		}

	};


	// -------------------------------------------------------------------------------------------------
	// CRC Time Class - mainly for timestamping and comparison checking
	// A timestamp crc can provide a unique id for anything that is created.
	class TimeClass {
	private:
		unsigned int	id;
		unsigned long	timeval;

	public:

		// CRC time class
		TimeClass()
		{
			CalcCrc();
		}

		~TimeClass()
		{
		}

		void CalcCrc()
		{
			LARGE_INTEGER	pc;
			QueryPerformanceCounter(&pc);

			timeval = pc.LowPart ;
			id = CRC::Int(timeval);
		}

		unsigned int Time() { return timeval; }
		unsigned int Id() { return id; }
	};

	// -------------------------------------------------------------------------------------------------
	// CRC String Class - CRC strings for compares etc. Uses std::string stl containers.
	// Used for crcing strings.. must be null terminated
	// Is case insensitive.. always converted to lowercase..

	// TODO: Need to turn into template - this should nearly all be done at compile time!

	class StringClass {
	private:
		unsigned int		id;

		Data::StringClass	orig;
	public:

		// CRC string class
		StringClass()
		{
			Clear();
		}

		StringClass(char *s)
		{
			Clear();
			SetString(s);
			id = Str(s);
		}

		~StringClass()
		{
		}

		void Clear()
		{
			orig.Clear();
			id = 0;
		}

		unsigned int Id() 
		{ 
			return id; 
		}

		int Size()
		{
			return orig.Size();
		}

		int Empty()
		{
			return orig.Empty();
		}

		Data::StringClass Orig() 
		{ 
			if(orig.Empty())
			{
				static Data::StringClass nullstring;
				return nullstring;
			}
			else
				return orig; 
		}

		char *String() 
		{ 
			if(orig.Empty())
				return NULL;
			else
				return (char *)orig.String(); 
		}

		inline unsigned int CalcStringCRC()
		{
			id = Str(orig.String());
			return id;
		}

		void AddString(char *s)
		{
			if(!s) return;
			orig << s;
			orig.Lower();

			id = CalcStringCRC();
		}

		void AddString(const char *s)
		{
			AddString((char *)s);
		}

		// When setting strings we make copies.. do not interefere with original data
		void SetString(char *s)
		{
			if(!s) return;
			
			orig.Clear();
			orig.SetData(s);
			orig.Lower();

			id = CalcStringCRC();
		}

		// When setting strings we make copies.. do not interefere with original data
		void SetString(const char *s)
		{
			SetString((char *)s);
		}

		void operator << (char *s) { AddString(s); }
		void operator << (const char *s) { AddString((char *)s); }
		void operator << (Data::StringClass *s) { AddString((char *)s->String()); }
		void operator << (Data::StringClass s) { AddString((char *)s.String()); }
		void operator = (Data::StringClass s) { SetString((char *)s.String()); }
		void operator = (const char *s) { SetString((char *)s); }
		void operator = (char *s) { SetString(s); }

		void SetId(unsigned int _id)
		{
			orig.Clear();
			id = _id;
		}

		void operator = (CRC::StringClass s) { SetString((char *)s.String()); }
		void operator = (unsigned int _id) 	{ SetId(_id); }
	};


	// TODO: Need to turn into template - this should nearly all be done at compile time!

	class ModIdClass 
	{
	private:
		CRC::StringClass	full;	// Module id
		CRC::StringClass	mod;	// Module id
		CRC::StringClass	func;	// Function id

	public:

		// When we add to a ModId we pull apart the incoming text and seperate into module and function
		ModIdClass(char *s)
		{
			full.SetString(s);
			Update();
		}
		
		ModIdClass(const char *s)
		{
			full.SetString(s);
			Update();
		}

		ModIdClass(Data::StringClass s)
		{
			full.SetString(s.String());
			Update();
		}

		ModIdClass()
		{
			Clear();
		}

		~ModIdClass()
		{
			Clear();
		}

		void Clear()
		{
			full.Clear();
			mod.Clear();
			func.Clear();
		}

		// Pull Apart
		void Update()
		{
			// Dont update if we have no text!
			if(full.Empty()) return;

			char *token;
			char seps[] = ".";
			char temp[255];

			strcpy(temp,full.String());

			token = strtok(temp,seps);
			if(token)
			{
				mod = token;

				// Only bother looking for two
				token = strtok(NULL,seps);
				if(token)
				{
					func = token;
				}
			}
		}

		void AddString(const char *s)
		{
			full << s;
			Update();
		}

		void AddString(char *s)
		{
			full << s;
			Update();
		}

		void SetString(char *s)
		{
			mod.Clear();
			func.Clear();

			full.Clear();
			full = s;
			Update();
		}

		void operator << (char *s) { AddString(s); }
		void operator << (const char *s) { AddString(s); }
		void operator << (Data::StringClass *s) { AddString((char *)s->String()); }
		void operator << (Data::StringClass s) { AddString((char *)s.String()); }
		void operator = (CRC::StringClass s) { SetString((char *)s.String()); }
		void operator = (const char *s) { SetString((char *)s); }
		void operator = (char *s) { SetString(s); }

		void operator = (ModIdClass m) 
		{
			if(m.full.Empty())
			{
				SetId(m.Id());
				SetModId(m.GetModId());
				SetFuncId(m.GetFuncId());
			}
			else
			{
				operator = (m.full);
			}
		}

		unsigned int Id()
		{
			return full.Id();
		}

		// Calls for retrieving info from the dest and src crc ids
		unsigned int GetModId()
		{
			return mod.Id();
		}

		unsigned int GetFuncId()
		{
			return func.Id();
		}

		// Calls for retrieving info from the dest and src crc ids
		CRC::StringClass GetMod()
		{
			return mod;
		}

		CRC::StringClass GetFunc()
		{
			return func;
		}

		const char *String()
		{
			return full.String();
		}

		void SetId(unsigned int id)
		{
			full.SetId(id);
		}

		void SetModId(unsigned int id)
		{
			mod.SetId(id);
		}

		void SetFuncId(unsigned int id)
		{
			func.SetId(id);
		}
	};

}

#endif