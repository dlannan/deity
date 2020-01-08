
#ifndef _aSpaceRouter_H_
#define _aSpaceRouter_H_

#include "..\\..\\System\\System.h"

// Dont forget to add includes if necessary!

extern ApolloClass	apollo;

namespace aSpaceRouter 
{
	// SpaceRouter is a spatialiser that routes packets based on requests from modules.

	typedef struct 
	{
		Data::StringClass	type;		// Some sort of node type label

		Vector3				origin;		// Position of object
		Vector3				min;		// Bounding box minimum
		Vector3				max;		// Bounding box maximum
	
		float				radius;		// Sphere check radius
		unsigned int		mag;		// Magnitude - dimensional size (like a multiplier)

		CRC::ModIdClass		ownerModule;// Should be a module ID
		CRC::ModIdClass		ownerObject;// Should be a object ID
	} Object;


	//                  ___________
	//			      /  1  /  2  /|      ^
	//              /____ /____ /  |      | Y Axis
	//            /  3  /  4  /| 2 |      |
	//          /____ /____ /  |  /|      |
	//		   |     |     | 4 |/  |      |       X Axis
	//   5---->|  3  |  4  |  /| 6 |      /-------->
	//         |_____|_____|/  |  /     /
	//         |     |     | 8 |/     /  Z Axis
	//         |  7  |  8  |  /     /   
	//         |_____|_____|/

	struct SpatialNode
	{
		SpatialNode		*child1;		
		SpatialNode		*child2;
		SpatialNode		*child3;
		SpatialNode		*child4;
		SpatialNode		*child5;
		SpatialNode		*child6;
		SpatialNode		*child7;
		SpatialNode		*child8;

		SpatialNode		*parent;

		vector<Object *>	objects;
	};

	typedef struct SpatialNode SpatialNode;

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();
}

#endif
