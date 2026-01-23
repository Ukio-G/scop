#ifndef SCOP_DEBUG3DLINE_HPP
#define SCOP_DEBUG3DLINE_HPP
#include <memory>


#include "GL/glew.h"
#include "Graphic/Shader.hpp"
#include "cfg_parser.hpp"
#include "math.hpp"

class Debug3DLine
{
public:
  Debug3DLine( const glm42::vec3& begin, const glm42::vec3& end, const glm42::vec3& color = glm42::vec3(1.0f, 0.0f, 0.0f) )
      : m_begin( begin )
      , m_end( end )
      , m_vertexes{ begin[0], begin[1], begin[2] , end[0], end[1], end[2] }
      , m_color(color)
  {
    init();
  }

  void init()
  {
    glGenVertexArrays( 1, &m_VAO );
    glBindVertexArray( m_VAO );

    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( m_vertexes ), m_vertexes, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 3, ( void* )0 );

    glGenBuffers( 1, &m_EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( m_indexes ), m_indexes, GL_STATIC_DRAW );

    glBindVertexArray( 0 );

    auto& shaders = config_ptr->sections[ "shaders" ];

    Shader vs( std::get< std::string >( shaders[ "line_vs" ] ).c_str(), GL_VERTEX_SHADER );
    Shader fs( std::get< std::string >( shaders[ "line_fs" ] ).c_str(), GL_FRAGMENT_SHADER );

    m_shaderProgram = std::make_unique< ShaderProgram >( &vs, &fs );
  }


  void draw( const glm42::mat4& view, const glm42::mat4& projection )
  {
    m_shaderProgram->use();

    m_shaderProgram->setMatrix4d("projection", projection);
    m_shaderProgram->setMatrix4d("view", view);
    m_shaderProgram->setFloat3d("uColor", m_color);

    glBindVertexArray( m_VAO );
    glLineWidth(5.0f);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
  }

private:
  GLuint m_VAO;
  GLuint m_EBO;
  GLuint m_VBO;

  std::unique_ptr< ShaderProgram > m_shaderProgram;

  glm42::vec3 m_begin;
  glm42::vec3 m_end;

  glm42::vec3 m_color;
  float    m_vertexes[ 2 * 3 ];
  uint16_t m_indexes[ 2 ] = { 0, 1 };
};
#endif