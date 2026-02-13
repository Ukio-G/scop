#ifndef SHADERSCONTROLS_HPP
#define SHADERSCONTROLS_HPP

#include "Camera.hpp"
#include "Graphic/Object3D.hpp"
#include "Graphic/Shader.hpp"
#include <memory>

struct TweenFloat {
  float &value;
  float from = 0.0f;
  float to = 1.0f;
  float duration = 0.6f;
  float elapsed = 0;
  bool  active = false;

  void start( float to_, float dur ) {
    from     = value;
    to       = to_;
    duration = std::max( 1e-6f, dur );
    elapsed  = 0;
    active   = true;
  }

  bool update( float dt ) {
    if( !active )
      return false;
    elapsed += dt;

    float t = std::min( elapsed / duration, 1.0f );

    value   = from + ( to - from ) * t;
    if( t >= 1.0f )
      active = false;
    return true;
  }
};


class ShadersControls {
  public:
  ShadersControls() = delete;
  ShadersControls(ShaderProgram* program) : m_program(program) { }
  ~ShadersControls() {
    glDeleteBuffers(1, &UBO);
  }

  void initControls();
  void update(Camera& camera, glm42::mat4& projectionMatrix);
  void initUBO();
  void initLineShaders();
  void useLineShaders();
  void useObjectsShader();

  ShaderProgram* getLineProgram() const { return m_lineProgram.get(); }
  ShaderProgram* getObjectsProgram() const { return m_program; }

  GLuint UBO = 0;
  private:

  struct alignas(16) FrameData
  {
     glm42::mat4   projection;
     glm42::mat4   view;
     glm42::mat4   transform;
     glm42::vec3   viewPos;

     float padding;

     float  textureColorLerpFactor;
     int    flatShading;
     int    grayscale;
  } frameData{};

  TweenFloat factor { .value = frameData.textureColorLerpFactor };
  bool m_texturesEnabled = true;

  ShaderProgram* m_program;
  std::unique_ptr< ShaderProgram > m_lineProgram;
};

#endif
