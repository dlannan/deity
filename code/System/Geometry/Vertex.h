// Geometry types

#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Matrix.h"

#pragma pack(1)

#define			VERTEX_COLOR	0x01
#define			VERTEX_XYZ		0x02
#define			VERTEX_NORMAL	0x04
#define			VERTEX_TEX1		0x08
#define			VERTEX_TEX2		0x10

// Some basic geomtry types
typedef struct Vector3 Vertex3;

typedef struct Vector4 Vertex4;

typedef struct Vector3 Normal3;

typedef struct Vector4 Normal4;

typedef struct Vector2 Texel;

typedef struct Vector4 Color;

struct ColorChar4
{
	union
	{
		struct 
		{
			unsigned char	r;
			unsigned char	g;
			unsigned char	b;
			unsigned char	a;
		};
		unsigned int	rgba;
	};

	ColorChar4(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) { Set(r,g,b,a); }
	~ColorChar4() {}

	inline Vector4 ToVector() 
	{
		Vector4 tmp;
		tmp.r = r/255.0;tmp.g = g/255.0;tmp.b = b/255.0;tmp.a = a/255.0;
		return tmp;
	}

	inline void Set(float r, float g, float b, float a = 1.0f)
	{
		r = (unsigned char)(r/255.0f);g = (unsigned char)(g/255.0f);
		b = (unsigned char)(b/255.0f);a = (unsigned char)(a/255.0f);
	}
};

typedef struct ColorChar4 ColorChar4;

typedef struct 
{
	Vertex3		p;		// The untransformed position for the vertex.
	Normal3		n;		// normal for the vertex
	ColorChar4	c;		// vertex color - in an unsigned int
	Texel		t;		// texture coordinates
} VertexType;

// Vertex related
typedef struct
{
	Vertex3		o;		// Origin - top left
	Vertex3		s;		// Size - bottom right (relative to Origin)
	
} RectType;

#endif