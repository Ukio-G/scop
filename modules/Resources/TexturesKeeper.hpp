#ifndef __TEXTURESKEEPER__H__
#define __TEXTURESKEEPER__H__

#include "Graphic/Texture.hpp"
#include "Graphic/TexturesPack.hpp"
#include "modules/Resources/DescriptionObject3D.hpp"
#include <string>
#include <unordered_map>

class TexturesKeeper {
public:
  using Ptr = std::shared_ptr<TexturesKeeper>;
  TexturesKeeper() = default;
  std::unordered_map<std::string, TexturesPack> textures;

  void loadTextureFromFile(const std::string & name, const std::string & path, ETextureType type);
  void loadTexturesFromFile(const DescriptionObject3D & descriptionObject, const std::string & basedir);
  void loadMaterialFromFile(const std::string & name, const std::string & path);
};

#endif //!__TEXTURESKEEPER__H__