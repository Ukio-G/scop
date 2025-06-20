#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "math.hpp"

struct alignas(sizeof(float) * 8) Vertex {
	typedef float PositionType;
	typedef float UVType;

	PositionType pos[3];
	PositionType normal[3];
	UVType       textureUV[2];
};



#endif