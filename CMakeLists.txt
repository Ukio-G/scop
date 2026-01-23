cmake_minimum_required(VERSION 3.20)
project(scop)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "")


# Явно задаем sysroot, чтобы clang/scan-deps находили системные заголовки (time.h и др.)
set(MACOS_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")
if(APPLE AND EXISTS "${MACOS_SYSROOT}")
    set(CMAKE_OSX_SYSROOT "${MACOS_SYSROOT}" CACHE PATH "")
endif()

# Подавляем ворнинги об устаревшем OpenGL в Xcode/Clang
add_compile_definitions(GL_SILENCE_DEPRECATION)

set(CMAKE_C_COMPILER /opt/homebrew/opt/llvm@20/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm@20/bin/clang++)

# find_package(OpenGL REQUIRED)
# find_package(GLEW   REQUIRED)
# find_package(glfw3  REQUIRED)

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
    # GLEW::GLEW        # загружаем расширения OpenGL
    # glfw              # кроссплатформенное окно/ввод
    # OpenGL::GL        # системный фреймворк OpenGL
)


target_include_directories(${APP_TARGET} PRIVATE deps/include)
target_link_directories(${APP_TARGET} PRIVATE deps/lib)


# target_include_directories(${APP_TARGET} PRIVATE /opt/homebrew/opt/llvm@19/include)
# target_link_directories(${APP_TARGET} PRIVATE /opt/homebrew/opt/llvm@19/lib)

target_link_libraries(math_tests
    PRIVATE
    # GLEW::GLEW        # загружаем расширения OpenGL
    # glfw              # кроссплатформенное окно/ввод
    # OpenGL::GL        # системный фреймворк OpenGL
)

target_include_directories(${APP_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${APP_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/)
target_include_directories(math_tests  PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(math_tests  PRIVATE ${CMAKE_SOURCE_DIR}/)

# Явно прокидываем sysroot в компиляцию/линковку для clang-scan-deps
if(APPLE AND EXISTS "${MACOS_SYSROOT}")
    target_compile_options(${APP_TARGET} PRIVATE -isysroot ${MACOS_SYSROOT})
    target_link_options(${APP_TARGET} PRIVATE -isysroot ${MACOS_SYSROOT})
    target_compile_options(math_tests  PRIVATE -isysroot ${MACOS_SYSROOT})
    target_link_options(math_tests  PRIVATE -isysroot ${MACOS_SYSROOT})
endif()
