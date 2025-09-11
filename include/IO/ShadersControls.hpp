#ifndef SHADERSCONTROLS_HPP
#define SHADERSCONTROLS_HPP

#include "Graphic/Shader.hpp"

struct TweenFloat {
  float value = 0.0f;
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

  void initControls();
  void update();

  private:

  bool useFlatColor = true;
  bool useGrayscaleColors = true;

  TweenFloat factor { .value = 0.0f };

  ShaderProgram* m_program;
};

#endif
