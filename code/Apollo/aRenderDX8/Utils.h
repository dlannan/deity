// Utility functions 
#ifndef _RENDER_UTILS_H_
#define _RENDER_UTILS_H_

#include "..\\..\\System\\System.h"

#include "aRenderDX8.h"
#include "Primitive.h"
#include "RenderVertex.h"
#include "Utils.h"

namespace aRenderDX8
{
	inline unsigned int GetShaderType(unsigned int format);
	inline unsigned int GetShaderSize(unsigned int format);

	void FillVertexBuffer(VertexPtrType *v);
	void BuildWindowVerts(VertexPtrType *v);
	void BuildFontVerts(VertexPtrType *v);

}

#endif