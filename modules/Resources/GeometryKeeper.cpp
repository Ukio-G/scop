// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "GeometryKeeper.hpp"
#include <GL/glew.h>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "Vertex.hpp"
#include "include/math.hpp"
#include "objLoader.hpp"
#pragma pack(1)

void GeometryKeeper::loadGeometryFromFile(const std::string &name,
                                          const std::string &objFilename) {
  // Initialize Loader
  obj::Loader Loader;

  // Load .obj File
  Loader.LoadFile(objFilename);

  auto &mesh = Loader.meshes.at(name);

  // Am I really need this copy?
  vertexData[name] = mesh.vertexes;
  indexesData[name] = mesh.indexes;

  // Randomize colors
  int i = 0;
  for (auto &vtx : vertexData[name]) {
    vtx.color[0] = 1.0f / static_cast<float>(1 + (i++) % 5);
    vtx.color[1] = 1.0f / static_cast<float>(1 + (i++) % 5);
    vtx.color[2] = 1.0f / static_cast<float>(1 + (i++) % 5);
  }

  unsigned int vertex_size = sizeof(geom::Vertex);
  size_t vertexes_bytes = vertexData[name].size() * vertex_size;
  size_t indexes_bytes = indexesData[name].size() * sizeof(unsigned int);

  std::cout << "name: " << name << std::endl;
  std::cout << "vertexes_bytes: " << vertexes_bytes << std::endl;
  std::cout << "indexes_bytes: " << indexes_bytes << std::endl;

  void *vertex_data = vertexData[name].data();
  void *indexes_data = indexesData[name].data();

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

  geometry[name] = {{VAO, VBO, EBO}, mesh.bbox,
                    indexesData[name].size(),
                    vertexData[name].size()
                    };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_bytes, indexes_data,
               GL_STATIC_DRAW);

  // Make VBO buffer active
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexes_bytes, vertex_data, GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (void *)0);
  glEnableVertexAttribArray(0);

  // Texture Coordinate
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size,
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Normal
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_size,
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Color
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertex_size,
                        (void *)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);
}