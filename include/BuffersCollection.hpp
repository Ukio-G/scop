#ifndef BUFFERS_COLLECTION_HPP
#define BUFFERS_COLLECTION_HPP

#include "Vertex.hpp"
#include <cstddef>
#include <tuple>
#include "math.hpp"

struct BuffersCollection {
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
};


struct Geometry {
	BuffersCollection buffers;

	geom::BoundBox bbox {};

	size_t indexes_count = 0;
	size_t vertexes_count = 0;
};

#endif
