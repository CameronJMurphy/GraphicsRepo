#include "Camera.h"

void Camera::SetPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	projectionTransform = glm::perspective(/*glm::pi<float>() **/ fieldOfView, aspectRatio, near, far);
}
void Camera::SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	viewTransform = glm::lookAt(from, to, up);
	worldTransform = glm::inverse(viewTransform);
}
void Camera::SetPosition(glm::vec3 position)
{
	worldTransform[3] = glm::vec4(position, 1.f);
	viewTransform = glm::inverse(worldTransform);
}
void Camera::SetView(glm::mat4 mat)
{
	worldTransform = glm::translate(GetPosition()) * mat;
	viewTransform = glm::inverse(worldTransform);
}

glm::mat4 Camera::GetWorldPosition()
{
	return worldTransform;
}
glm::mat4 Camera::GetView()
{
	return viewTransform;
}
glm::mat4 Camera::GetProjection()
{
	return projectionViewTransform;
}
void Camera::UpdateProjectionViewTransform()
{
	projectionViewTransform =  projectionTransform * viewTransform;
}

//void Camera::SetLookAt(glm::mat4 _worldTransform)
//{
//	viewTransform = glm::inverse(_worldTransform);
//}

glm::vec3 Camera::GetForward()
{
	auto& f = worldTransform[2];
	return glm::vec3(f.x, f.y, f.z);
}
glm::vec3 Camera::GetUp()
{
	auto& f = worldTransform[1];
	return glm::vec3(f.x, f.y, f.z);
}
glm::vec3 Camera::GetRight()
{
	auto& f = worldTransform[0];
	return glm::vec3(f.x, f.y, f.z);
}

glm::vec3 Camera::GetPosition()
{
	auto p = GetWorldPosition()[3];
	glm::vec3 retrieve(p[0], p[1], p[2]);
	return retrieve;
}