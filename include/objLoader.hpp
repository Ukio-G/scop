#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "math.hpp"
#include <array>
#include <fstream>
#include <map>
#include <sstream>
#include <unordered_map>

namespace obj {
enum RecordType : size_t {
  Comment,
  VertexPos,
  VertexUV,
  VertexNormal,
  Face,
  Material,
  Smooth,
  Group,
  Object,
  Undefined
};

class Loader {
public:
  bool LoadFile(const std::string &string) {}

  static inline size_t skip_spaces(const std::string & line, size_t pos = 0) {
    while (line[pos] == ' ')
      pos++;
    return pos;
  }

  template <class T, size_t N>
  static inline auto read_n_values(const std::string & line, size_t pos) -> std::array<T, N> {
      std::array<T, N> result;

      std::istringstream stream(line);
      stream.ignore(static_cast<long>(pos));

      for (int i = 0; i < N; ++i) {
        stream >> result[i];
      }

      if (stream.fail())
        throw std::runtime_error("Fail to parse line: " + line);
      return result;
  }

  RecordType getLineType(const std::string &line) {
    // Skip leading spaces
    size_t pos = skip_spaces(line);

    if (line[pos] == '#')
      return RecordType::Comment;
    if (line[pos] == 'v')
      return RecordType::VertexPos;
    if (line[pos] == 'v' && line[pos + 1] == 't')
      return RecordType::VertexUV;
    if (line[pos] == 'v' && line[pos + 1] == 'n')
      return RecordType::VertexNormal;
    if (line[pos] == 'f')
      return RecordType::Face;
    if (line[pos] == 'm')
      return RecordType::Material;
    if (line[pos] == 's')
      return RecordType::Smooth;
    if (line[pos] == 'g')
      return RecordType::Group;
    if (line[pos] == 'o')
      return RecordType::Object;

    return Undefined;
  }

  template <size_t T> void parseLine(const std::string &line) {
    size_t pos = skip_spaces(line);

  }

  inline void parseVectorPos(const std::string &line) {
    size_t pos = skip_spaces(line);
    auto result = read_n_values<double, 3>(line, pos + 1);
    positions.emplace_back(result[0], result[1], result[2]);
  }

  inline void parseVectorUV(const std::string &line) {
        size_t pos = skip_spaces(line);
        auto result = read_n_values<double, 2>(line, pos + 2);
        texcoords.emplace_back(result[0], result[1]);
  }

  inline void parseVectorNormal(const std::string &line) {
        size_t pos = skip_spaces(line);
        auto result = read_n_values<double, 3>(line, pos + 2);
        normals.emplace_back(result[0], result[1], result[2]);
  }

  inline void parseFace(const std::string &line) {

  }

  inline void parseMaterial(const std::string &line) {

  }

  inline void parseSmooth(const std::string &line) {

  }

  inline void parseGroup(const std::string &line) {

  }

  inline void parseObject(const std::string &line) {

  }

  void LoadNextMesh(std::ifstream &file) {

    for (std::string line; std::getline(file, line);) {

      RecordType type = getLineType(line);
      if (type == RecordType::Comment)
        continue;

      if (type == RecordType::Undefined)
        throw std::runtime_error("Undefined record while parsing OBJ file");

      switch (type) {
      case RecordType::VertexPos:
        parseVectorPos(line);
        break;
      case RecordType::VertexUV:
        parseVectorUV(line);
        break;
      case RecordType::VertexNormal:
        parseVectorNormal(line);
        break;
      case RecordType::Face:
        parseFace(line);
        break;
      case RecordType::Material:
        parseMaterial(line);
        break;
      case RecordType::Smooth:
        parseSmooth(line);
        break;
      case RecordType::Group:
        parseGroup(line);
        break;
      case RecordType::Object:
        parseObject(line);
        break;
      }
    }
  }

public:
  std::unordered_map<std::string, geom::Mesh> meshes;


private:
  std::string currentMesh;

  // Cache for one mesh (Then object parsing finished compile all of this to polygons)
  std::vector<glm42::vec3> positions;
  std::vector<glm42::vec3> normals;
  std::vector<glm42::vec2> texcoords;

  
};

} // namespace obj

#endif // OBJLOADER_HPP
