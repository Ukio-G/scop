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

#endif //!__DESCRIPTIONOBJECT3D__H__