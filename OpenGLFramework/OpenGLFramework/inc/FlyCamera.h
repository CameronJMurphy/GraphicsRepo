#pragma once
#include "Camera.h"
#include <GLFW/glfw3.h>

class FlyCamera : public Camera
{
private:
	float speed;
	glm::vec3 up;
	float lastX;
	float lastY;
	double heading;
	double pitch;
public:
	void Update(float deltaTime);
	void SetSpeed(float _speed);
};

