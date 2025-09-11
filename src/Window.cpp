// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Graphic/Object3D.hpp"
#include "IO/KeysControls.hpp"
#include "IO/ObjectsControls.hpp"
#include "IO/mouseHandler.hpp"
#include "Camera.hpp"
#include "math.hpp"
#include "modules/Resources/GeometryKeeper.hpp"
#include <stdexcept>
#include <thread>
#include "Graphic/RenderUtils/WireBBoxManager.hpp"

#include <GL/glew.h>

#include "IO/Window.hpp"
#include "FPSCounter.hpp"
#include "include/TransfromBehaviour/TransfromBehaviour.hpp"
#include "include/IO/ShadersControls.hpp"

Window::Window() : width( 0 ) , height( 0 ) {
}

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

void Window::initGLEW() {
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to initialize GLEW");
}

void Window::initViewport() {
  int fbw, fbh;
  glfwGetFramebufferSize( glfwWindow, &fbw, &fbh );
  glViewport( 0, 0, fbw, fbh );
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
	shaderProgram->setFloat3d("viewPos", camera->position);
}

void Window::addObject3DToDraw(Object3D * object3d) {
	objects3d.push_back(object3d);
}

void Window::draw3DObjects() {
  shaderProgram->use();
	passUniforms();
	for (auto & object3D: objects3d ) {
		object3D->draw(*shaderProgram);
	}
}

void Window::drawLoop() {
  WireBBoxManager          wbox;
  TransformationBehaviours behaviours;
  ObjectsControls          objControls( this, objects3d, behaviours );

  wbox.init();
  glfwSetInputMode( glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
  glfwMakeContextCurrent( glfwWindow );
  glfwSwapInterval( 1 );
  objControls.initControls();

  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );

  float aspect_ratio = static_cast< float >( width ) / static_cast< float >( height );
  projectionMatrix   = glm42::perspective( 0.001f, 1000.0f, 45.0f, aspect_ratio );
  FpsCounter counter;

  while( !glfwWindowShouldClose( glfwWindow ) ) {
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    keysControls->pollingKeysEvent();
    mouseControls->pollingMouseEvents();
    camera->updateViewMatrix();

    behaviours.update();

    draw3DObjects();
    wbox.draw( camera->viewMatrix, projectionMatrix, **objControls.m_selectedObjectIt );

    counter.frame();

    glfwSwapBuffers( glfwWindow );
    glfwPollEvents();
  }

  std::cout << "Draw loop destroyed" << std::endl;
  EventChannel::getInstance().publish( "destroyApplication", true );
  glfwTerminate();
}

void Window::initShaders() {
	Shader vs("../resources/shaders/vs.glsl", GL_VERTEX_SHADER);
	Shader fs("../resources/shaders/fs.glsl", GL_FRAGMENT_SHADER);
	shaderProgram = new ShaderProgram(&vs, &fs);

  shaderProgram->use();
  shaderProgram->setInt("diffuseMap", 0);
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

	camera = new Camera({2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	camera->initMovements();
}

Window::NativeWindowType* Window::getNativeWindowPtr() {
	return glfwWindow;
}
 

void Window::init() {
	initGLFW();
	initGLEW();

	initViewport();
	initShaders();
	initIO();
}
