#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "IO/Window.hpp"

class Camera {
public:
	Camera(const glm42::vec3 & pos, const glm42::vec3 &rotation);
	void initMovements();
	void update(double x, double y, double dx, double dy);

	glm42::vec3 position;
	glm42::vec3 rotation;

	glm42::vec3 front;
	glm42::vec3 up;
	glm42::vec3 right;

	glm42::mat4 viewMatrix;
	bool movementActive;
};
#endif
