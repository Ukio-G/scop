#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "TextureParameters.hpp"
#include <memory>
#include <string>

class Texture {
public:
  Texture() = default;
  explicit Texture(const std::string &path);
  Texture(const std::string &path, const TextureParameters &parameters);
  Texture(Texture &&other ) noexcept;
  Texture(const Texture &other);
  ~Texture();
  Texture& operator=(const Texture &Texture);
  Texture& operator=(Texture &&Texture ) noexcept;

  void init(unsigned char* data);
  void bind(int type = 0);
  void getGLTexture(unsigned char* data);

  std::shared_ptr<unsigned int> id = nullptr;
  std::string path;
  TextureParameters parameters;
  int width = 0;
  int height = 0;
  int numChannels = 0;
  size_t textureSize = 0;
};

#endif
