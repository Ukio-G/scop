#ifndef TRANSFORM_BEHAVIOUR_HPP
#define TRANSFORM_BEHAVIOUR_HPP

#include <algorithm>
#include <map>
#include <utility>
#include <variant>
#include <vector>
#include "include/Graphic/Object3D.hpp"
#include "math.hpp"

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace Behaviours {

  struct Spin {
    Spin(glm42::vec3 tick_delta) : tick_delta(tick_delta) { }
    glm42::vec3 tick_delta = {} ;
    Spin& operator+=(const Spin& other) {
      this->tick_delta = this->tick_delta + other.tick_delta;
      return *this;
    }
  };

  struct Translation {
    Translation(glm42::vec3 tick_delta) : tick_delta(tick_delta) { }
    glm42::vec3 tick_delta = {} ;
    Translation& operator+=(const Translation& other) {
      this->tick_delta = this->tick_delta + other.tick_delta;
      return *this;
    }
  };
}


class TransformationBehaviours {
  public:
  using BehaviourVariant = std::variant<Behaviours::Spin, Behaviours::Translation>;
  using ObjectTransforms = std::vector<BehaviourVariant>;

  std::map<Object3D*, ObjectTransforms > m_objectsTransforms;

  inline void apply(Object3D* object, const Behaviours::Spin& spin) {
    auto rotate = object->getRotate();

    rotate[0] += spin.tick_delta[0];
    rotate[1] += spin.tick_delta[1];
    rotate[2] += spin.tick_delta[2];

    object->setRotate(rotate);
  }

  inline void apply(Object3D* object, const Behaviours::Translation& translation) {
    glm42::vec4 translate = object->getTranslate();

    translate[0] += translation.tick_delta[0];
    translate[1] += translation.tick_delta[1];
    translate[2] += translation.tick_delta[2];

    object->setTranslate(translate);
  }
public:

  inline void addBehaviourToObject(Object3D* object, BehaviourVariant behaviourVariant)
  {
    auto & objectTransforms = m_objectsTransforms[object];

    for( auto& transform : objectTransforms ) {
      if (transform.index() == behaviourVariant.index()) {
      std::visit( overloaded{
          [&](Behaviours::Spin & spin) { spin += std::get<Behaviours::Spin>(behaviourVariant); },
          [&](Behaviours::Translation & translation) { translation += std::get<Behaviours::Translation>(behaviourVariant); },
        }, transform );
        return;
      }
    }

    objectTransforms.push_back(behaviourVariant);
}

  inline void removeBehaviourFromObject(Object3D* object)
  {
    m_objectsTransforms.erase(object);
  }

  void update()
  {
    for( auto& [ object, transforms ] : m_objectsTransforms ) {
      for( auto& transform : transforms ) {
        std::visit( overloaded{
            [&](Behaviours::Spin & spin) { apply( object, spin ); },
            [&](Behaviours::Translation & translation) { apply( object, translation ); },
          }, transform );
      }
    }
  }
};

#endif