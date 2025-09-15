#ifndef OBJECTSCONTROLS_HPP
#define OBJECTSCONTROLS_HPP

#include <vector>
#include "Graphic/Object3D.hpp"
#include "TransformBehaviour/TransformBehaviour.hpp"
#include "include/IO/Window.hpp"

class ObjectsControls {
public:
  ObjectsControls(Window* owner, std::vector<Object3D*>& objects, TransformationBehaviours& behaviours) :
  m_owner(owner),
  m_objects3d(objects),
  m_behaviours(behaviours) {
    m_selectedObjectIt = m_objects3d.begin();
  }

  void initControls();

  std::vector<Object3D*>::iterator m_selectedObjectIt;

  private:
  Window* m_owner;
  std::vector<Object3D*>& m_objects3d;
  TransformationBehaviours& m_behaviours;
};

#endif