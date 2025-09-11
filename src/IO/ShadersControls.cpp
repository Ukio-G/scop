#include "include/IO/ShadersControls.hpp"
#include "include/IO/KeysControls.hpp"

void ShadersControls::initControls() {
    auto& eventChannel = EventChannel::getInstance();

    // Color interpolate-flat
    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_C, [this](Window * window) {
      m_program->use();
      m_program->setBool("u_flatShading", useFlatColor);
      useFlatColor = !useFlatColor;
    }));

    // Texture-color
    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_T, [this](Window * window) {
      // По нажатию на эту кнопку должен произойти плавный переход от цветов вершин к текстуре и наоборот
      factor.start(factor.value < 0.5f ? 1.0f : 0.0f, 0.7f);
    }));

    // GrayScale on-off
    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_G, [this](Window * window) {
      m_program->use();
      m_program->setBool("u_grayscale", useGrayscaleColors);
      useGrayscaleColors = !useGrayscaleColors;
    }));
}

void ShadersControls::update() {
  if (factor.update(1.f / 90.f)) {
    m_program->use();
    m_program->setFloat("u_textureColorLerpFactor", factor.value);
  }
}