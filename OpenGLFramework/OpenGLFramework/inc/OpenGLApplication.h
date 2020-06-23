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

	////////SCENE OBJECTS////////
	aie::ShaderProgram spearShader;
	aie::OBJMesh spearMesh;
	glm::mat4 spearTransform;

	aie::ShaderProgram minotaurShader;
	aie::OBJMesh minotaurMesh;
	glm::mat4 minotaurTransform;

	aie::Texture gridTexture;

	////////GRASS////////
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

	////////WALLS////////
	aie::ShaderProgram wallShader;
	Mesh wallMesh;
	glm::mat4 wallTransform;
	aie::Texture wallTexture;

	aie::ShaderProgram wall1Shader;
	Mesh wall1Mesh;
	glm::mat4 wall1Transform;
	aie::Texture wall1Texture;

	aie::ShaderProgram wall2Shader;
	Mesh wall2Mesh;
	glm::mat4 wall2Transform;
	aie::Texture wall2Texture;

	aie::ShaderProgram wall3Shader;
	Mesh wall3Mesh;
	glm::mat4 wall3Transform;
	aie::Texture wall3Texture;

	aie::ShaderProgram wall4Shader;
	Mesh wall4Mesh;
	glm::mat4 wall4Transform;
	aie::Texture wall4Texture;

	aie::ShaderProgram wall5Shader;
	Mesh wall5Mesh;
	glm::mat4 wall5Transform;
	aie::Texture wall5Texture;

	aie::RenderTarget renderTarget;

	struct Light {
		glm::vec3 position;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	Light lights[2];
	glm::vec3 ambientLight;

	float zPos = -50; //used for the moving minotaur pos
	bool reached = false;
public:
	bool Start();
	bool Update();
	void Draw();
	void Exit();
	Side SideOfPlanePoint(glm::vec4 _plane, glm::vec3 point);
	Side SideOfPlaneSphere(glm::vec4 _plane, BoundingSphere _sphere);
	void getFrustumPlanes(const glm::mat4& transform, glm::vec4* planes);
	bool InRenderView(vec4 planes[], BoundingSphere _sphere);
	void BindShaders();
	void LoadShaders();
	bool LinkShaders();
	void SetMeshTransforms();
	bool LoadTextures();
};