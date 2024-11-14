#include <functional>
#include <iostream>
#include "Camera.hpp"
#include "EventChannel.hpp"
#include "IO/MouseEvents.hpp"
#include "IO/Window.hpp"

Camera::Camera(const glm42::vec3 &pos, const glm42::vec3 &rotation): position(pos), rotation(rotation), movementActive(true) {
	up = {0.0, 1.0, 0.0};
	front = {0.0, 0.0, -1.0};
	this->right = glm42::cross(front, up);
	viewMatrix = glm42::lookAt(position, glm42::normalize(pos + front), glm42::normalize(up));
}

void Camera::update(double x, double y, double dx, double dy) {
		if (!movementActive)
			return;

		glm42::vec3 direction;
		double sens = 0.1;
		dx *= sens;
		dy *= sens;

		auto & yaw = rotation.x;
		auto & pitch = rotation.y;

		yaw   -= dx;
		pitch += dy;

		if(yaw > 89.0f)
			yaw = 89.0f - 360.f;
		if(pitch < -89.0f)
			pitch = -89.0f + 360.f;;

		direction.x = cos(glm42::radians(yaw)) * cos(glm42::radians(pitch));
		direction.y = sin(glm42::radians(pitch));
		direction.z = sin(glm42::radians(yaw)) * cos(glm42::radians(pitch));
		front = glm42::normalize(direction);

		viewMatrix = glm42::lookAt(position, position + front, up);
}

void Camera::initMovements() {
	using namespace std::placeholders;
	const float cameraSpeed = 0.2f;
	auto & eventChannel = EventChannel::getInstance();

	eventChannel.publish("NewKeyEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_W, [&, cameraSpeed](Window * window) {
		position += cameraSpeed * front;
		viewMatrix = glm42::lookAt(position, position + front, up);
	}));

	eventChannel.publish("NewKeyEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_S, [&, cameraSpeed](Window * window) {
		position -= cameraSpeed * front;
		viewMatrix = glm42::lookAt(position, position + front, up);
	}));

	eventChannel.publish("NewKeyEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_A, [&, cameraSpeed](Window * window) {
		this->position -= glm42::normalize(glm42::cross(front, up)) * cameraSpeed;
		viewMatrix = glm42::lookAt(position, position + front, up);
	}));

	eventChannel.publish("NewKeyEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_D, [&, cameraSpeed](Window * window) {
		this->position += glm42::normalize(glm42::cross(front, up)) * cameraSpeed;
		viewMatrix = glm42::lookAt(position, position + front, up);
	}));

	eventChannel.publish("NewKeyEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_LEFT_ALT, [&, cameraSpeed](Window * window) {
		movementActive = false;
		glfwSetInputMode(window->getNativeWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}));

	eventChannel.publish("NewKeyEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_RIGHT_ALT, [&, cameraSpeed](Window * window) {
		movementActive = true;
		glfwSetInputMode(window->getNativeWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}));

	eventChannel.publish("NewMouseEvent", 
	std::make_pair<EMouseEvent, std::any>(
		EME_MOVE, 
		static_cast<std::function<void(double,double,double,double)> >(std::bind(&Camera::update, this, _1, _2, _3, _4)))
	);
}


