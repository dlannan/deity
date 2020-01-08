#ifndef _MESH_TYPES_H_
#define _MESH_TYPES_H_

// This mesh namespace has all sorts of mesh related types.
// Many useful functions also for manipulating these types

namespace Mesh
{
	typedef struct {
		float	r,g,b,a;
	} ColourF;

	typedef struct {
		int		r,g,b,a;
	} ColourI;

	typedef struct {
		char	r,g,b,a;
	} ColourB;

	typedef struct {
		float	x,y,z,w;
	} VertexF;

	typedef struct {
		int		x,y,z,w;
	} VertexI;

	typedef struct {
		float	s,t;
	} UvF;

	typedef struct {
		int		s,t;
	} UvI;

	typedef struct {
		float	x,y,z,w;
	} NormalF;

	typedef struct {
		int		x,y,z,w;
	} NormalI;
}

#endif
