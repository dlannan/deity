// TGA File loader

#ifndef _TGA_FILE_H_
#define _TGA_FILE_H_

// Make sure compiler aligment doesnt stuff up our structure
#pragma pack(1)

typedef struct
{
	unsigned char	imageIdLength;	// Length of the Id field
	unsigned char	colorMapType;
	unsigned char	imageTypeCode;
	struct ColorMap
	{
		unsigned short	origin;
		unsigned short	length;
		unsigned char	entrySize;
	} cmap;
	struct Image
	{
		unsigned short	xOrigin;
		unsigned short	yOrigin;
		unsigned short	width;
		unsigned short	height;
		unsigned char	pixelSize;
		unsigned char	desc;
	} image;

} TGAHeader;

class TGAFile
{
private:
	TGAHeader	header;

	void		*imageIdField;
	void		*colorMapField;
	void		*imageData;
public:

	TGAFile();
	~TGAFile();

	TGAFile(const char *name);

	bool LoadTGA(const char *name);
	bool SaveTGA(const char *name);

	bool MakeIndexed();		// Convert to an indexed format - need good colour algo
	bool MakeRGBA();		// Covert indexed to a standard RGBA format

	inline void *GetImageIdField() { return imageIdField; }
	inline void *GetColorMap() { return colorMapField; }
	inline void *GetImageData() { return imageData; }
	inline TGAHeader *GetHeader() { return &header; }

	inline bool IsIndexed() { return (header.imageTypeCode == 0x01); }
	inline bool IsRGBA() {  return (header.imageTypeCode == 0x02); }

};

#endif