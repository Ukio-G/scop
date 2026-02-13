#include "include/IO/ShadersControls.hpp"
#include "Graphic/Object3D.hpp"
#include "Graphic/Shader.hpp"
#include "include/IO/KeysControls.hpp"
#include "cfg_parser.hpp"
#include "math.hpp"
#include <iostream>

void ShadersControls::initControls() {
    auto& eventChannel = EventChannel::getInstance();

    // Color interpolate-flat
    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_C, [this](Window *) {
      m_program->use();
      frameData.flatShading = !frameData.flatShading;
    }));

    // Texture-color
    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_T, [this](Window *) {
      m_texturesEnabled = !m_texturesEnabled;
      factor.start(m_texturesEnabled ? 0.0f : 1.0f, 0.7f);
    }));

    // GrayScale on-off
    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_G, [this](Window *) {
      m_program->use();
      frameData.grayscale = !frameData.grayscale;
    }));
}

void ShadersControls::initUBO()
{
  glGenBuffers(1, &UBO);

  // Uniform block in shader and User-defined binding point number
  GLuint blockIndex = glGetUniformBlockIndex(m_program->shaderProgram, "FrameData");
  if (blockIndex == GL_INVALID_INDEX)
    std::cerr << "ERROR: FrameData block not found in main program\n";
  else
    glUniformBlockBinding(m_program->shaderProgram, blockIndex, (GLuint)ShaderProgram::BindingPoint::FrameData);

  glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ShadersControls::FrameData), &(this->frameData), GL_DYNAMIC_DRAW);

  // User-defined binding point number and just generated buffer with glGenBuffers(...)
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)ShaderProgram::BindingPoint::FrameData, UBO);
  
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShadersControls::initLineShaders()
{
  auto& shaders = config_ptr->sections[ "shaders" ];

  Shader vs( std::get< std::string >( shaders[ "line_vs" ] ).c_str(), GL_VERTEX_SHADER );
  Shader fs( std::get< std::string >( shaders[ "line_fs" ] ).c_str(), GL_FRAGMENT_SHADER );

  m_lineProgram = std::make_unique< ShaderProgram >( &vs, &fs );

  GLuint blockIndex = glGetUniformBlockIndex(m_lineProgram->shaderProgram, "FrameData");
  if (blockIndex == GL_INVALID_INDEX)
    std::cerr << "ERROR: FrameData block not found in line program\n";
  else
    glUniformBlockBinding(m_lineProgram->shaderProgram, blockIndex, (GLuint)ShaderProgram::BindingPoint::FrameData);

}

void ShadersControls::useLineShaders()
{
  if (m_lineProgram)
    m_lineProgram->use();
}

void ShadersControls::useObjectsShader()
{
  if (m_program)
    m_program->use();
}

void ShadersControls::update(Camera& camera, glm42::mat4& projectionMatrix) {
  factor.update(1.f / 60.f);

  frameData.projection = projectionMatrix;
  frameData.view = camera.viewMatrix;
  frameData.viewPos = camera.position;
  glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShadersControls::FrameData), &(this->frameData));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
