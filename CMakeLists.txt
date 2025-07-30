cmake_minimum_required(VERSION 3.28)
project(scop)

set(CMAKE_CXX_STANDARD 20)

#–– macOS-специфика ––––––––––––––––––––––––––––––––––––––––––
# Минимальная версия системы (при необходимости понизьте)
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "")

# Подавляем ворнинги об устаревшем OpenGL в Xcode/Clang
add_compile_definitions(GL_SILENCE_DEPRECATION)

#–– Зависимости ––––––––––––––––––––––––––––––––––––––––––––––
find_package(OpenGL REQUIRED)          # даёт импорт-таргет OpenGL::GL
find_package(GLEW   REQUIRED)          # импорт-таргет GLEW::GLEW
find_package(glfw3  REQUIRED)          # импорт-таргет glfw

#–– Цели ––––––––––––––––––––––––––––––––––––––––––––––––––––––
set(APP_TARGET scop)

add_executable(${APP_TARGET}
    main.cpp
    src/Shader.cpp  src/Texture.cpp  src/Window.cpp
    src/Object3D.cpp src/Camera.cpp  src/KeysControls.cpp
    src/mouseHandler.cpp src/TextureParser.cpp
    modules/Resources/GeometryKeeper.cpp
    modules/Resources/TextureKeeper.cpp
)

add_executable(math_tests src/math_test.cpp)

#–– Линковка ––––––––––––––––––––––––––––––––––––––––––––––––––
target_link_libraries(${APP_TARGET}
    PRIVATE
    GLEW::GLEW        # загружаем расширения OpenGL
    glfw              # кроссплатформенное окно/ввод
    OpenGL::GL        # системный фреймворк OpenGL
)

target_link_libraries(math_tests
    PRIVATE
    GLEW::GLEW        # загружаем расширения OpenGL
    glfw              # кроссплатформенное окно/ввод
    OpenGL::GL        # системный фреймворк OpenGL
)
#–– Инклуды вашего проекта ––––––––––––––––––––––––––––––––––––
target_include_directories(${APP_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${APP_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/)
target_include_directories(math_tests  PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(math_tests  PRIVATE ${CMAKE_SOURCE_DIR}/)
