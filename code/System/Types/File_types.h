// FileClass

#ifndef _FILE_TYPES_H_
#define _FILE_TYPES_H_

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include <stdio.h>
#include <vector>
#include "string_types.h"
#include "Binary_types.h"

using namespace std;

namespace File
{
	#define		FILE_READ				0x001
	#define		FILE_WRITE				0x002
	#define		FILE_APPEND				0x004
	#define		FILE_TEXT				0x008
	#define		FILE_BINARY				0x010

	#define		FILE_SEEK_END			SEEK_END
	#define		FILE_SEEK_CUR			SEEK_CUR
	#define		FILE_SEEK_SET			SEEK_SET

	#define		FILE_OK					0
	#define		FILE_ERROR				-1

	#define		FILE_ALREADYOPEN		10
	#define		FILE_INVALID_FILENAME	11
	#define		FILE_INVALID_FORMAT		12
	#define		FILE_NO_FORMAT_SET		13

	#define		FILE_READ_ERROR			20

	#define		MAX_PAGE_WIDTH			0xFFFF

	// Hopefully we can eventually do this   
	//				DataBuffer << zeusFile		to read
	//				DataBuffer >> zeusFile		to write
	// The operation depends on the pointer type
	// unsigned char or int is binary file, char is text

	class FileClass {
		struct FormatType{
			unsigned int	flags;
		};

	public:
		FILE				*handle;	// Handle to file
		Data::StringClass	name;		// Filename - full path included please
		Data::StringClass	sformat;	// read, write, binary, text etc..
		FormatType			format;
		CRC::StringClass	id;

		FileClass()
		{
			handle = NULL;
			format.flags = 0;
		}

		FileClass(Data::StringClass *fname, unsigned int method)
		{
			Open(fname, method);
		}

		~FileClass()
		{
			// Make sure its closed
			if(handle) Close();
		}

		int Open(const char *fname, unsigned int method)
		{
			ASSERT(fname == NULL, "File::Open")
			ASSERT(method == 0 ,"File::Open")
			if((fname == NULL) || (method == 0)) return FILE_ERROR;	// Dont do anything stupid ok?!

			if(method & 0xFFE0) return FILE_INVALID_FORMAT;

			// Must call close or destroy to re-open a file
			if(handle) return FILE_ALREADYOPEN;

			BuildFormat(method);
			name = fname;
			// Set the id
			id << (char *)name.String();
			
			// Try and open file
			handle = fopen(name.String(), sformat.String());

			// Wasnt able to open file.. at the moment.. who cares why..
			if(!handle)
			{
				return FILE_ERROR;
			}

			return FILE_OK;
		}

		int Open(Data::StringClass *fname, unsigned int method)
		{
			return Open(fname->String(), method);
		}

		void Close(void)
		{
			ASSERT(handle == NULL, "File::CloseFile")	// Closing an unopened file!
			if(handle == NULL) return;	// Dont do anything stupid ok?!

			fclose(handle);
		}

		// ------------------------------------------------------------------------
		// SEEKING
		int SeekFile(unsigned long pos, int seektype)
		{
			ASSERT(handle == NULL, "File::SeekFile")
			if(handle == NULL) return FILE_ERROR;	// Dont do anything stupid ok?!

			if(fseek(handle, pos, seektype) == 0)
				return FILE_OK;
			else
				return FILE_ERROR;
		}

		// ------------------------------------------------------------------------
		// READING
		int ReadTextFile(Data::PageType *pg)
		{
			ASSERT( handle == NULL, "File::ReadTextFile");
			if(handle == NULL) return FILE_ERROR;	// Dont do anything stupid ok?!

			char temp[MAX_PAGE_WIDTH];
			Data::StringClass	sc;

			fgets(temp, MAX_PAGE_WIDTH, handle);
			if(temp)
			{
				sc << temp;
				pg->push_back(sc);
				sc.Clear();
			}

			while(temp && !feof(handle))
			{
				fgets(temp, MAX_PAGE_WIDTH, handle);
				if(temp)
				{
					sc << temp;
					pg->push_back(sc);
					sc.Clear();
				}
			}	

			return FILE_OK;
		}

		int ReadBinFile(Data::BlockType *bin)
		{
			ASSERT( handle == NULL, "File::ReadBinFile");
			if(handle == NULL) return FILE_ERROR;	// Dont do anything stupid ok?!

			unsigned char	*temp = NULL;
			unsigned int	count = 0;
			unsigned int	size;

			fseek(handle, 0, SEEK_SET);
			fseek(handle, 0, SEEK_END);
			size = ftell(handle);
			ASSERT(size <= 0, "Zero sized file!");
			fseek(handle, 0, SEEK_SET);	// If we dont do this, the read will fail!

			temp = new unsigned char[size];
			count = fread(temp, 1, size, handle);
			if(count > 0)
			{
				bin->AddData(temp,count);
			}

			delete [] temp;

			return FILE_OK;
		}

		void operator >> (Data::PageType *pg)
		{
			if(!pg) return;
			ReadTextFile(pg);
		}

		void operator >> (Data::BlockType *bin)
		{
			if(!bin) return;
			ReadBinFile(bin);
		}

		// ------------------------------------------------------------------------
		//WRITING
		int WriteTextFile(Data::PageType *pg)
		{
			ASSERT( handle == NULL, "File::WriteTextFile");
			if(handle == NULL) return FILE_ERROR;	// Dont do anything stupid ok?!

			for(int lines=0; lines<pg->size(); lines++)
			{
				if((*pg)[lines].String())
				{
					fputs((*pg)[lines].String(), handle);
				}
			}

			return FILE_OK;
		}
	
		int WriteBinFile(Data::BlockType *bin)
		{
			ASSERT( handle == NULL, "File::WriteBinFile");
			if(handle == NULL) return FILE_ERROR;	// Dont do anything stupid ok?!

			fwrite(bin->GetData(),bin->GetUsed(), 1, handle);
			return FILE_OK;
		}

		void operator << (Data::PageType *pg)
		{
			if(!pg) return;
			WriteTextFile(pg);
		}

		void operator << (Data::BlockType *pg)
		{
			if(!pg) return;
			WriteBinFile(pg);
		}


	private:
		void BuildFormat(unsigned int fmat)
		{
			// Important.. default mode is text mode.. any binary flag
			// overrides this..
			format.flags = fmat;
			if(fmat&FILE_APPEND)
			{
				sformat << "+";
			}
			if(fmat&FILE_READ)
			{
				sformat << "r";
			}
			if(fmat&FILE_WRITE)
			{
				sformat << "w";
			}
			if(fmat&FILE_BINARY)
			{
				sformat << "b";
			}
		}
	};

}

#endif