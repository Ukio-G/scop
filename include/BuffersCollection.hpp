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


struct Geometry {
	BuffersCollection buffers;

	geom::BoundBox bbox {};

	size_t indexes_count;
	size_t vertexes_count;
};

#endif
