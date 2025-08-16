#ifndef BUFFERS_COLLECTION_HPP
#define BUFFERS_COLLECTION_HPP

#include "Vertex.hpp"
#include <cstddef>
#include <tuple>
#include "math.hpp"

struct BuffersCollection {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

struct BoundBox {
	struct Dim {
		float min = 0.f;
		float max = 0.f;
	};
	Dim x {};
	Dim y {};
	Dim z {};
};

struct Geometry {
	BuffersCollection buffers;

	BoundBox bbox {};
	glm42::vec3 bbox_center {};

	size_t indexes_count;
	size_t vertexes_count;
};

#endif
