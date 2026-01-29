#ifndef SCOP_DEBUG3DLINE_HPP
#define SCOP_DEBUG3DLINE_HPP
#include "GL/glew.h"
#include "math.hpp"

class Debug3DLine
{
public:
  Debug3DLine( const glm42::vec3& begin, const glm42::vec3& end, const glm42::vec3& color = glm42::vec3(1.0f, 0.0f, 0.0f) )
      : m_begin( begin )
      , m_end( end )
      , m_vertexes{ begin[0], begin[1], begin[2], color[0], color[1], color[2],
                    end[0],   end[1],   end[2],   color[0], color[1], color[2] }
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers( 1, &m_EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( m_indexes ), m_indexes, GL_STATIC_DRAW );

    glBindVertexArray( 0 );
  }


  void draw() const
  {
    glBindVertexArray( m_VAO );
    glLineWidth(5.0f);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
  }

private:
  GLuint m_VAO = 0;
  GLuint m_EBO = 0;
  GLuint m_VBO = 0;

  glm42::vec3 m_begin;
  glm42::vec3 m_end;

  float    m_vertexes[ 12 ];
  uint16_t m_indexes[ 2 ] = { 0, 1 };
};
#endif
