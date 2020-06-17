#pragma once

#include "Application.h"
#include <gl_core_4_4.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include "FlyCamera.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

#include <GLFW/glfw3.h>
#include <Gizmos.h>
#include <time.h>
#include <BoundingSphere.h>
#include "Mesh.h"
#include "Shader.h"
#include <Texture.h>;
#include <OBJMesh.h>
#include <tiny_obj_loader.h>
#include "RenderTarget.h"

enum Side
{
	InFront,
	Behind,
	OnSurface
};

class OpenGlApplication : public Application {
private:

	GLFWwindow* window;
	//set up camera
	vec3 cameraPosition{ 0,10,-10 };
	vec3 camera_target{ 0 };
	float zoom = 0.9f;
	//const float speed = 0.01f;
	float angle = 0.0f;
	FlyCamera* camera;
	const float cameraSpeed = 5.f;
	float deltaTime;
	float newTime;
	float oldTime;
	glm::vec4 plane;

	aie::ShaderProgram spearShader;
	aie::OBJMesh spearMesh;
	glm::mat4 spearTransform;
	aie::Texture gridTexture;

	aie::ShaderProgram grassShader;
	Mesh grassMesh;
	glm::mat4 grassTransform;
	aie::Texture grassTexture;
	
	aie::ShaderProgram tiltedGrassShader;
	Mesh tiltedGrassMesh;
	glm::mat4 tiltedGrassTransform;
	aie::Texture tiltedGrassTexture;

	aie::ShaderProgram longGrassShader;
	Mesh longGrassMesh;
	glm::mat4 longGrassTransform;
	aie::Texture longGrassTexture;

	aie::ShaderProgram wallShader;
	Mesh wallMesh;
	glm::mat4 wallTransform;
	aie::Texture wallTexture;

	aie::RenderTarget renderTarget;

	struct Light {
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	Light light;
	Light light2;
	glm::vec3 ambientLight;
public:
	bool Start();
	bool Update();
	void Draw();
	void Exit();
	Side SideOfPlanePoint(glm::vec4 _plane, glm::vec3 point);
	Side SideOfPlaneSphere(glm::vec4 _plane, BoundingSphere _sphere);
	void getFrustumPlanes(const glm::mat4& transform, glm::vec4* planes);
	bool InRenderView(vec4 planes[], BoundingSphere _sphere);

};