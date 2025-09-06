#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "math.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <filesystem>

namespace obj {
enum RecordType : size_t {
  Comment,
  VertexPos,
  VertexUV,
  VertexNormal,
  Face,
  Material,
  MaterialLib,
  Smooth,
  Group,
  Object,
  EmptyLine,
  Undefined
};

class Loader {
public:
  void LoadFile(const std::string &string) {
    namespace fs = std::filesystem;

    std::ifstream file(string);
    fs::path p(string);

    currentMesh = p.stem();

    LoadNextMesh(file);

    recenterOriginToBoundBoxCenter(currentMesh);
  }

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

  inline std::vector<size_t> fanTriangulate(auto & face_indices) {
    std::vector<size_t> result;
    for (size_t i = 1; i + 1 < face_indices.size(); ++i) {
      result.push_back(face_indices[0]);
      result.push_back(face_indices[i]);
      result.push_back(face_indices[i + 1]);
    }
    return result;
  }

  RecordType getLineType(const std::string &line) {
    // Skip leading spaces
    size_t pos = skip_spaces(line);

    if (line[pos] == '#')
      return RecordType::Comment;
    if (line[pos] == 'v' && line[pos + 1] == 't')
      return RecordType::VertexUV;
    if (line[pos] == 'v' && line[pos + 1] == 'n')
      return RecordType::VertexNormal;
    if (line[pos] == 'v')
      return RecordType::VertexPos;
    if (line[pos] == 'f')
      return RecordType::Face;
    if (line[pos] == 'm' && line.find("mtllib") != std::string::npos)
      return RecordType::MaterialLib;
    if (line[pos] == 'u' && line.find("usemtl") != std::string::npos)
      return RecordType::Material;
    if (line[pos] == 's')
      return RecordType::Smooth;
    if (line[pos] == 'g')
      return RecordType::Group;
    if (line[pos] == 'o')
      return RecordType::Object;
    if (line[pos] == '\0')
      return RecordType::EmptyLine;

    return Undefined;
  }

  inline size_t insertVertexOrGetIndex(const geom::Vertex& vtx) {
    auto & vtxes = meshes[currentMesh].vertexes;
    auto it = std::find(vtxes.begin(), vtxes.end(), vtx);

    if (it != vtxes.end()) {
      return std::distance(vtxes.begin(), it);
    }

    // Need to insert
    vtxes.push_back(vtx);
    meshes[currentMesh].bbox << vtx;
    return vtxes.size() - 1;
  }

  inline geom::Vertex fromFaceFragment(const std::string & objFaceFragment) {
    auto first_delim = objFaceFragment.find('/');
    auto second_delim = objFaceFragment.find('/', first_delim + 1);
    geom::Vertex result;

    // v
    if ((first_delim == std::string::npos) && (second_delim == std::string::npos))
    {
      result.pos = positions[std::stoi(objFaceFragment) - 1];
    }

    // v/vt
    if ((first_delim != std::string::npos) && (second_delim == std::string::npos))
    {
      auto pos_str = objFaceFragment.substr(0, first_delim);
      auto uv_str = objFaceFragment.substr(first_delim, objFaceFragment.size() - 1);
      result.pos = positions[std::stoi(pos_str) - 1];
      result.uv = texcoords[std::stoi(uv_str) - 1];
    }

    if ((first_delim != std::string::npos) && (second_delim != std::string::npos))
    {
      // v//vn (general case)
      auto pos_str = objFaceFragment.substr(0, first_delim);
      auto normal_str = objFaceFragment.substr(second_delim + 1, objFaceFragment.size() - 1);
      result.pos = positions[std::stoi(pos_str) - 1];
      result.normal = normals[std::stoi(normal_str) - 1];

      // v/vt/vn
      if ((first_delim + 1) != (second_delim))
      {
        auto uv_str = objFaceFragment.substr(first_delim + 1, second_delim - 1);
        result.uv = texcoords[std::stoi(uv_str) - 1];
      }
    }


    return result;
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
    std::istringstream ss(line);
    std::string token;
    std::vector<size_t> indexes;

    ss >> token;

    while (ss >> token) {
      auto vtx = fromFaceFragment(token);
      auto idx = insertVertexOrGetIndex(vtx);
      indexes.push_back(idx);
    }

    auto & mesh_indexes = meshes[currentMesh].indexes;
    // Fan sub-triangulate
    if (indexes.size() > 3) {
      auto fan_triangulated_indexes = fanTriangulate(indexes);
      mesh_indexes.insert(mesh_indexes.end(), fan_triangulated_indexes.begin(), fan_triangulated_indexes.end());
    } else {
      mesh_indexes.insert(mesh_indexes.end(), indexes.begin(), indexes.end());
    }
  }

  inline void parseMaterial(const std::string &line) {

  }

  inline void parseSmooth(const std::string &line) {

  }

  inline void parseGroup(const std::string &line) {
    throw std::runtime_error("Not supported yet");
  }

  inline void parseObject(const std::string &line) {
    std::istringstream ss(line);
    std::string token;

    ss >> token;
    ss >> token;

    std::cout << "Parse object: " << token << std::endl;

    currentMesh = std::move(token);
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
      default:
          break;
      }
    }
  }

  void recenterOriginToBoundBoxCenter(const std::string & meshName) {
    auto & vtxs = meshes[meshName].vertexes;
    auto &bbox = meshes[meshName].bbox;
    for (auto &vtx : vtxs) {
      vtx.pos = vtx.pos - bbox.center();
    }
  }

  void printDebug() {

    for (auto &[name, mesh] : meshes) {
        std::cout << "Mesh: " << name << std::endl;

        std::cout << "Vertexes count:" << mesh.vertexes.size() << std::endl;
        std::cout << "Indexes count:" << mesh.indexes.size() << std::endl;
        std::cout << "Bound box: \n";
        std::cout << mesh.bbox;

        std::cout << "Vertexes: " << std::endl;
        for (auto & vtx : mesh.vertexes)
          std::cout << vtx << std::endl;

        std::cout << "Indexes: " << std::endl;
        for (int i = 0; i < mesh.indexes.size(); i+=3)
          std::cout << "( " << mesh.indexes[i] << ", " << mesh.indexes[i + 1]  << ", " << mesh.indexes[i + 2]  <<  " )" << std::endl;

    }
  }

public:
  std::unordered_map<std::string, geom::Mesh> meshes;


private:
  std::string currentMesh = "default";

  // Cache for one mesh (Then object parsing finished compile all of this to polygons)
  std::vector<glm42::vec3> positions;
  std::vector<glm42::vec3> normals;
  std::vector<glm42::vec2> texcoords;
};

} // namespace obj

#endif // OBJLOADER_HPP
