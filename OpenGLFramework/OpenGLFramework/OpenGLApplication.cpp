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
	//Shaders
	/*shader.loadShader(aie::eShaderStage::VERTEX, "shaders/simple.vert");
	shader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/simple.frag");*/
	shader.loadShader(aie::eShaderStage::VERTEX, "shaders/textured.vert");
	shader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/textured.frag");
	if (shader.link() == false)
	{
		printf("Shader Error: %s\n", shader.getLastError());
		return false;
	}
	if (gridTexture.load("meshes/characters/Pyro/Pyro_D.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}
	//if (bunnyMesh.load("meshes/stanford/Bunny.obj",
	//	true, true) == false) {
	//	printf("Soulspear Mesh Error!\n");
	//	return false;
	//}
	if (bunnyMesh.load("meshes/soulspear/soulspear.obj",
		true, true) == false) {
		printf("Soulspear Mesh Error!\n");
		return false;
	}
	
	// define 4 vertices for 2 triangles
	//Mesh::Vertex vertices[4];
	//vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	//vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	//vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	//vertices[3].position = { 0.5f, 0, -0.5f, 1 };
	//unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };
	//quadMesh.Initialise(4, vertices, 6, indices);

	quadMesh.IntialiseQuad();
	// make the quad 10 units wide
	quadTransform = { 10,0,0,0,
					  0,10,0,0,
					  0,0,10,0,
					  0,0,0,1 };
	//get current time
	newTime = glfwGetTime();
	//set up plane
	plane = vec4(0, 1, 0, -1);
	////

	//// load imaginary texture
	//aie::Texture texture1;
	//texture1.load("mytexture.png");
	//// create a 2x2 black-n-white checker texture
	//// RED simply means one colour channel, i.e. grayscale
	//aie::Texture texture2;
	//unsigned char texelData[4] = { 0, 255, 255, 0 };
	//texture2.create(2, 2, aie::Texture::RED, texelData);

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
	//do stuff, all game logic and drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	angle += 0.001f;

	return true;
};
void OpenGlApplication::Draw() {

	camera->SetLookAt(camera->GetWorldPosition()[3], camera_target, glm::vec3(0, 1, 0));
	camera->SetPerspective(zoom, 1280.f / 720.f, 0.1f, 1000.f);

	aie::Gizmos::clear();
	aie::Gizmos::addTransform(glm::translate(camera_target));


	//shader.bind();
	//auto pvm = camera->GetProjection() * quadTransform;
	//shader.bindUniform("ProjectionViewModel", pvm);
	//quadMesh.Draw();
	//aie::Gizmos::addSphere(vec3{ 0 }, 1.1, 12, 16, vec4{ 1.0f,1.0f, 0.2f, 1.0f });
	//auto earth_move = glm::translate(camera_target);
	//auto earth_rotate = glm::rotate(angle, vec3(0, 1, 0));
	//auto earth_spin = glm::rotate(-angle * 2.0f, vec3(0, 1, 0));

	//auto earth_final = earth_rotate * earth_move * earth_spin;
	//aie::Gizmos::addSphere(vec3{ 0 }, 0.3, 12, 16, vec4{ 0.0f,0.3f, 0.8f, 1.0f }, &earth_final);

	//const vec4 white(1);
	//const vec4 black(0, 0, 0, 1);

	//for (int i = 0; i < 21; ++i) {
	//	aie::Gizmos::addLine(vec3(-10 + i, 0, 10),
	//		vec3(-10 + i, 0, -10),
	//		i == 10 ? white : black);
	//	aie::Gizmos::addLine(vec3(10, 0, -10 + i),
	//		vec3(-10, 0, -10 + i),
	//		i == 10 ? white : black);
	//}

	////////
	vec4 planes[6];
	getFrustumPlanes(camera->GetProjection(), planes);

	shader.bind();

	auto pvm = camera->GetProjection() * quadTransform;
	shader.bindUniform("ProjectionViewModel", pvm);
	//shader.bindUniform("diffuseTexture", 0);
	gridTexture.bind(0);
	bunnyMesh.draw();

	//BoundingSphere sphere;
	//sphere.centre = vec3(0, cosf(glfwGetTime()) + 1, 0);
	//sphere.radius = 0.5f;
	//vec4 plane(0, 1, 0, -1);
	//if (InRenderView(planes, sphere))
	//{
	//	aie::Gizmos::addSphere(sphere.centre, sphere.radius,
	//		8, 8, vec4(1, 0, 1, 1));
	//}
	//vec4 planeColour(1, 1, 0, 1);
	//if (SideOfPlaneSphere(plane, sphere) == Side::InFront)
	//{
	//	planeColour = vec4(0, 1, 0, 1);
	//}
	//else if (SideOfPlaneSphere(plane, sphere) == Side::Behind)
	//{
	//	planeColour = vec4(1, 0, 0, 1);
	//}
	//aie::Gizmos::addTri(vec3(4, 1, 4), vec3(-4, 1, -4), vec3(-4, 1, 4),
	//	planeColour);
	//aie::Gizmos::addTri(vec3(4, 1, 4), vec3(4, 1, -4), vec3(-4, 1, -4),
	//	planeColour);



	camera->Update(deltaTime);
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