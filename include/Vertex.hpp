#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "math.hpp"

struct alignas(sizeof(float) * 16) Vertex {
	typedef float PositionType;
	typedef float UVType;

	PositionType pos[3];
	PositionType normal[3];
	UVType       textureUV[2];

	glm42::vec3	 tangent;
	glm42::vec3	 bitangent;
};



#endif