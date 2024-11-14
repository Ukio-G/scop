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

#include "IO/UI.hpp"

#include "WindowSettings.hpp"

#include <string>

class KeysControls;
class MouseControls;
class Camera; 

class Window {
	friend class GLWindowModule;
public:
using NativeWindowType = GLFWwindow;
	Window();
	~Window();
	Window(int width, int height);

	void init();
	void initGLFW();
	void initUI();
	void initGLEW();
	void initViewport();
	void initShaders();
	void initIO();

	void passUniforms();
	void draw3DObjects();

	void startDrawing();
	void drawLoop();
	void drawInterface();
	void addObject3DToDraw(Object3D*);

	NativeWindowType* getNativeWindowPtr();

	GLFWwindow* glfwWindow;
private:
	template<class T>
	using GeometryContainer = std::vector<T>;
	GeometryContainer<Object3D*> objects3d;
	ShaderProgram* shaderProgram;
	ShaderProgram* lineShader;
	KeysControls* keysControls;
	MouseControls* mouseControls;
	UI* ui;
	Camera* camera;
	std::thread* renderThr;
	int width;
	int height;
	std::string name;
	WindowSettings settings;
	glm42::mat4 projectionMatrix;
};

#endif
