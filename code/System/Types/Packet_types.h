
#ifndef _PACKET_TYPES_H_
#define _PACKET_TYPES_H_

// Std template gear
#include <vector>

#include "CRC_types.h"
#include "String_types.h"

using namespace std;

// Nice macro for making packets - pretty easy. 
// Make sure you dont declare more than one per scope.
#define Packet(_src, _dst)		static Packet::PackClass packet;	\
								packet.Build(_src, _dst);	

#define Padd					packet +=
#define Psend					apollo.Send(&packet);

namespace Packet
{
	#define MAX_PACKET_SIZE		0x80				// 128 bytes
	#define MAX_VAR_SIZE		MAX_PACKET_SIZE
	#define MAX_PACKET_PARAMS	16

	#define	D_VOID				(void *)NULL
	#define D_STRING			(const char *)NULL
	#define	D_UCHAR				(unsigned char)0x02
	#define	D_CHAR				(char)0x03
	#define	D_UINT				(unsigned int)0x04
	#define	D_INT				(int)0x05
	#define	D_ULONG				(unsigned long)0x06
	#define D_LONG				(long)0x07
	#define	D_FLOAT				(float)0x08

	const unsigned int	P_VOID   = 0x00;
	const unsigned int	P_STRING = 0x01;
	const unsigned int	P_UCHAR  = 0x02;
	const unsigned int	P_CHAR   = 0x03;
	const unsigned int	P_UINT   = 0x04;
	const unsigned int	P_INT    = 0x05;
	const unsigned int	P_ULONG  = 0x06;
	const unsigned int	P_LONG   = 0x07;
	const unsigned int	P_FLOAT  = 0x08;

	typedef struct 
	{
		//current number of params in our packet
		unsigned int numParams;

		//bit fields hold parameter types. 4 bits per param, max params 16 (MAX_PACKET_PARAMS)
		//param bits are added (to a param slot p1-p8 or p9-p16) when PAdd() is called.
		//final bitfield is crosschecked with submitted functions' bitfield providing run time 
		//type checking on all packets. Assert will occur if packet&function bitfields do not match
		unsigned int parameter[2];

		//array of parameter start addresses. used for ASM jumps when GetData(x) is called on a packet
		unsigned int startAddress[MAX_PACKET_PARAMS];

	} ParamInfo;

	// PackClass 
	class PackClass 
	{
	private:
		unsigned char	data[MAX_PACKET_SIZE];
		unsigned int	dataSize;
		unsigned int	dataIndex;		// Use this for fetching data
		ParamInfo		params;

		// Future use - mainly for debugging, profiling and tracking
		CRC::TimeClass		timestamp;

	public:
		CRC::ModIdClass	srccrc;
		CRC::ModIdClass	destcrc;

		// PackClass Constructor
		// Builds and empty packet
		PackClass()
		{
			Clear();
		}

		// Builds and empty packet
		~PackClass(void)
		{
		}

		void Build(CRC::ModIdClass src, CRC::ModIdClass dest)
		{
			Clear();
			srccrc = src;
			destcrc = dest;	
		}

		void Build(Data::StringClass src, Data::StringClass dest)
		{
			Clear();
			srccrc = src;
			destcrc = dest;	
		}

		void Build(char *src, char *dest)
		{
			Clear();
			srccrc << src;
			destcrc << dest;	
		}

		void Build(const char *src, const char *dest)
		{
			Clear();
			srccrc << src;
			destcrc << dest;	
		}

		// CRC setting
		void Build(unsigned int src, unsigned int dest)
		{
			Clear();
			srccrc.SetId(src);
			destcrc.SetId(dest);	
		}

		unsigned int	Size(void) { return dataSize; }

		unsigned int	Params(void) { return params.numParams; }

		// Debug, profiling and tracking.
		CRC::TimeClass	TimeStamp(void) { return timestamp; }

		// We only assign data - leave the src and dst alone
		void operator = (PackClass packet)   
		{ 
			packet = *this;
		}

		//adds the 4 bit type code to ParamInfo.Parameter[x]
		inline void AddTypeToField(unsigned int type)
		{
			if (params.numParams < 8)
			{
				params.parameter[0] |= (type >> (params.numParams * 4)) & 0xF;
			}
			else
			{
				params.parameter[1] |= (type >> (params.numParams * 4)) & 0xF;
			}
		}

		inline unsigned int GetDataType(unsigned int index)
		{
			if (params.numParams < 8)
			{
				return ((params.parameter[0] >> (index * 4)) & 0xF);
			}
			else
			{
				return ((params.parameter[1] >> (index * 4)) & 0xF);
			}
		}


		void DataClear()
		{
			memset(data,0,MAX_PACKET_SIZE * sizeof(unsigned int));
		}

		void ParamsClear()
		{
			memset(&params, 0, sizeof(ParamInfo));
		}

		void Clear()
		{
			dataSize = 0;
			dataIndex = 0;
			// Reset lifetime too
			timestamp.CalcCrc();

			srccrc.Clear();
			destcrc.Clear();
			DataClear();
			ParamsClear();
		}
/*
		void GetData(unsigned int index, void *var, unsigned int type)
		{
			ASSERT( GetDataType(index)!=type, "Packet::PackClass incorrect data fetch.");
			switch(type)
			{
			case P_CHAR:
				memcpy(var,&data[params.startAddress[index]], sizeof(char));
				break;
			case P_UCHAR:
				memcpy(var,&data[params.startAddress[index]], sizeof(unsigned char));
				break;
			case P_STRING:
				strcpy((char *)var,(const char *)&data[params.startAddress[index]]);	// NULL TERMINATED STRINGS PPL!
				break;
			case P_UINT:
				memcpy(var,&data[params.startAddress[index]], sizeof(unsigned int));
				break;
			case P_INT:
				memcpy(var,&data[params.startAddress[index]], sizeof(int));
				break;
			case P_ULONG:
				memcpy(var,&data[params.startAddress[index]], sizeof(unsigned long));
				break;
			case P_LONG:
				memcpy(var,&data[params.startAddress[index]], sizeof(long));
				break;
			case P_FLOAT:
				memcpy(var,&data[params.startAddress[index]], sizeof(float));
				break;
			}
		}
*/

		template<class T> T GetDataIndex(unsigned int index, T type)
		{
			return *(T *)&data[params.startAddress[index]];
		}

		template<class T> T GetData(T type)
		{
			if(dataIndex > params.numParams) dataIndex = 0;
			return GetDataIndex(dataIndex++,type);
		}

		// Override the string call
		const char *GetDataIndex(unsigned int index, const char * type)
		{
			return (const char *)&data[params.startAddress[index]];
		}

		// Override the string call
		const char *GetData(const char *type)
		{
			if(dataIndex > params.numParams) dataIndex = 0;
			return GetDataIndex(dataIndex++,type);
		}

		void GetDataStart()
		{
			dataIndex = 0;
		}
/*
		void GetData(void *var, unsigned int type)
		{
			GetData(dataIndex,var,type);
			dataIndex ++;
			if(dataIndex > params.numParams) dataIndex = 0;
		}
*/
		unsigned int AddData(void *_data,unsigned int size)
		{
			ASSERT(params.numParams > MAX_PACKET_PARAMS, "Packet::BaseClass - You are a fukkin wally. Do you know that 16 parameters is just plain fukin stupid. USE A FUKKIN STRUCT FOR GODS SAKE.");
			ASSERT(data==NULL, "Packet::BaseClass data is null idiot.")
			ASSERT(dataSize + size > MAX_PACKET_SIZE, "Packet::BaseClass Packet size exceeded")

			memcpy(&data[dataSize], _data, size);
			params.startAddress[params.numParams] = dataSize;
			params.numParams ++;
			dataSize += size;
			return ERROR_OK;
		}

		template<class T>	unsigned int operator += (T _data)
		{
			AddParam(_data);
			return ERROR_OK;
		}

		unsigned int AddParam(unsigned int _data)
		{
			AddTypeToField(P_UINT);
			AddData((void *)&_data, sizeof(unsigned int));
			return ERROR_OK;
		}

		unsigned int AddParam(int _data)
		{
			AddTypeToField(P_INT);
			AddData((void *)&_data, sizeof(int));
			return ERROR_OK;
		}

		unsigned int AddParam(float _data)
		{
			AddTypeToField(P_FLOAT);
			AddData((void *)&_data, sizeof(float));
			return ERROR_OK;
		}

		unsigned int AddParam(char _data)
		{
			AddTypeToField(P_CHAR);
			AddData((void *)&_data, sizeof(char));
			return ERROR_OK;
		}

		unsigned int AddParam(unsigned char _data)
		{
			AddTypeToField(P_UCHAR);
			AddData((void *)&_data, sizeof(unsigned char));
			return ERROR_OK;
		}

		unsigned int AddParam(long _data)
		{
			AddTypeToField(P_LONG);
			AddData((void *)&_data, sizeof(long));
			return ERROR_OK;
		}

		unsigned int AddParam(unsigned long _data)
		{
			AddTypeToField(P_ULONG);
			AddData((void *)&_data, sizeof(unsigned long));
			return ERROR_OK;
		}

		unsigned int AddParam(char *_data)
		{
			AddTypeToField(P_STRING);
			AddData((void *)_data, strlen(_data)+1);
			return ERROR_OK;
		}

		unsigned int AddParam(const char *_data)
		{
			AddTypeToField(P_STRING);
			AddData((void *)_data, strlen(_data)+1);
			return ERROR_OK;
		}

		unsigned int AddParam(Data::StringClass _data)
		{
			AddTypeToField(P_STRING);
			AddData((void *)_data.String(), _data.Size()+1);
			return ERROR_OK;
		}
	};


	// BaseClass manipulation routines
	// Data packing routines
/*	class PackClass : public BaseClass
	{
	public:
		PackClass() { 	Clear(); }

		void Build(CRC::ModIdClass src, CRC::ModIdClass dest)
		{
			Clear();
			srccrc = src;
			destcrc = dest;	
		}

		void Build(Data::StringClass src, Data::StringClass dest)
		{
			Clear();
			srccrc = src;
			destcrc = dest;	
		}

		void Build(char *src, char *dest)
		{
			Clear();
			srccrc << src;
			destcrc << dest;	
		}

		void Build(const char *src, const char *dest)
		{
			Clear();
			srccrc << src;
			destcrc << dest;	
		}

		// CRC setting
		void Build(unsigned int src, unsigned int dest)
		{
			Clear();
			srccrc.SetId(src);
			destcrc.SetId(dest);	
		}

		~PackClass() { 	 }

		// Setting data (obviously only sets one unsigned int value in the array)
		void Set(unsigned int &_data) 
		{ 
			Clear(); 
			SetPacketData(_data); 			
			AddTypeToField(P_UINT);
		}
		void Set(int &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data); 			
			AddTypeToField(P_INT);
		}
		void Set(float &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data);
			AddTypeToField(P_FLOAT);
		}
		void Set(char &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data); 
			AddTypeToField(P_CHAR);
		}
		void Set(unsigned char &_data)	
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data); 
			AddTypeToField(P_UCHAR);
		}

		operator = (unsigned int &_data)
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data); 
			AddTypeToField(P_UINT);
		}
		
		operator = (int &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data);
			AddTypeToField(P_INT);
		}
		
		operator = (float &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data);
			AddTypeToField(P_FLOAT);
		}

		operator = (char &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data); 
			AddTypeToField(P_CHAR);
		}

		operator = (unsigned char &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int &)_data); 
			AddTypeToField(P_UCHAR);
		}
		
		// Special setting of doubles and longs!
		void Set(long &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int )(_data & 0xFFFFFFFF));
			BaseClass::operator += ((unsigned int )((_data >> 32) & 0xFFFFFFFF));
			AddTypeToField(P_LONG);
		}

		void Set(unsigned long &_data) 
		{ 
			Clear(); 
			SetPacketData((unsigned int )(_data & 0xFFFFFFFF));
			BaseClass::operator += ((unsigned int )((_data >> 32) & 0xFFFFFFFF));
			AddTypeToField(P_ULONG);
		}

		operator = (long &_data)
		{
			Clear(); 
			SetPacketData((unsigned int )(_data & 0xFFFFFFFF));
			BaseClass::operator += ((unsigned int )((_data >> 32) & 0xFFFFFFFF));
			AddTypeToField(P_LONG);
		}

		operator = (unsigned long &_data)
		{
			Clear(); 
			SetPacketData((unsigned int )(_data & 0xFFFFFFFF));
			BaseClass::operator += ((unsigned int )((_data >> 32) & 0xFFFFFFFF));
			AddTypeToField(P_ULONG);
		}

		// Setting an array of data - only support basic types

		// Appending data
		void Append(int _data) 	
		{  BaseClass::operator += ((unsigned int )_data);	}
		void Append(unsigned int _data) { BaseClass::operator += (_data); }
		void Append(float _data) { BaseClass::operator += ((unsigned int )_data); }

		// This may look wasteful - but we may do a wide char (support unicode) later!
		void Append(char _data) { BaseClass::operator += ((unsigned int )_data);	}
		void Append(unsigned char _data) { BaseClass::operator += ((unsigned int )_data); }

		// Nice pointer data appending calls
		void Append(float *fptr, int size)
		{
			while(size-=4)	// we know a float is 4 bytes
			{
				BaseClass::operator += ((unsigned int )*fptr);
				fptr++;
			}
		}

		// Nice pointer data appending calls
		void Append(unsigned int *fptr, int size)
		{
			while(size-=4)	// we know a float is 4 bytes
			{
				BaseClass::operator += ((unsigned int )*fptr);
				fptr++;
			}
		}

		void Set(PackClass *p)
		{
			Clear();
			if(p->Size()>0)
			{
				for(int i = 0; i<(p->Size()/sizeof(unsigned int)); i++)
				{
					Append(p->GetDataUI(i));
				}
			}

			destcrc = p->destcrc;
			srccrc = p->srccrc;
		}

		char *GetString(int pos)	// Select which string in the data to return
		{
			static char		temp[MAX_STRING_SIZE];
			unsigned int	intpos = 0;
			unsigned int	getint = 0;
			unsigned int	charpos = 0;
			unsigned int	complete = 0;
			unsigned int	strcnt = 0;

			if(pos < 0) return NULL;

			// Until we get to the right string index
			while(!complete && (charpos < MAX_STRING_SIZE))
			{
				// Grab an int
				getint = GetDataUI(intpos++);

				// check for null chars
				if( (getint & 0xFF) && (getint & 0xFF00) && (getint & 0xFF0000) && (getint & 0xFF000000) )
				{
					if(pos == strcnt)
					{
						memcpy(&temp[charpos],(char *)&getint,sizeof(unsigned int));
						charpos+=sizeof(unsigned int);
					}
				}
				else
				{
					// Have reached null
					if(pos == strcnt)
					{
						for(int i=0;(i<4);i++)
						{
							temp[charpos++] = (getint >> (i*8)) & 0xFF;
							if(!temp[charpos-1]) break;
						}

						complete = 1;
					}
					else
					{
						strcnt++;
					}
				}
			}

			temp[charpos+1] = '\0';
			return temp;
		}

		// Nice operators to work with
		operator += (unsigned int _data) 
		{ 
			BaseClass::operator += ((unsigned int )_data); 
			AddTypeToField(P_UINT);
		}
		
		operator += (int _data) 
		{ 
			BaseClass::operator += ((unsigned int )_data); 
			AddTypeToField(P_INT);
		}
		
		operator += (float _data) 
		{ 
			BaseClass::operator += (*(unsigned int *)&_data); 
			AddTypeToField(P_FLOAT);
		}
		
		operator += (char _data) 
		{ 
			BaseClass::operator += ((unsigned int )_data); 
			AddTypeToField(P_CHAR);
		}
		
		operator += (unsigned char _data) 
		{ 
			BaseClass::operator += ((unsigned int )_data); 
			AddTypeToField(P_UCHAR);
		}

		operator += (long _data)
		{
			int sz = sizeof(long)/sizeof(unsigned int);
			while(sz--)
			{
				BaseClass::operator += ((unsigned int )(_data & 0xFFFFFFFFL));
				_data = _data >> 32;
			}
			AddTypeToField(P_LONG);
		}

		operator += (unsigned long _data)
		{
			int sz = sizeof(unsigned long)/sizeof(unsigned int);
			while(sz--)
			{
				BaseClass::operator += ((unsigned int )(_data & 0xFFFFFFFFL));
				_data = _data >> 32;
			}
			AddTypeToField(P_ULONG);
		}

		operator += (char *s)
		{
			int		len = strlen(s);
			int		intctr = len/4;
			int		left = 0;
			
			for(int i=0; i<intctr; i++)
			{
				 BaseClass::operator += ( *((unsigned int *)s) );
				 s+=4;
			}

			// check for odd ending
			left = (len%4);

			// Build the int
			unsigned int end = *((unsigned int *)s);
			switch(left)
			{
			case 3:	end &= 0x00FFFFFF;
				break;
			case 2: end &= 0x0000FFFF;
				break;
			case 1: end &= 0x000000FF;
				break;
			case 0: end &= 0x00000000;
				break;
			}
			BaseClass::operator += (end);

			AddTypeToField(P_STRING);
		}

		operator += (const char *s)
		{
			PackClass::operator += ((char *)s);
			AddTypeToField(P_STRING);
		}

	};
*/
	typedef vector<Packet::PackClass *> PacketVec;


/*
	typedef struct
	{
		enum T = {	
					VOIDPTR,
					CHAR, UNSIGNED_CHAR, CHARPTR, UNSIGNED_CHARPTR,
					INT, UNSIGNED_INT, INTPTR, UNSIGNED_INTPTR,
					FLOAT, FLOATPTR,
					LONG, UNSIGNED_LONG, LONGPTR, UNSIGNED_LONGPTR
				}
		int	cPos;	// char position in buffer type sits
	}

	// Stack based packet building?
	template <int i>
	class PStack
	{
		unsigned char	Buffer[i];	// byte blocks of mem for storage
		TypeInfo		dTypes[i];	// Can have a maximum of 1 type per char!
		int				maxItems;
		int				cItems
	public:
		void PStack( void ) : maxItems(i) : cItems(0) : { };
	};

	template <int i> void PStack<i >::operator += ( const void *_item )
	{
		if( cItems + sizeof(void *) > maxItems )
		{
			memcpy(&Buffer[cItems], &_item,sizeof(void *);
			dTypes[cItems].cPos = cItems;
			dTypes[cItems].T = VOIDPTR;
			cItems += sizeof(void *);
		}	
		else
		 throw "Stack overflow error.";
		return;
	}

	template <int i> T PStack<i >::pop( void )
	{
		if( cItems < i )
		 return StackBuffer[cItems++]
		else
		 throw "Stack underflow error.";
	}

	template <int i> int PStack<i >::Size( void )
	{
		return cItems;
	}

	template <int i> int PStack<i >::Find( const T item )
	{

		return cItems;
	}
*/
}

#endif



