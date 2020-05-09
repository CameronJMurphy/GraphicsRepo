#include "FlyCamera.h"

void FlyCamera::Update(float deltaTime)
{
	//WASD movement controls
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W))
	{
		//SetPosition(glm::vec3(0,0, (GetWorldPosition()[3].z + speed) * deltaTime));
		auto p = GetPosition() + GetForward() * -speed * deltaTime;
		SetPosition(p);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A))
	{
		auto p = GetPosition() + GetRight() * -speed * deltaTime;
		SetPosition(p);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S))
	{
		auto p = GetPosition() + GetForward() * speed * deltaTime;
		SetPosition(p);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D))
	{
		auto p = GetPosition() + GetRight() * speed * deltaTime;
		SetPosition(p);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q))
	{
		auto p = GetPosition() + GetUp() * speed * deltaTime;
		SetPosition(p);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E))
	{
		auto p = GetPosition() + GetUp() * -speed * deltaTime;
		SetPosition(p);
	}
	////WASD target controls
	//if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP))
	//{
	//	glm::vec3 temp = glm::vec3(GetView()[3].x, GetView()[3].y + speed, GetView()[3].z);
	//	SetLookAt(GetWorldPosition()[3], temp, glm::vec3(0,1,0));
	//}
	//if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT))
	//{
	//	glm::vec3 temp = glm::vec3(GetView()[3].x + speed, GetView()[3].y, GetView()[3].z);
	//	SetLookAt(GetWorldPosition()[3], temp, glm::vec3(0, 1, 0));
	//}
	//if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT))
	//{
	//	glm::vec3 temp = glm::vec3(GetView()[3].x - speed, GetView()[3].y, GetView()[3].z);
	//	SetLookAt(GetWorldPosition()[3], temp, glm::vec3(0, 1, 0));
	//}
	//if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN))
	//{
	//	glm::vec3 temp = glm::vec3(GetView()[3].x, GetView()[3].y - speed, GetView()[3].z);
	//	SetLookAt(GetWorldPosition()[3], temp, glm::vec3(0, 1, 0));
	//}

	//need to add rotation using mouse
	double mx, my;
	glfwGetCursorPos(glfwGetCurrentContext(), &mx, &my);

	if (lastX == -1)
	{
		lastX = mx;
		lastY = my;
	}
	const double dx = mx - lastX;
	const double dy = my - lastY;

	lastX = mx;
	lastY = my;

	heading -= dx * 0.002f;
	pitch -= dy * 0.002f;

	pitch = glm::clamp(pitch, -glm::half_pi<double>(), glm::half_pi<double>());

	auto roty = glm::rotate((float)heading, glm::vec3(0, 1.0f, 0));
	auto rotz = glm::rotate((float)pitch, glm::vec3(1, 0, 0));
	SetView(roty * rotz);
	//update camera
	UpdateProjectionViewTransform();
}
void FlyCamera::SetSpeed(float _speed)
{
	speed = _speed;
}
