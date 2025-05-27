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
	Loader.LoadFile(objFilename);

        auto& mesh = Loader.meshes.at(name);
        // Copy data (Very bad, yea, need own obj parser)
        size_t vertexes_bytes = mesh.vertexes.size() * sizeof(geom::Vertex);
        size_t indexes_bytes = mesh.indexes.size() * sizeof(size_t);

        // Am I really need this copy?
        vertexData[name] = mesh.vertexes;
        indexesData[name] = mesh.indexes;

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


        geometry[name] = {{VAO, VBO, EBO}, (Vertex*)vertex_data, (unsigned int*)indexes_data, vertexData[name].size(), indexesData[name].size()};
        // generateTBN(geometry[name]);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_bytes, indexes_data, GL_STATIC_DRAW);

        // Make VBO buffer active
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexes_bytes, vertex_data, GL_STATIC_DRAW);

        unsigned int vertex_size = sizeof(geom::Vertex);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)0);
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture Coordinate
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Colors
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
}