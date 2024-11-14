#ifndef WINDOW_SETTINGS_HPP
#define WINDOW_SETTINGS_HPP

#include <GLFW/glfw3.h>

struct WindowSettings {
	int contextVersionMajor = 3;
	int contextVersionMinor = 3;

	int profile = GLFW_OPENGL_CORE_PROFILE;
	int resizable = GL_FALSE;

	void apply() {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, contextVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, contextVersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, resizable);
		glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);
	}
};

#endif
