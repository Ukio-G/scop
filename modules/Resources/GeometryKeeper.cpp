#include <GL/glew.h>
#include <cstring>
#include <stdexcept>
#include <vector>
#include "GeometryKeeper.hpp"

#include "objLoader.hpp"
#include "Vertex.hpp"
#include "include/math.hpp"

void GeometryKeeper::loadGeometryFromFile(const std::string & name, const std::string &objFilename) {
	// Initialize Loader
	obj::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile(objFilename);

	// If so continue
	if (loadout)
	{
		// Copy data (Very bad, yea, need own obj parser)
		size_t vertexes_bytes = Loader.LoadedVertices.size() * sizeof(objl::Vertex);
		size_t indexes_bytes = Loader.LoadedIndices.size() * sizeof(unsigned int);

		vertexData[name] = Loader.LoadedVertices;
		indexesData[name] = Loader.LoadedIndices;

		void* vertex_data = vertexData[name].data();
		void* indexes_data = indexesData[name].data();

		// Generate Vertex Array Object
		unsigned int VAO;

		// Create VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Generate Vertex Buffers
		unsigned int VBO, EBO;

		// Create VBO buffer
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);


		geometry[name] = {{VAO, VBO, EBO}, (Vertex*)vertex_data, (unsigned int*)indexes_data, Loader.LoadedVertices.size(), Loader.LoadedIndices.size()};
		// generateTBN(geometry[name]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_bytes, indexes_data, GL_STATIC_DRAW);

		// Make VBO buffer active
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexes_bytes, vertex_data, GL_STATIC_DRAW);

		unsigned int vertex_size = sizeof(objl::Vertex);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)0);
		glEnableVertexAttribArray(0);

		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture Coordinate
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Tangents
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// Bitangents
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)(11 * sizeof(float)));
		glEnableVertexAttribArray(4);

		glBindVertexArray(0);
	} else {
		throw std::runtime_error("Unable to read data from: " + objFilename);
	}
}

void GeometryKeeper::newGeometry(const std::string & name, const std::vector<Vertex> &data, const std::vector<int> &indexes) {
	newGeometry(name, data.data(), indexes.data(), data.size(), indexes.size());
}


void GeometryKeeper::newLineGeometry(const std::string & name, const glm42::vec3& start, const glm42::vec3& end) {

	vertexData[name] = {
		{{start[0], start[1], start[2]}, {0.0, 0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
        {{end[0], end[1], end[2]}, {0.0, 0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
	};

	
	// Generate Vertex Array Object
	unsigned int VAO;

	// Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate Vertex Buffers
	unsigned int VBO;

	// Create VBO buffer
	glGenBuffers(1, &VBO);

	void* vertex_data = vertexData[name].data();
	unsigned int vertex_size = sizeof(objl::Vertex);
	int vertexes_bytes = vertexData[name].size() * vertex_size;

	geometry[name] = {{VAO, VBO, 0}, (Vertex*)vertex_data, nullptr, 0, 2};
	
	// Make VBO buffer active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexes_bytes, vertex_data, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void GeometryKeeper::newLineStripGeometry(const std::string & name, const std::vector<glm42::vec3> & points) {
	vertexData[name].reserve(points.size());

	for(auto & point: points) {
		vertexData[name].push_back({
			{point[0], point[1], point[2]}, {0.0, 0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}
		});
	}

	void* vertex_data = vertexData[name].data();
	unsigned int vertex_size = sizeof(objl::Vertex);
	int vertexes_bytes = vertexData[name].size() * vertex_size;

	// Generate Vertex Array Object
	unsigned int VAO;

	// Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate Vertex Buffers
	unsigned int VBO;
	geometry[name] = {{VAO, VBO, 0}, (Vertex*)vertex_data, nullptr, 0, vertexData[name].size()};

	// Create VBO buffer
	glGenBuffers(1, &VBO);

	// Make VBO buffer active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexes_bytes, vertex_data, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void GeometryKeeper::newGeometry(const std::string & name, const Vertex *vertexData, const int *indexes, size_t vertexes_count, size_t indexes_count) {
	// Generate Vertex Array Object
	unsigned int VAO;

	// Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate Vertex Buffers
	unsigned int VBO, EBO;

	// Create VBO buffer
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	unsigned int vertex_size = sizeof(objl::Vertex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indexes)) * indexes_count, indexes, GL_STATIC_DRAW);

	// Make VBO buffer active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexes_count, vertexData, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// Bitangents
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);

	//geometry[name] = {{VAO, VBO, EBO}, vertexData, (const unsigned int*)indexes,  vertexes_count, indexes_count};
	geometry[name] = {{VAO, VBO, EBO}, (Vertex*)vertexData, (unsigned int*)indexes, vertexes_count, indexes_count};
	glBindVertexArray(0);
}