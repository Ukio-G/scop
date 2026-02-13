#ifndef TEXTUREPARSER_H
#define TEXTUREPARSER_H

#include <memory>
#include "Graphic/Texture.hpp"
#include "modules/Resources/TexturesKeeper.hpp"

class TextureParser {
    public:
    TextureParser(TexturesKeeper* textureKeeper);
    ~TextureParser();

    // BMP
    void loadBMPFromFile(const std::string& fileName);

private:
    TexturesKeeper* textureKeeper;
};


#endif
