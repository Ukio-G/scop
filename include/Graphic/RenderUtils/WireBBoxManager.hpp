
#include <cstddef>
#include <iostream>
#include "Graphic/Object3D.hpp"
#include "Graphic/Shader.hpp"
#include "IO/ShadersControls.hpp"
#include "cfg_parser.hpp"
#include "math.hpp"

class WireBBoxManager {

public:
  WireBBoxManager() = default;

  void init(GLuint frameUBO) {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    glBindVertexArray(0);

    auto& shaders = config_ptr->sections[ "shaders" ];

    Shader vs( std::get< std::string >( shaders[ "boundbox_vs" ] ).c_str(), GL_VERTEX_SHADER );
    Shader fs( std::get< std::string >( shaders[ "boundbox_fs" ] ).c_str(), GL_FRAGMENT_SHADER );

    m_shaderProgram = std::make_unique< ShaderProgram >( &vs, &fs );

    m_frameUBO = frameUBO;

    glGenBuffers(1, &m_objectUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_objectUBO);

    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm42::mat4), nullptr, GL_DYNAMIC_DRAW);

    unsigned int frameDataIdx = glGetUniformBlockIndex(m_shaderProgram->shaderProgram, "FrameData");
    if (frameDataIdx == GL_INVALID_INDEX)
      std::cerr << "ERROR: FrameData block not found in bbox program\n";
    else
      glUniformBlockBinding(m_shaderProgram->shaderProgram, frameDataIdx, (GLuint) ShaderProgram::BindingPoint::FrameData);

    unsigned int objDataIdx = glGetUniformBlockIndex(m_shaderProgram->shaderProgram, "ObjectData");
    if (objDataIdx == GL_INVALID_INDEX)
      std::cerr << "ERROR: ObjectData block not found in bbox program\n";
    else
      glUniformBlockBinding(m_shaderProgram->shaderProgram, objDataIdx, (GLuint) ShaderProgram::BindingPoint::ObjectData);

    unsigned int subspaceDataIdx = glGetUniformBlockIndex(m_shaderProgram->shaderProgram, "SubspaceData");
    if (subspaceDataIdx == GL_INVALID_INDEX)
      std::cerr << "ERROR: SubspaceData block not found in bbox program\n";
    else
      glUniformBlockBinding(m_shaderProgram->shaderProgram, subspaceDataIdx, (GLuint) ShaderProgram::BindingPoint::SubspaceData);

    // User-defined binding point number and just generated buffer with glGenBuffers(...)
    glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)ShaderProgram::BindingPoint::ObjectData, m_objectUBO);
    glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)ShaderProgram::BindingPoint::FrameData, m_frameUBO);
    glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)ShaderProgram::BindingPoint::SubspaceData, m_subspaceUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void draw(const Object3D& obj) {
      glm42::mat4 bboxLocal = glm42::mat4::id();
      auto bbox = obj.getBoundBox();
      auto modelMatrix = obj.getModelMatrix();

      float dx = bbox.x.max - bbox.x.min;
      float dy = bbox.y.max - bbox.y.min;
      float dz = bbox.z.max - bbox.z.min;

      bboxLocal = glm42::scale(bboxLocal, {dx, dy, dz, 1});

      m_shaderProgram->use();
      m_shaderProgram->setMatrix4d("uBboxScale", bboxLocal);

      glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)ShaderProgram::BindingPoint::ObjectData, m_objectUBO);
      glBindBuffer(GL_UNIFORM_BUFFER, m_objectUBO);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm42::mat4), &obj.getModelMatrix());
      glBindVertexArray(m_VAO);

      glLineWidth(1.0f);
      glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, (void*)0);

      glBindVertexArray(0);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }


private:
  float vertex_positions[48] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f
  };

  uint16_t indexes[24] = {
  0,1,  2,3,  4,5,  6,7,
  0,2,  1,3,  4,6,  5,7,
  0,4,  1,5,  2,6,  3,7
  };

  GLuint                           m_VAO = 0, m_VBO = 0, m_EBO = 0;
  GLuint                           m_objectUBO = 0;
  GLuint                           m_frameUBO = 0;
  GLuint                           m_subspaceUBO = 0;
  std::unique_ptr< ShaderProgram > m_shaderProgram;
};
