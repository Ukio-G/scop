#ifndef __DESCRIPTIONOBJECT3D__H__
#define __DESCRIPTIONOBJECT3D__H__

#include <optional>
#include <string>
#include <iostream>
#include "math.hpp"

struct DescriptionObject3D {
  std::string name;
  std::string modelPath;

  std::optional<std::string> diffusePath;
  std::optional<std::string> specularPath;
  std::optional<std::string> normalPath;
  std::optional<std::string> mtlPath;

  std::optional<glm42::vec3> rotate;
  std::optional<glm42::vec3> translate;
  std::optional<glm42::vec3> scale;
};

inline std::ostream& operator<<(std::ostream& out, const DescriptionObject3D& obj) {
  std::cout << "name: " << obj.name << std::endl;
  std::cout << "model: " << obj.modelPath << std::endl;

  if (obj.diffusePath)
    std::cout << "diffuse: " << *obj.diffusePath << std::endl;
  if (obj.specularPath)
    std::cout << "specular: " << *obj.specularPath << std::endl;
  if (obj.normalPath)
    std::cout << "normal: " << *obj.normalPath << std::endl;
  if (obj.mtlPath)
    std::cout << "material: " << *obj.mtlPath << std::endl;

  return out;
}

/*
using json = nlohmann::json;

inline void from_json(const nlohmann::json &j, DescriptionObject3D &obj) {
  j.at("name").get_to(obj.name);
  j.at("model").get_to(obj.modelPath);

  if (j.contains("diffuse")) {
    obj.diffusePath = j.at("diffuse");
  }

  if (j.contains("specular")) {
    obj.specularPath = j.at("specular");
  }

  if (j.contains("normal")) {
    obj.normalPath = j.at("normal");
  }

  if (j.contains("material")) {
    obj.mtlPath = j.at("material");
  }

  if (j.contains("translation")) {
    obj.translate = {j["translation"][0], j["translation"][1], j["translation"][2]};
  }

  if (j.contains("scale")) {
    obj.scale = {j["scale"][0], j["scale"][1], j["scale"][2]};
  }

  if (j.contains("rotate")) {
    obj.rotate = {j["rotate"][0], j["rotate"][1], j["rotate"][2]};
  }
}
*/

#endif //!__DESCRIPTIONOBJECT3D__H__