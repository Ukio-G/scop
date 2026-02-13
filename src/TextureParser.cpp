// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "TextureParser.hpp"

#include <filesystem>
#include <fstream>

inline size_t readInt(std::vector<char>& buffer, int offset, size_t bytes_to_read = 4) {
    size_t result = 0;

    for (size_t i = 0; i < bytes_to_read; i++) {
        result += static_cast<size_t>(buffer[i + offset]) << (i * 8);
    }

    return result;
}

TextureParser::TextureParser(TexturesKeeper* textureKeeper) : textureKeeper(textureKeeper)
{ }

TextureParser::~TextureParser() = default;

void TextureParser::loadBMPFromFile(const std::string &fileName) {
    int width, height;
    size_t pixel_data_idx = 0;

    // Open file
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << fileName << std::endl;
        return;
    }

    std::vector bytes((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
    if (bytes[0] != 'B' || bytes[1] != 'M') {
        std::cerr << "File is not BMP" << fileName << std::endl;
        return;
    }

    // Find pointer to begin image data
    pixel_data_idx = readInt(bytes, 10);
    std::cout << "pixel_data_idx: " << pixel_data_idx << std::endl;

    // Read header, Fill data from header
    size_t dib_header_bytes = readInt(bytes, 14);
    std::cout << "dib_header_bytes: " << dib_header_bytes << std::endl;

    width = readInt(bytes, 18);
    height = readInt(bytes, 22);

    size_t bpp = readInt(bytes, 28, 2);

    if (bpp != 32) {
        std::cerr << "Bpp is not 32 - unsupported BMP format" << std::endl;
        return;
    }

    // Allocate texture object
    std::string name = std::filesystem::path(fileName).stem();
    auto&t = textureKeeper->textures[name].textures[ETT_DIFFUSE];
    t = Texture();

    auto data = reinterpret_cast<unsigned char*>(bytes.data() + pixel_data_idx);
    t->height = height;
    t->width = width;
    t->numChannels = 4;
    t->textureSize = width * height * bpp;
    t->path = fileName;
    t->init(data);

    // Save texture to texture manager
}
