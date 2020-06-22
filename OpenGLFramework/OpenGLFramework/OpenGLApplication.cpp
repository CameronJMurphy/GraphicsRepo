#include "inc/OpenGLApplication.h"


bool OpenGlApplication::Start() {
	//Initialise the GLFW library
	if (glfwInit() == GLFW_FALSE) {
		return false;
	}
	//Attempt to create a window
	window = glfwCreateWindow(1280, 720, "OpenGL Stuff", nullptr, nullptr);

	//If it fails, bail out
	if (window == nullptr) {
		glfwTerminate();
		return false;
	}

	//We have a valid window, make it current for receiving OpenGL stuff
	glfwMakeContextCurrent(window);

	//Try to load OpenGL, if not, bail out
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}
	//open GJ version
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	//Setup stuff
	glClearColor(0.45f, 0.3f, 0.45f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//Set up Gizmos
	aie::Gizmos::create(65535, 65535, 65535, 65535);

	//setup camera
	camera = new FlyCamera();
	camera->SetPosition(cameraPosition);
	camera->SetLookAt(camera->GetWorldPosition()[3], camera_target, glm::vec3(0, 1, 0));
	camera->SetSpeed(cameraSpeed);
	//Load Shaders
	LoadShaders();
	//Try and load shaders
	if (!LinkShaders())
	{
		return false; //end if failed
	}
	//Set meshes to their correct transforms
	SetMeshTransforms();

	//get current time
	newTime = glfwGetTime();
	//set up plane
	plane = vec4(0, 1, 0, -1);

	lights[0].diffuse = { 1, 1, 1 };
	lights[0].specular = { 0.5, 0.5, 0.5 };
	
	lights[1].diffuse = { 3, 3, 3 };
	lights[1].specular = { 1, 1, 1 };
	ambientLight = { 0.25f, 0.25f, 0.25f };

	return true;
};
bool OpenGlApplication::Update() {
	if (glfwWindowShouldClose(window) == GLFW_TRUE ||
		glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		return false;
	}
	oldTime = newTime;
	newTime = glfwGetTime();
	deltaTime = newTime - oldTime;
	// rotate light
	lights[0].position = glm::normalize(vec3(glm::cos(newTime * 2),
		glm::sin(newTime * 2), 0));
	lights[1].position = glm::normalize(vec3(glm::cos(newTime * 3),
		glm::sin(newTime * 3), 0));

	//make minotaur walk
	float movespeed = 10 * deltaTime;
	if (!reached) //if he hasnt reached his point keep walking
	{
		zPos = zPos + movespeed;
		if (zPos > -25)
			reached = true;
		minotaurTransform = { 5,0,0,0,
				   0,5,0,0,
				   0,0,5,0,
				   0,10,zPos,1 };
	}	
	else //else walk to the other point
	{
		zPos = zPos - movespeed;
		if (zPos < -50)
			reached = false;
		minotaurTransform = { 5,0,0,0,
				   0,5,0,0,
				   0,0,-5,0,
				   0,10,zPos,1 };
	}

	
	
	//do stuff, all game logic and drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	angle += 0.0001f;

	return true;
};
void OpenGlApplication::Draw() {

	//setup Camera
	camera->SetLookAt(camera->GetWorldPosition()[3], camera_target, glm::vec3(0, 1, 0));
	camera->SetPerspective(zoom, 1280.f / 720.f, 0.1f, 1000.f);

	//clear 
	aie::Gizmos::clear();
	aie::Gizmos::addTransform(glm::translate(camera_target));


	//set up bounding box
	vec4 planes[6];
	getFrustumPlanes(camera->GetProjection(), planes);

	//Bind our Shaders and textures
	BindShaders();

	//Update the camera
	camera->Update(deltaTime);

	//draw to screen
	aie::Gizmos::draw(camera->GetProjection());

	glfwSwapBuffers(window);
	glfwPollEvents();
};
void OpenGlApplication::Exit() {
	//shut everything down
	delete camera;
	aie::Gizmos::destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
};

//not used
Side OpenGlApplication::SideOfPlanePoint(glm::vec4 _plane, glm::vec3 point)
{
	float d = glm::dot(vec3(_plane), point) + _plane.w;
	if (d < 0)
	{
		printf("Back\n");
		return Side::Behind;
	}
	else if (d > 0)
	{
		printf("Front\n");
		return Side::InFront;
	}
	else
	{
		printf("On the plane\n");
		return Side::OnSurface;
	}
}
//not used
Side OpenGlApplication::SideOfPlaneSphere(glm::vec4 _plane, BoundingSphere _sphere)
{
	float d = glm::dot(vec3(_plane), _sphere.centre) + _plane.w;
	if (d < -_sphere.radius)
	{
		return Side::Behind;
	}
	else if (d > _sphere.radius)
	{
		return Side::InFront;
	}
	else
	{
		return Side::OnSurface;
	}
}
//not used
void OpenGlApplication::getFrustumPlanes(const glm::mat4& transform, glm::vec4* planes) {
	// right side
	planes[0] = vec4(transform[0][3] - transform[0][0],
		transform[1][3] - transform[1][0],
		transform[2][3] - transform[2][0],
		transform[3][3] - transform[3][0]);
	// left side
	planes[1] = vec4(transform[0][3] + transform[0][0],
		transform[1][3] + transform[1][0],
		transform[2][3] + transform[2][0],
		transform[3][3] + transform[3][0]);
	// top
	planes[2] = vec4(transform[0][3] - transform[0][1],
		transform[1][3] - transform[1][1],
		transform[2][3] - transform[2][1],
		transform[3][3] - transform[3][1]);
	// bottom
	planes[3] = vec4(transform[0][3] + transform[0][1],
		transform[1][3] + transform[1][1],
		transform[2][3] + transform[2][1],
		transform[3][3] + transform[3][1]);
	// far
	planes[4] = vec4(transform[0][3] - transform[0][2],
		transform[1][3] - transform[1][2],
		transform[2][3] - transform[2][2],
		transform[3][3] - transform[3][2]);
	// near
	planes[5] = vec4(transform[0][3] + transform[0][2],
		transform[1][3] + transform[1][2],
		transform[2][3] + transform[2][2],
		transform[3][3] + transform[3][2]);
	// plane normalisation, based on length of normal
	for (int i = 0; i < 6; i++) {
		float d = glm::length(vec3(planes[i]));
		planes[i] /= d;
	}
}
//not used
bool OpenGlApplication::InRenderView(vec4 planes[], BoundingSphere _sphere)
{
	for (int i = 0; i < 6; i++) {
		if (SideOfPlaneSphere(planes[i], _sphere) == Side::Behind)
		{
			printf("Behind, don't render it!\n");
			return false;
			break;
		}
		else if (SideOfPlaneSphere(planes[i], _sphere) == Side::OnSurface)
		{
			printf("Touching, we still need to render it!\n");
			return true;
		}
		else
		{
			printf("Front, fully visible so render it!\n");
			return true;
		}
	}
}

void OpenGlApplication::BindShaders()
{
	//bind spear shader
	spearShader.bind();
	spearShader.bindUniform("Ia", ambientLight);
	for (int i = 0; i < 2; ++i)//give to lights to the shader
	{
		spearShader.bindUniform("Id", lights[i].diffuse);
		spearShader.bindUniform("Is", lights[i].specular);
		spearShader.bindUniform("lightPosition", lights[i].position);
	}
	spearShader.bindUniform("specularPower", 25.f);
	spearShader.bindUniform("cameraPosition", camera->GetPosition());
	auto pvm = camera->GetProjection() * spearTransform;
	spearShader.bindUniform("ProjectionViewModel", pvm);
	spearShader.bindUniform("NormalMatrix", glm::inverse(glm::mat3(spearTransform)));
	spearMesh.draw();

	//bind minotaur shader
	minotaurShader.bind();
	minotaurShader.bindUniform("Ia", ambientLight);
	for (int i = 0; i < 2; ++i)//give to lights to the shader
	{
		minotaurShader.bindUniform("Id", lights[i].diffuse);
		minotaurShader.bindUniform("Is", lights[i].specular);
		minotaurShader.bindUniform("lightPosition", lights[i].position);
	}
	minotaurShader.bindUniform("specularPower", 25.f);
	minotaurShader.bindUniform("cameraPosition", camera->GetPosition());
	pvm = camera->GetProjection() * minotaurTransform;
	minotaurShader.bindUniform("ProjectionViewModel", pvm);
	minotaurShader.bindUniform("NormalMatrix", glm::inverse(glm::mat3(minotaurTransform)));
	minotaurMesh.draw();

	// bind grass shader
	grassShader.bind();
	pvm = camera->GetProjection() * grassTransform;
	grassShader.bindUniform("ProjectionViewModel", pvm);
	grassShader.bindUniform("diffuseTexture", 0);
	grassTexture.bind(0);
	grassMesh.Draw();

	//bind wall shader
	wallShader.bind();
	pvm = camera->GetProjection() * wallTransform;
	wallShader.bindUniform("ProjectionViewModel", pvm);
	wallShader.bindUniform("diffuseTexture", 0);
	wallTexture.bind(0);
	wallMesh.Draw();

	//bind titledGrass shader
	tiltedGrassShader.bind();
	pvm = camera->GetProjection() * tiltedGrassTransform;
	tiltedGrassShader.bindUniform("ProjectionViewModel", pvm);
	tiltedGrassShader.bindUniform("diffuseTexture", 0);
	tiltedGrassTexture.bind(0);
	tiltedGrassMesh.Draw();

	//bind longGrass shader
	longGrassShader.bind();
	pvm = camera->GetProjection() * longGrassTransform;
	longGrassShader.bindUniform("ProjectionViewModel", pvm);
	longGrassShader.bindUniform("diffuseTexture", 0);
	longGrassTexture.bind(0);
	longGrassMesh.Draw();
}

void OpenGlApplication::LoadShaders()
{
	//Shaders
	spearShader.loadShader(aie::eShaderStage::VERTEX, "shaders/normalmap.vert");
	spearShader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/normalmap.frag");
	minotaurShader.loadShader(aie::eShaderStage::VERTEX, "shaders/normalmap.vert");
	minotaurShader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/normalmap.frag");
	grassShader.loadShader(aie::eShaderStage::VERTEX, "shaders/textured.vert");
	grassShader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/textured.frag");
	wallShader.loadShader(aie::eShaderStage::VERTEX, "shaders/textured.vert");
	wallShader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/textured.frag");
	tiltedGrassShader.loadShader(aie::eShaderStage::VERTEX, "shaders/textured.vert");
	tiltedGrassShader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/textured.frag");
	longGrassShader.loadShader(aie::eShaderStage::VERTEX, "shaders/textured.vert");
	longGrassShader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/textured.frag");
}

bool OpenGlApplication::LinkShaders()
{
	if (tiltedGrassShader.link() == false)
	{
		printf("Shader Error: %s\n", tiltedGrassShader.getLastError());
		return false;
	}
	if (longGrassShader.link() == false)
	{
		printf("Shader Error: %s\n", longGrassShader.getLastError());
		return false;
	}
	if (spearShader.link() == false)
	{
		printf("Shader Error: %s\n", spearShader.getLastError());
		return false;
	}
	if (minotaurShader.link() == false)
	{
		printf("Shader Error: %s\n", spearShader.getLastError());
		return false;
	}
	if (grassShader.link() == false)
	{
		printf("Shader Error: %s\n", grassShader.getLastError());
		return false;
	}
	if (wallShader.link() == false)
	{
		printf("Shader Error: %s\n", wallShader.getLastError());
		return false;
	}
	if (spearMesh.load("meshes/soulspear/soulspear.obj",
		true, true) == false) {
		printf("Soulspear Mesh Error!\n");
		return false;
	}
	if (minotaurMesh.load("meshes/Minotaur/Minotaur_04.obj",
		true, true) == false) {
		printf("Minotaur Mesh Error!\n");
		return false;
	}
	if (!grassTexture.load("meshes/grass.jpg"))
	{
		printf("grass texture Error!\n");
	}
	if (!wallTexture.load("meshes/wall.jpg"))
	{
		printf("wall texture Error!\n");
	}
	if (!tiltedGrassTexture.load("meshes/grass.jpg"))
	{
		printf("tiltedGrass texture Error!\n");
	}
	if (!longGrassTexture.load("meshes/grass.jpg"))
	{
		printf("longGrass texture Error!\n");
	}
}

void OpenGlApplication::SetMeshTransforms()
{
	wallMesh.IntialiseQuad();
	wallTransform = {
					  1,0,0,0,
					  0,1,0,0,
					  0,1,0,0,
					  0,0,10,1
	};

	grassMesh.IntialiseQuad();
	grassTransform = {
					  1,0,0,0,
					  0,1,0,0,
					  0,0,1,0,
					  0,0,0,1
	};

	longGrassMesh.IntialiseQuad();
	longGrassTransform = {
					  1,0,0,0,
					  0,1,0,0,
					  0,0,2,0,
					  0,10,-40,1
	};
	tiltedGrassMesh.IntialiseQuad();
	tiltedGrassTransform = {
						  1,0,0,0,
						  0,1,0,0,
						  0,-1,1,0,
						  0,0,-10,1
	};
	spearTransform = { 1,0,0,0,
					  0,1,0,0,
					  0,0,1,0,
					  0,0,0,1 };

	minotaurTransform = { 5,0,0,0,
					   0,5,0,0,
					   0,0,5,0,
					   0,10,-50,1 };
}