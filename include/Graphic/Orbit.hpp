#ifndef __ORBIT__H__
#define __ORBIT__H__

#include "BuffersCollection.hpp"
#include "Graphic/Object3D.hpp"
#include <memory>
#include "Line.hpp"
#include "LineStrip.hpp"

class Orbit {
    public:

    using Ptr = std::shared_ptr<Orbit>;

    Orbit(Object3D * moon);
    void setMoon(Object3D * moon);
    void draw(ShaderProgram & program);
    void initGeometry();
    void updateGeometry();
    
    glm42::vec3 rotationAxis = {0.0f, 1.0f, 0.0f};
    float greenwich_angle = 0.0f;
    float equator_angle = 0.0f;
    float radius = 10.0f;

    Object3D * moon = nullptr;
    float moon_speed = 0.0f;

    Line axis;
    LineStrip orbit;
};

#endif  //!__ORBIT__H__