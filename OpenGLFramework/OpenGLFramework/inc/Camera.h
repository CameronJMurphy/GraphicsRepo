#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
private:
	glm::mat4 worldTransform;
	glm::mat4 viewTransform;
	glm::mat4 projectionTransform;
	glm::mat4 projectionViewTransform;
public:
	virtual void Update(float deltaTime) = 0;
	void SetPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	/*void SetLookAt(glm::mat4 _worldTransform);*/
	void SetPosition(glm::vec3 position);
	void SetView(glm::mat4 mat);
	glm::mat4 GetWorldPosition();
	glm::vec3 GetPosition();
	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();
protected:
	void UpdateProjectionViewTransform();
};

