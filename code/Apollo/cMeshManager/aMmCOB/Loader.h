// TP_cob_class.h: interface for the TP_cob_class class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _COB_LOADER_H_
#define _COB_LOADER_H_

// Mainly for geometry.. and anything else that takes your fancy
#include "..\\..\\..\\System\\System.h"
#include "..\\MeshClass.h"

extern ApolloClass	apollo;

namespace aMmCOB 
{
	int LoadCOB(MeshType *mesh);

	// AMke sure the structures get the right packing!
	#pragma pack(1)

	#define	COB_OKAY	0x1000

	#define	F_HOLE		0x08;
	#define F_BACKCULL	0x10;

	// Chunk type definitions
	#define	CH_TYPE_END		"END "
	#define	CH_TYPE_GROUP	"Grou"
	#define	CH_TYPE_POLY	"PolH"
	#define	CH_TYPE_MAT		"Mat1"
	#define	CH_TYPE_TEXTURE	"PrTx"

	// Caligari File Header (v2.0)
	typedef struct {
		char	fh_name[9];
		char	fh_version[6];
		char	fh_format;
		char	fh_endian[2];
		char	fh_blank[13];
		char	fh_newline;
	} COBFileHeaderType;

	// Chunk Header 
	typedef struct {
		char	ch_type[4];
		short	ch_majorver;
		short	ch_minorver;
		long	ch_chunkid;
		long	ch_parentid;
		long	ch_length;
	} COBChunkHeaderType;

	// Elemet type
	typedef struct {
		short	s_length;
		char	*s_string;
	} COBStringType;

	typedef struct {
		short	n_dcount;
		COBStringType	n_string;
	} COBNameType;

	typedef struct {
		float	x;
		float	y;
		float	z;
	} COBCoordType;

	typedef struct {
		float	a;
		float	b;
		float	c;
		float	d;
	} COBMatrixType;

	typedef struct {
		long	gid;
		Vertex3	mv;
		Matrix	trans;
		Matrix	orient;
	} COBGroupType;

	typedef struct {
		COBCoordType	la_center;
		COBCoordType	la_xaxis;
		COBCoordType	la_yaxis;
		COBCoordType	la_zaxis;
	} COBLocalAxesType;

	typedef struct {
		Matrix		cp_ma;
	} COBCurrentPositionType;

	typedef struct {
		long	lv_number;
		COBCoordType	*lv_vertices;
	} COBLocalVertexType;

	typedef struct {
		float	u;
		float	v;
	} COBUVPairType;

	typedef struct {
		long	uv_number;
		COBUVPairType	*uv_pairs;
	} COBUVVertexListType;

	typedef struct {
		long	fh_vertex;
		long	fh_uvindex;
	} COBIndexType;

	typedef struct {
		unsigned char	f_flags;
		short			f_number;
		short			f_material;
		COBIndexType	*f_vertices;
	} COBFaceType;

	typedef struct {
		unsigned char	h_flags;
		short			h_number;
		COBIndexType	*h_vertices;
	} COBHoleType;

	/// Not good idea!!
	// TODO - FIX THIS!
	// Material structure
	typedef struct {
		short			number;		// identity of material
		char			shader;		// 'f'-flat,'p'-phong,'m'-metal
		char			facettype;	// 'f'-faceted,'a'-autofaceted,'s'-smooth
		char			autofacet;	// autofacet angle (0-179 degrees)
		float			r,g,b;
		float			alpha;		// opacity??? (0.0-1.0)
		float			ambient;	// (0.0-1.0)
		float			specular;	// (0.0-1.0)
		float			highlight;	// (0.0-1.0)
		float			refrac;		// Index of refraction.
	} material_type;
}

#endif
