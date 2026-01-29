#ifndef WINDOW_HPP
#define WINDOW_HPP

// GLEW
#include "Graphic/Object3D.hpp"
#include "Graphic/Shader.hpp"
#include "modules/Resources/GeometryKeeper.hpp"
#include <thread>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "WindowSettings.hpp"

#include <string>

class KeysControls;
class MouseControls;
class Camera; 

class Window {
public:
using NativeWindowType = GLFWwindow;
	Window();
	~Window();
	Window(int width, int height);

	void init();
	void initGLFW();
	void initGLEW();
	void initViewport();
	void initShaders();
	void initIO();

	void draw3DObjects();

	void drawLoop();
	void addObject3DToDraw(Object3D*);

	NativeWindowType* getNativeWindowPtr();

	GLFWwindow* glfwWindow = nullptr;
private:
	template<class T>
	using GeometryContainer = std::vector<T>;
	GeometryContainer<Object3D*> objects3d;

	ShaderProgram* shaderProgram = nullptr;
	KeysControls* keysControls = nullptr;
	MouseControls* mouseControls = nullptr;
	Camera* camera = nullptr;

	int width;
	int height;

	std::string name = "window";
	WindowSettings settings;
	glm42::mat4 projectionMatrix;
};

#endif
