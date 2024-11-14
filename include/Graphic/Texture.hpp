#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "TextureParameters.hpp"
#include <string>

class Texture {
public:
  Texture(const std::string &path);
  Texture(const std::string &path, const TextureParameters &parameters);
  Texture(Texture &&other);
  Texture(const Texture &other);
  ~Texture();
  Texture& operator=(const Texture &Texture);
  Texture& operator=(Texture &&Texture);

  void init();
  void bind(int type = 0);
  void getGLTexture();

  unsigned int id;
  std::string path;
  TextureParameters parameters;
  int width;
  int height;
  int numChannels;
  unsigned char *data = nullptr;
  size_t textureSize = 0;
};

#endif
