#pragma once

#include <vector>
#include <iostream>
#include "Debug3DLine.hpp"
#include "Graphic/Object3D.hpp"
#include "IO/ShadersControls.hpp"
#include "math.hpp"

class Subspace
{
  public:

  Subspace(const glm42::mat4 & subspace_matrix,
          const glm42::vec3& grid_color = glm42::vec3(0.5f, 0.5f, 0.5f),
          int grid_from = -10, int grid_to = 10, int grid_step = 1)
    : m_grid(grid_color, grid_from ,grid_to, grid_step), m_subspace_matrix(subspace_matrix) { }

  struct Grid
  {
    Grid(const glm42::vec3& color, int from = -10, int to = 10, int step = 1)
        : m_from(from), m_to(to), m_step(step), m_color(color)
    {
      lines.reserve( 1000 );
      for(int z = m_from; z < m_to; z++)
      {
        lines.emplace_back(glm42::vec3{m_from, 0.0f, z}, glm42::vec3{m_to, 0.0f, z}, m_color);
      }

      for( int x = m_from; x < m_to; x++ )
      {
        lines.emplace_back( glm42::vec3{ x, 0.0f, m_from }, glm42::vec3{ x, 0.0f, m_to }, m_color );
      }
    }

    int m_from;
    int m_to;
    int m_step;
    glm42::vec3 m_color;

    ~Grid() {}

    std::vector< Debug3DLine > lines;

    void draw(  ) const
    {
      for (const auto &line: lines) {
        line.draw();
      }
    }
  } m_grid;

  struct Basis
  {
    Basis()
        : m_lines{
              { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 } },
              { { 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 } },
              { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
          }
    {
    } 

    Debug3DLine m_lines[ 3 ];

    void draw() const
    {
      m_lines[ 0 ].draw(  );
      m_lines[ 1 ].draw(  );
      m_lines[ 2 ].draw(  );
    }
  } m_basis;


  void addObj(Object3D* obj)
  {
    m_objects.push_back(obj);
  }

  void initUBO(ShadersControls& shaders)
  {
    ensureSubspaceUBO();
    updateSubspaceUBO();

    bindSubspaceBlock(shaders.getLineProgram());
    bindSubspaceBlock(shaders.getObjectsProgram());

    glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)ShaderProgram::BindingPoint::SubspaceData, m_UBO);
  }

  void draw( ShadersControls& shaders ) const {
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)ShaderProgram::BindingPoint::SubspaceData, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm42::mat4), &(this->m_subspace_matrix));

    shaders.useLineShaders();
    m_grid.draw();
    m_basis.draw();
    for (const auto& line : m_lines)
    {
      line->draw();
    }

    shaders.useObjectsShader();
    for (const auto& obj : m_objects)
    {
      obj->draw();
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  glm42::mat4 m_subspace_matrix;
  std::vector<Object3D*> m_objects;
  std::vector<Debug3DLine*> m_lines;
  mutable GLuint m_UBO = 0;

private:
  void ensureSubspaceUBO() const
  {
    if (m_UBO != 0)
      return;
    glGenBuffers(1, &m_UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm42::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void updateSubspaceUBO() const
  {
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm42::mat4), &m_subspace_matrix);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void bindSubspaceBlock(ShaderProgram* program) const
  {
    if (!program)
      return;
    GLuint blockIndex = glGetUniformBlockIndex(program->shaderProgram, "SubspaceData");
    if (blockIndex == GL_INVALID_INDEX)
    {
      std::cerr << "ERROR! Unable to find block index for subspace data" << std::endl;
      return;
    }
    glUniformBlockBinding(program->shaderProgram, blockIndex, (GLuint)ShaderProgram::BindingPoint::SubspaceData);
  }
};
