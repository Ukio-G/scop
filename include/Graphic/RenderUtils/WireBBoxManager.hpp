
#include <cstddef>
#include "Graphic/Object3D.hpp"
#include "Graphic/Shader.hpp"
#include "cfg_parser.hpp"
#include "math.hpp"

class WireBBoxManager {

public:

  WireBBoxManager() = default;

  void init() {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    glBindVertexArray(0);

    auto& shaders = config_ptr->sections[ "shaders" ];

    Shader vs( std::get< std::string >( shaders[ "boundbox_vs" ] ).c_str(), GL_VERTEX_SHADER );
    Shader fs( std::get< std::string >( shaders[ "boundbox_fs" ] ).c_str(), GL_FRAGMENT_SHADER );

    m_shaderProgram = std::make_unique< ShaderProgram >( &vs, &fs );
  }



  void draw(const glm42::mat4& view, const glm42::mat4& projection, const Object3D& obj) {
      glm42::mat4 bboxLocal = glm42::mat4::id();
      auto bbox = obj.getBoundBox();
      auto modelMatrix = obj.getModelMatrix();

      float dx = bbox.x.max - bbox.x.min;
      float dy = bbox.y.max - bbox.y.min;
      float dz = bbox.z.max - bbox.z.min;

      bboxLocal = glm42::scale(bboxLocal, {dx, dy, dz, 1});

      m_shaderProgram->use();
      m_shaderProgram->setMatrix4d("uModel", modelMatrix);
      m_shaderProgram->setMatrix4d("projection", projection);
      m_shaderProgram->setMatrix4d("view", view);
      m_shaderProgram->setMatrix4d("uBboxScale", bboxLocal);
      m_shaderProgram->setFloat3d("uColor", {1.0, 0.0, 0.0});

      glBindVertexArray(m_VAO);
      glLineWidth(1.0f);
      glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, (void*)0);

      glBindVertexArray(0);
  }


private:
  float vertex_positions[24] = {
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,

    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
  };

  uint16_t indexes[24] = {
  0,1,  2,3,  4,5,  6,7,
  0,2,  1,3,  4,6,  5,7,
  0,4,  1,5,  2,6,  3,7
  };

  GLuint                           m_VAO = 0, m_VBO = 0, m_EBO = 0;
  std::unique_ptr< ShaderProgram > m_shaderProgram;
};