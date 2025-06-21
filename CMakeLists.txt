cmake_minimum_required(VERSION 3.28)
project(scop)

set(CMAKE_CXX_STANDARD 20)
#set(OpenGL_GL_PREFERENCE LEGACY)
set(APP_TARGET scop)

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glfw
#find_package(glfw3 REQUIRED)
#include_directories(${GLFW_INCLUDE_DIRS})
#link_libraries(${GLFW_LIBRARY_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})


include_directories(. include)

add_executable(${APP_TARGET}
        main.cpp

        src/Shader.cpp
        src/Texture.cpp
        src/Window.cpp
        src/Object3D.cpp
        src/Camera.cpp
        src/KeysControls.cpp
        src/mouseHandler.cpp
        src/TextureParser.cpp

        modules/Resources/GeometryKeeper.cpp
        modules/Resources/TextureKeeper.cpp

        include/objLoader.hpp
)

add_executable(
        math_tests

        src/math_test.cpp
        src/math_test.cpp
)


target_link_libraries(${APP_TARGET} ${GLEW_LIBRARIES} dl)


target_link_libraries(
        ${APP_TARGET}
        glfw
        ${OPENGL_LIBRARIES}
        ${EXTRA_LIBS}
)