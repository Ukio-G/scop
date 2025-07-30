// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Graphic/TextureParameters.hpp"
#include <cstring>

#ifdef TARGET_OS_OSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Graphic/Texture.hpp"
#include <stdexcept>
#include <iostream>

Texture::Texture(const std::string &path) : path(path) { init(); }

Texture::Texture(const std::string &path, const TextureParameters &parameters)
    : path(path), parameters(parameters) {
  init();
}

Texture::Texture(Texture &&other ) noexcept {
  *this = std::move(other);
}

Texture::Texture(const Texture &other) {
  *this = other;
}

Texture::~Texture() = default;

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

Texture &Texture::operator=(Texture &&other ) noexcept
{
  if (this == &other)
    return *this;

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
  if (!data)
    throw std::runtime_error("Failed to load texture: " + path);
  
  textureSize = width * height * numChannels;
  std::cout << "Loaded texture from: \"" << path << "\", size: " << textureSize;
  std::cout << ", channels: " << numChannels << std::endl;

  getGLTexture();
}

void Texture::getGLTexture()
{
  glGenTextures( 1, &id );
  glBindTexture( GL_TEXTURE_2D, id );

  std::cout << "Texture ID: " << id << std::endl;
  parameters.apply();

  const GLint  internal = ( numChannels == 4 ) ? GL_RGBA8 : GL_RGB8;  // корректный internalFormat
  const GLenum external = ( numChannels == 4 ) ? GL_BGRA : GL_BGR;    // раскладка исходных байт


  glTexImage2D( GL_TEXTURE_2D, 0, internal, width, height, 0, external, GL_UNSIGNED_BYTE, data );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glGenerateMipmap( GL_TEXTURE_2D );

  glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture::bind(int type) {
  glActiveTexture(GL_TEXTURE0 + type);
  glBindTexture(GL_TEXTURE_2D, id);
}