
#ifndef _aMaterial_H_
#define _aMaterial_H_

#include <D3DX8.h>
#include <D3d8.h>
#include <D3dx8tex.h>

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include <vector>

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

extern ApolloClass	apollo;

namespace aMaterial 
{
	#define MAX_TEXTURE_HANDLES		10		// 10 per material is plenty to start with

	// ---------------------------------------------------------------------------------
	// Textures
	//

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


	typedef struct {
		D3DTEXTUREADDRESS	dxaddrmode;		// wrap, mirror etc
		IDirect3DTexture8	*dxtex;
	} DXTEXTURE;

	typedef struct {
		void		*texdata;	// Texel data
		void		*paldata;	// Indexed palette data (if any)

		int			w,h;		// Width & Height
		int			c;			// Number of bytes components per pixel
		int			i;			// Indexed palette size - non-indexed should be 0

		int			lod;		// useful for various apps
		int			clamp;		// clamp info bits
	} GENTEXTURE;
	
	typedef struct {
		// Texture stuff
		Data::StringClass	texfilename;

		union 
		{
			DXTEXTURE	*dxtex;
			GENTEXTURE	*ogltex;
			GENTEXTURE	*ps2tex;
		};
	} TextureType;

	// ---------------------------------------------------------------------------------
	// Materials
	//
	// OGL structure for keeping material info - used for a generic material
	// Color indexes not supported
	typedef struct GenMaterial {

		Vector4 amb;
		Vector4	diff;
		Vector4 spec;
		Vector4	emit;

		float shiny;

	} GENMATERIAL;


	// Simple material structure
	struct MaterialType {
		CRC::StringClass		id;		// Assignable id for a material - use in 3DS etc.
		vector<unsigned int>	tex;

		union
		{
			GENMATERIAL			*oglmat;
			D3DMATERIAL8		*dxmat;
			GENMATERIAL			*ps2mat;
		};
	};

	typedef struct MaterialType MaterialType;

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();

	// Submission functions
	void	SetRenderDevice(Packet::PackClass *p);

	void	CreateMaterial(Packet::PackClass *p);
	void	RemoveMaterial(Packet::PackClass *p);
	void	FindMaterial(Packet::PackClass *p);

	void	SetProperties(Packet::PackClass *p);

	void	LoadTexture(Packet::PackClass *p);
	void	UnloadTexture(Packet::PackClass *p);
	void	FindTexture(Packet::PackClass *p);
	
	void	AddTexture(Packet::PackClass *p);
	void	RemoveTexture(Packet::PackClass *p);

}

#endif
