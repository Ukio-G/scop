#ifndef __TEXTURESPACK__H__
#define __TEXTURESPACK__H__

#include "Graphic/Texture.hpp"
#include <cstddef>
#include <memory>
#include <optional>
#include <array>

enum ETextureType {
    ETT_UNDEFINED = -1, // Service value
    
    ETT_DIFFUSE = 0,
    ETT_SPECULAR = 1,
    ETT_NORMAL = 2,

    ETT_MAX = 3, // Service value
};

struct TexturesPack {
  using Ptr = std::shared_ptr<TexturesPack>;
  
  Texture* diffuse() {
    if (!textures[ETT_DIFFUSE])
      return nullptr;

    return &(*textures[ETT_DIFFUSE]);
  }

  Texture* specular() {
    if (!textures[ETT_SPECULAR])
      return nullptr;

    return &(*textures[ETT_SPECULAR]);
  }
  
  Texture* normal() {
    if (!textures[ETT_NORMAL])
      return nullptr;
    return &(*textures[ETT_NORMAL]);
  }
  
  std::array<std::optional<Texture>, 3> textures;
};

#endif  //!__TEXTURESPACK__H__
