#include "Graphic/Object3D.hpp"
#include "IO/KeysControls.hpp"
#include "IO/mouseHandler.hpp"
#include "Camera.hpp"
#include "modules/Resources/GeometryKeeper.hpp"
#include <stdexcept>
#include <thread>


#include <GL/glew.h>

#include "IO/Window.hpp"


Window::Window() : 
	height(0), 
	width(0), 
	glfwWindow(nullptr), 
	name("GLWindow"),
	renderThr(nullptr) { }

void Window::initGLFW() {
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	settings.apply();

	// Create a GLFWwindow object that we can use for GLFW's functions
	glfwWindow = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if (glfwWindow == nullptr)
		throw std::runtime_error("Failed to create GLFW glfwWindow");

	glfwMakeContextCurrent(glfwWindow);
}

void Window::initUI() {

}

void Window::initGLEW() {
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to initialize GLEW");
}

void Window::initViewport() {
	glViewport(0, 0, width, height);
}

Window::Window(int width, int height) : width(width), height(height), name("Window") {
	init();
}

void Window::startDrawing() {
	glfwMakeContextCurrent(NULL);
	renderThr = new std::thread(&Window::drawLoop, this);
}

void Window::passUniforms() {
	shaderProgram->setMatrix4d("projection", projectionMatrix);
	shaderProgram->setMatrix4d("view", camera->viewMatrix);
	shaderProgram->setFloat3d("lightPos", {30.0, 5.0, 0.0});
	shaderProgram->setFloat3d("viewPos", camera->position);
	shaderProgram->setFloat3d("lightColor", {1.0, 1.0, 1.0});
}

void Window::addObject3DToDraw(Object3D * object3d) {
	objects3d.push_back(object3d);
}

void Window::draw3DObjects() {
	passUniforms();
	for (auto & object3D: objects3d ) {
		object3D->draw(*shaderProgram);
	}
}

void Window::drawInterface() {
	ui->draw();
}

void Window::drawLoop() {
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(glfwWindow);

	ui = new UI();
	ui->init(glfwWindow);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	projectionMatrix = glm42::perspective(45.0f, 800.f/800.f, 0.001f, 1000.0f);
	while (!glfwWindowShouldClose(glfwWindow)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		drawInterface();

		keysControls->pollingKeysEvent();
		mouseControls->pollingMouseEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		draw3DObjects();
		passUniforms();

		if (ui->orbit)
			ui->orbit->draw(*lineShader);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(glfwWindow);
	}

	std::cout << "Draw loop destroyed" << std::endl;
	glfwTerminate();
    EventChannel::getInstance().publish("destroyApplication", true);

}

void Window::initShaders() {
	Shader vs("../resources/shaders/vs.glsl", GL_VERTEX_SHADER);
	Shader fs("../resources/shaders/fs.glsl", GL_FRAGMENT_SHADER);
	shaderProgram = new ShaderProgram(&vs, &fs);


	Shader line_vs("../resources/shaders/line_vs.glsl", GL_VERTEX_SHADER);
	Shader line_fs("../resources/shaders/line_fs.glsl", GL_FRAGMENT_SHADER);
	lineShader = new ShaderProgram(&line_vs, &line_fs);
}

Window::~Window() {
	renderThr->join();
	
	delete renderThr;
	glfwDestroyWindow(glfwWindow);

	delete shaderProgram;
	delete keysControls;
	delete mouseControls;
	delete camera;
}

void Window::initIO() {
	keysControls = new KeysControls(*this);
	mouseControls = new MouseControls(*this);

	camera = new Camera({0.0f, 0.0f, 0.0f}, {-270.0f, 0.0f, 0.0f});
	camera->initMovements();
}

Window::NativeWindowType* Window::getNativeWindowPtr() {
	return glfwWindow;
}
 

void Window::init() {
	initGLFW();
	initGLEW();

	initViewport();
	initUI();
	initShaders();
	initIO();
}
