#include "Graphic/Texture.hpp"
#include "TexturesKeeper.hpp"

void TexturesKeeper::loadTextureFromFile(const std::string & name, const std::string & path, ETextureType type) {
    textures[name].textures[type].emplace(path);
}

void TexturesKeeper::loadTexturesFromFile(const DescriptionObject3D & descriptionObject, const std::string & basedir) {
    if (descriptionObject.diffusePath) {
        loadTextureFromFile(descriptionObject.name, basedir + *descriptionObject.diffusePath, ETT_DIFFUSE);
    }

    if (descriptionObject.specularPath) {
        loadTextureFromFile(descriptionObject.name, basedir + *descriptionObject.specularPath, ETT_SPECULAR);
    }

    if (descriptionObject.normalPath) {
        loadTextureFromFile(descriptionObject.name, basedir + *descriptionObject.normalPath, ETT_NORMAL);
    }
}

void TexturesKeeper::loadMaterialFromFile(const std::string & name, const std::string & path) {
    /* Wait for implement */
}