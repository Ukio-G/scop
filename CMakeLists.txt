cmake_minimum_required(VERSION 3.20)
project(scop)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "")

find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)

set(APP_TARGET scop)

add_executable(${APP_TARGET}
    main.cpp
    src/Shader.cpp  src/Texture.cpp  src/Window.cpp
    src/Object3D.cpp src/Camera.cpp  src/KeysControls.cpp
    src/mouseHandler.cpp src/TextureParser.cpp
    modules/Resources/GeometryKeeper.cpp
    modules/Resources/TextureKeeper.cpp
    src/IO/ObjectsControls.cpp
    src/IO/ShadersControls.cpp   
)

add_executable(math_tests src/math_test.cpp)

target_link_libraries(${APP_TARGET}
    PRIVATE
    GLEW::GLEW
    glfw
    OpenGL::GL
)

target_include_directories(${APP_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${APP_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/)
target_include_directories(math_tests  PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(math_tests  PRIVATE ${CMAKE_SOURCE_DIR}/)
