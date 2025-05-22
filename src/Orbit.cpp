#include "Graphic/Orbit.hpp"
#include "Graphic/Shader.hpp"

Orbit::Orbit(Object3D * moon) : moon (moon) {

}

void Orbit::setMoon(Object3D * moon) {
    this->moon = moon;
}

void Orbit::draw(ShaderProgram & program) {
    updateGeometry();
    axis.draw(program);
    orbit.draw(program);
}

void Orbit::initGeometry() {
    
}

void Orbit::updateGeometry() {
    static float angle = 0;
    axis.setRotate({equator_angle, 0.0, greenwich_angle, 1.0});

    orbit.setScale({radius, radius, radius, 1.0});
    orbit.setRotate({equator_angle, 0.0, greenwich_angle, 1.0});

    axis.updateModelMatrix();
    orbit.updateModelMatrix();

    moon->setTranslate({0.0, 0.0, -radius,1.0});
    moon->setRotateMatrix(glm42::rotate(orbit.getRotationMatrix(), angle, glm42::vec3{0.0, 1.0, 0.0}));
    angle += (moon_speed / 10);
}