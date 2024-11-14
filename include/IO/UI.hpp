#ifndef __UI__H__
#define __UI__H__

#include "Graphic/Object3D.hpp"
#include <cstddef>
#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>

//ImGui
#include "Graphic/Orbit.hpp"
#include <memory>

class UI {
    public:
    UI();
    void init(GLFWwindow* window);
    void draw();
    void setMoonOrbit(Orbit * orbit);

    std::shared_ptr<Orbit> orbit = nullptr;
    private:
    GLFWwindow* window = nullptr;
};


#endif  //!__UI__H__