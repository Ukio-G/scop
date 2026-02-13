// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "EventChannel.hpp"
#include "Graphic/Object3D.hpp"
#include "IO/KeysControls.hpp"
#include "IO/ObjectsControls.hpp"
#include "IO/mouseHandler.hpp"
#include "Camera.hpp"
#include "cfg_parser.hpp"
#include "math.hpp"
#include "modules/Resources/GeometryKeeper.hpp"
#include <stdexcept>
#include <thread>
#include "Graphic/RenderUtils/WireBBoxManager.hpp"

#include <GL/glew.h>

#include "IO/Window.hpp"
#include "FPSCounter.hpp"
#include "include/TransformBehaviour/TransformBehaviour.hpp"
#include "include/IO/ShadersControls.hpp"
#include "include/Debug3DLine.hpp"
#include "include/Subspace.hpp"


Window::Window() : width( 0 ) , height( 0 ) {
}

void Window::initGLFW() {
	// Init GLFW
  glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, false);
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

Window::Window(int width, int height) : width(width), height(height), name("Window") { }

void Window::addObject3DToDraw(Object3D * object3d) {
	objects3d.push_back(object3d);
}

void Window::drawLoop() {
  WireBBoxManager          wbox;
  TransformationBehaviours behaviours;
  ObjectsControls          objControls( this, objects3d, behaviours );
  ShadersControls          shadersControls ( shaderProgram );

  objControls.initControls();

  shadersControls.initControls();
  shadersControls.initUBO();
  shadersControls.initLineShaders();

  wbox.init(shadersControls.UBO);

  auto id = glm42::mat4::id();

  // glm42::mat4 ss_scaled = glm42::scale(id, glm42::vec3{1, 0.1, 0.1});
  glm42::mat4 ss_1 = glm42::translate(id, glm42::vec3{15, 0, 0});
  glm42::mat4 ss_2 = glm42::translate(id, glm42::vec3{-15, 0, 0});

  Subspace subspace(ss_1, glm42::vec3(0.f, 0.5f, 0.5f));
  Subspace subspace2(ss_2, glm42::vec3(0.2f, 0.7f, 0.9f));

  subspace.initUBO(shadersControls);
  subspace2.initUBO(shadersControls);

  glfwSetInputMode( glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
  glfwMakeContextCurrent( glfwWindow );
  glfwSwapInterval( 1 );

  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );

  float aspect_ratio = static_cast< float >( width ) / static_cast< float >( height );
  projectionMatrix   = glm42::perspective( 0.1f, 100.0f, 45.0f, aspect_ratio );

  FpsCounter counter;

  bool show_fps    = (config_ptr->sections.contains("render_loop")) ? std::get<bool> (config_ptr->sections["render_loop"]["show_fps"]): false;
  bool draw_bounds = (config_ptr->sections.contains("render_loop")) ? std::get<bool> (config_ptr->sections["render_loop"]["draw_bounds"]): false;

  for (auto& obj: objects3d)
  {
    obj->initUBO(*shaderProgram);
    subspace.addObj(obj);
    subspace2.addObj(obj);
  }

  while( !glfwWindowShouldClose( glfwWindow ) ) {
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    keysControls->pollingKeysEvent();
    mouseControls->pollingMouseEvents();
    camera->updateViewMatrix();

    shaderProgram->use();
    behaviours.update();

    shadersControls.update(*camera, projectionMatrix);
    
    if (!objects3d.empty() && draw_bounds)
      wbox.draw( **objControls.m_selectedObjectIt );

    if (show_fps)
      counter.frame();

    subspace.draw( shadersControls );
    wbox.draw(**objControls.m_selectedObjectIt);

    subspace2.draw( shadersControls );
    wbox.draw(**objControls.m_selectedObjectIt);

    glfwSwapBuffers( glfwWindow );
    glfwPollEvents();
  }

  std::cout << "Draw loop destroyed" << std::endl;
  EventChannel::getInstance().publish( "destroyApplication", true );
  glfwTerminate();
}

void Window::initShaders() {
  auto& shaders = config_ptr->sections["shaders"];
  std::string& vs_str = std::get< std::string > (shaders[ "main_vs" ]);
  std::string& fs_str = std::get< std::string > (shaders[ "main_fs" ]);
  Shader vs( vs_str.c_str(), GL_VERTEX_SHADER );
  Shader fs( fs_str.c_str(), GL_FRAGMENT_SHADER );

  shaderProgram = new ShaderProgram(&vs, &fs);

  shaderProgram->use();
  shaderProgram->setInt("diffuseMap", 0);
}

Window::~Window() {
	glfwDestroyWindow(glfwWindow);

	delete shaderProgram;
	delete keysControls;
	delete mouseControls;
	delete camera;
  for (auto obj_ptr : objects3d) {
    delete obj_ptr;
  }
}

void Window::initIO() {
	keysControls = new KeysControls(*this);
	mouseControls = new MouseControls(*this);

	camera = new Camera({2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	camera->initMovements();

  EventChannel::getInstance().publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_ESCAPE, [this](Window *) {
      glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
  }));

  keysControls->printControls();
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
