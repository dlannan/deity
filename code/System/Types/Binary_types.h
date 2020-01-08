// Array types

#ifndef _BINARY_TYPES_H_
#define _BINARY_TYPES_H_

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include <vector>
#include "func_types.h"

// We make 64K 'lines' of data.. from BinClass.. then put these 
// in BinType array

using namespace std;

namespace Data
{
	// Default minimum data block size
	const int binDataMin = 1024;	
	
	// -------------------------------------------------------------------------------------------------
	// Binary Class - simplistic memory allocation for loading and keeping binary data
	//
	// Important: Should not be used for adding data to during main loops. Data deleting and allocing
	// occurs, and its not dynamic its contiguous.
	class BinClass {
	private:
		unsigned char	*data;
		unsigned char	*curr;
		unsigned int	used;
		unsigned int	max;

	public:
		BinClass(unsigned int dataSize = binDataMin)
		{
			ASSERT(dataSize <= 0, "Invalid DataSize");

			used = 0;
			max = dataSize;
			data = new unsigned char[max];
			curr = data;

		}

		~BinClass()
		{
			if(data) delete [] data;
			curr = NULL;
		}

		void Clear()
		{
			if(data) memset(data,0,max);
			used = 0;
			curr = data;
		}

		inline int GetUsed()
		{
			return used;
		}

		inline const int GetMax() const
		{
			return max;
		}

		// Messy - needs some work.
		inline void CheckFree(unsigned int size)
		{
			// resize
			if( (size > (max-used)) && (data != NULL) )
			{
				unsigned int offset = curr - data;
				max = used+size;
				unsigned char *newdat = new unsigned char [max];
				memcpy(newdat, data, used);
				delete [] data;
				data = newdat;
				curr = data + offset;
			}
		}

		inline void operator << (unsigned int _data)
		{
			CheckFree(sizeof(_data));
			*(unsigned int *)curr = _data;
			curr += sizeof(_data);
			used += sizeof(_data);
		}

		inline void operator << (int _data)
		{
			CheckFree(sizeof(_data));
			*(int *)curr = _data;
			curr += sizeof(_data);
			used += sizeof(_data);
		}

		inline void operator << (long _data)
		{
			CheckFree(sizeof(_data));
			*(long *)curr = _data;
			curr += sizeof(_data);
			used += sizeof(_data);
		}

		inline void operator << (unsigned char _data)
		{
			CheckFree(sizeof(_data));
			*(unsigned char *)curr = _data;
			curr += sizeof(_data);
			used += sizeof(_data);
		}

		inline void operator << (char _data)
		{
			CheckFree(sizeof(_data));
			*(char *)curr = _data;
			curr += sizeof(_data);
			used += sizeof(_data);
		}

		inline void AddData(unsigned char *_data, unsigned int sz)
		{
			CheckFree(sz);
			memcpy(curr,_data,sz);
			curr += sz;
			used += sz;
		}

		inline void AddData(void *_data, unsigned int sz)
		{
			CheckFree(sz);
			memcpy(curr,_data,sz);
			curr += sz;
			used += sz;
		}

		inline unsigned char *GetData(void)
		{
			return data;
		}

		inline unsigned char *GetData(unsigned int offset)
		{
			ASSERT( (offset > max) || (offset < 0), "Bad offset"); 
			return &data[offset];
		}
	};

	typedef Data::BinClass BinType;

	// Bintree is a simple binary tree sorting list. 
	// As each element is inserted it is sorted - the root node may change
	template <class T>
	class BinTree {
	private:
		vector<BinType *>	blist;
		
	public:
		void Add(T *data) 
		{
//			memcpy(temp,f,sizeof(Function));
		}

		void Find(CRC::StringClass id)
		{
		}

		bool Empty(void)
		{
			return blist.empty();
		}
	};

	typedef Data::BinType	BlockType;
}

#endif