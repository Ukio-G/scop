#include "Graphic/TextureParameters.hpp"
#include <cstring>


#include <GL/gl.h>

#include "Graphic/Texture.hpp"
#include <stdexcept>
#include <iostream>

Texture::Texture(const std::string &path) : path(path) { init(); }

Texture::Texture(const std::string &path, const TextureParameters &parameters)
    : path(path), parameters(parameters) {
  init();
}

Texture::Texture(Texture &&other) {
  *this = std::move(other);
}

Texture::Texture(const Texture &other) {
  *this = other;
}

Texture::~Texture() { }

Texture &Texture::operator=(const Texture &other) {
  if (this == &other)
    return *this;
  path.clear();

  parameters = other.parameters;
  width = other.width;
  height = other.height;
  numChannels = other.numChannels;
  textureSize = other.textureSize;

  if (!other.data)
  	return *this;
  
  data = new unsigned char[textureSize];
  memcpy(data, other.data, textureSize);
  getGLTexture();
  
  return *this;
}

Texture &Texture::operator=(Texture &&other) {
  path = std::move(other.path);
  parameters = other.parameters;
  width = other.width;
  height = other.height;
  numChannels = other.numChannels;
  id = other.id;
  data = other.data;
  textureSize = other.textureSize;

  other.data = nullptr;
  other.path.clear();
  other.width = 0;
  other.height = 0;
  other.id = 0;
  other.numChannels = 0;
  other.textureSize = 0;

  return *this;
}

void Texture::init() {
//  stbi_set_flip_vertically_on_load(true);
//  data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

  if (!data)
    throw std::runtime_error("Failed to load texture: " + path);
  
  textureSize = width * height * numChannels;
  std::cout << "Loaded texture from: \"" << path << "\", size: " << textureSize;
  std::cout << ", channels: " << numChannels << std::endl;

  getGLTexture();
}

void Texture::getGLTexture() {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  parameters.apply();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + (numChannels == 4), width, height, 0, GL_RGB + (numChannels == 4),
               GL_UNSIGNED_BYTE, data);

//  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(int type) {
  glActiveTexture(GL_TEXTURE0 + type);
  glBindTexture(GL_TEXTURE_2D, id);
}