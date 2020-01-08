// TGA Loader

#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include "TGALoader.h"

TGAFile::TGAFile()
{
	memset(&header,0,sizeof(TGAHeader));
	imageIdField = NULL;
	colorMapField = NULL;
	imageData = NULL;
}

TGAFile::~TGAFile()
{
	// Make sure the associated data is destroyed
	if(imageIdField) delete [] imageIdField;
	if(colorMapField) delete [] colorMapField;
	if(imageData) delete [] imageData;
}

TGAFile::TGAFile( const char * name)
{ 
	memset(&header,0,sizeof(TGAHeader));
	imageIdField = NULL;
	colorMapField = NULL;
	imageData = NULL;

	LoadTGA(name); 
}

bool TGAFile::LoadTGA(const char *name)
{
	FILE	*fptr = NULL;

	fptr = fopen(name, "rb");
	if(!fptr) return false;

	// Read the header
	fread(&header,sizeof(TGAHeader),1,fptr);

	// Image identity field?
	if(header.imageIdLength)
	{
		imageIdField = (void *)new unsigned char[header.imageIdLength];
		fread(imageIdField,header.imageIdLength,1,fptr);
	}

	// Only support 2 types - will implement full system later
	if(header.imageTypeCode == 0x01)
	{
		// Indexed colourmap 
		colorMapField = new unsigned char[header.cmap.length * (header.cmap.entrySize >> 2)];
		fread(colorMapField,header.cmap.length * (header.cmap.entrySize >> 2),1,fptr);

		// Image data
		imageData = new unsigned char[header.image.width * header.image.height * 2];
		fread(imageData,header.image.width * header.image.height * 2,1,fptr);
	}
	else if(header.imageTypeCode == 0x02)
	{
		// No colourmap for this type
		imageData = new unsigned char[header.image.width * header.image.height * (header.image.pixelSize >> 2)];
		fread(imageData,header.image.width * header.image.height * (header.image.pixelSize >> 2),1,fptr);
	}

	fclose(fptr);
	return true;
}

bool TGAFile::SaveTGA(const char *name)
{
	FILE	*fptr = NULL;

	fptr = fopen(name,"wb");
	if(!fptr) return false;

	// Write the header
	fwrite(&header,sizeof(TGAHeader),1,fptr);

	// Write the image identity field
	if(header.imageIdLength)
	{
		if(imageIdField)
		{
			fwrite(imageIdField,header.imageIdLength,1,fptr);
		}
	}

	// Write the colour map
	if(header.imageTypeCode == 0x01)
	{
		if(colorMapField)
		{
			fwrite(colorMapField,header.cmap.length * (header.cmap.entrySize >> 2),1,fptr);
		}

		if(imageData)
		{
			fwrite(imageData, header.image.width * header.image.height * 2, 1, fptr);
		}
	}

	else if(header.imageTypeCode == 0x02)
	{
		// No color index table

		// Write the image data
		if(imageData)
		{
			fwrite(imageData, header.image.width * header.image.height * (header.image.pixelSize >> 2), 1, fptr);
		}
	}

	fclose(fptr);

	return true;
}

bool TGAFile::MakeIndexed() 	// Convert to an indexed format - need good colour algo
{
	return false;
}

bool TGAFile::MakeRGBA()		// Covert indexed to a standard RGBA format
{
	return false;
}


