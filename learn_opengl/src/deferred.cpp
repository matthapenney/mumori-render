#include <iostream>
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "vertexbufferlayout.h"
#include "framebuffer.h"
#include "camera.h"
#include "cubemap.h"
#include "gbuffer.h"
#include "model.h"

#include "shader.h"
#include "texture.h"
#include "equirectangularmap.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image/stb_image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


void framebuffer_size_callback(GLFWwindow* win, int width, int height);
void processInput(GLFWwindow* win);
void mouseCallback(GLFWwindow* win, double xpos, double ypos);
void mouse_botton_callback(GLFWwindow* win, int button, int action, double xpos, double ypos);
void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);
void renderQuad();
void renderSphere();


// Screen Settings
const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT = 1080;

float u_MixValue = 0.2f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float pitch = 89.0f;
float yaw = -90.0f;
float lastX = (SCREEN_WIDTH / 2.0f);
float lastY = (SCREEN_HEIGHT / 2.0f);
bool g_firstMouse = true;
float FOV = 75.0f;
Camera g_camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), FOV);

bool spawnModel = false;

bool g_isFlashOn = false;
bool g_disablePost = false;

bool notInitialized = false;
bool debugNormal = false;
bool debugAlbedo = false;
bool debugPosition = false;
bool debugMetallic = false;
bool debugRoughness = false;
bool debugAO = false;
bool DrawManyLights = false;

unsigned int isDebugEnabled = 0;

std::vector<std::string> faces
{
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/front.jpg",
		"res/textures/skybox/back.jpg"
};


int main()
{
	//instantiate + configure GLFW window after including the properly linked header files
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	GLFWwindow* win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "mumoriR", NULL, NULL);
	if (win == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
	glfwSwapInterval(1);

	//initialize GLAD before calling OpenGL functions since it manages function pointers (os-specific)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}


	int scrWidth, scrHeight;
	glfwGetFramebufferSize(win, &scrWidth, &scrHeight);
	// Set up vertex data with attributes
	float points[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};


	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f, 0.0f, 1.0f, 0.0f
	};

	float plane2Vertices[] =
	{
		// positions            // normals         // texcoords
		 5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,  5.0f,  0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 5.0f,

		 5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,  5.0f,  0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 5.0f,
		 5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,  5.0f, 5.0f
	};

	float qv[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	float LightVerticesTest[] =
	{
		-1.0f, -1.0f,  1.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,
	};


	glm::vec3 translate(0.0f);
	glm::vec3 rotate(0.0f);
	float rotation_X = 0.0f;
	float rotation_Y = 0.0f;
	float rotation_Z = 0.0f;

	glm::vec3 objtranslate(0.0f);
	float objrotation_X = 0.0f;
	float objrotation_Y = 0.0f;
	float objrotation_Z = 0.0f;

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};


	stbi_set_flip_vertically_on_load(true);

	// Depth Test
	glEnable(GL_DEPTH_TEST);

	// --------------------------------------------------------------------------------------
	// Buffer Declaration for Meshes
	// --------------------------------------------------------------------------------------
	//// Cube Object
	//VertexArray	cubeVA;
	//VertexBuffer VB(vert, 36 * 8 * sizeof(float));
	//VertexBufferLayout layout;
	//layout.Push<float>(3);
	//layout.Push<float>(2);
	//layout.Push<float>(3);
	//cubeVA.AddBuffer(VB, layout);


	// Cube Object Round 2
	VertexArray	cubeVA;
	VertexBuffer VB(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	cubeVA.AddBuffer(VB, layout);

	// Cube Lightsource
	VertexArray lightVA;
	VertexBuffer lightVB(points, 36 * 3 * sizeof(float));
	VertexBufferLayout lightLayout;
	lightLayout.Push<float>(3);
	lightVA.AddBuffer(lightVB, lightLayout);


	// Plane 
	VertexArray planeVA;
	VertexBuffer planeVB(plane2Vertices, sizeof(plane2Vertices));
	VertexBufferLayout planeLayout;
	planeLayout.Push<float>(3);
	planeLayout.Push<float>(3);
	planeLayout.Push<float>(2);
	planeVA.AddBuffer(planeVB, planeLayout);

	// Cubemap
	VertexArray skyboxVA;
	VertexBuffer skyboxVB(skyboxVertices, sizeof(skyboxVertices));
	VertexBufferLayout skyboxLayout;
	skyboxLayout.Push<float>(3);
	skyboxVA.AddBuffer(skyboxVB, skyboxLayout);

	// 2D Quad
	VertexArray quadVA;
	VertexBuffer quadVB(qv, sizeof(qv));
	VertexBufferLayout quadLayout;
	quadLayout.Push<float>(3);
	quadLayout.Push<float>(2);
	quadVA.AddBuffer(quadVB, quadLayout);


	Renderer renderer;
	// Point Light Params ------------------------------------------------------------------
	glm::vec3 g_lightDiffuse(0.8f, 0.8f, 0.8f);
	glm::vec3 g_lightAmbient(0.3f, 0.3f, 0.3f);
	glm::vec3 g_lightSpecular(1.0f, 1.0f, 1.0f);
	glm::vec3 g_lightDirection(-0.2f, -1.0f, -0.3f);

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // white
	glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);
	//glm::vec3(0.7f, 0.2f, 2.0f)
	glm::vec3 g_albedo(1.0f);
	glm::vec3 obj_albedo(0.95f);
	float intensity = 1.0f;
	float g = 0.8f;

	// -------------------------------------------------------------------------------------
	// Texture Creation
	// -------------------------------------------------------------------------------------

	Cubemap skybox;
	skybox.AddFace(faces);
	skybox.SetParams();

	// load PBR material textures
	Texture albedomap("res/textures/waterwornstone1_bl/waterwornstone1_Base_Color.png", "albedomap");
	Texture normalmap("res/textures/waterwornstone1_bl/waterwornstone1_Normal.png", "normalmap");
	Texture metallicmap("res/textures/waterwornstone1_bl/waterwornstone1_Metallic.png", "metallicmap");
	Texture roughnessmap("res/textures/waterwornstone1_bl/waterwornstone1_Roughness.png", "roughnessmap");
	Texture aomap("res/textures/waterwornstone1_bl/waterwornstone1_Ambient_Occlusion.png", "aomap");

	// --------------------------------------------------------------------------------------
	// Shader Creation 
	// --------------------------------------------------------------------------------------
	Shader geometry("res/shaders/gbuffer.fxi");
	Shader lightingPass("res/shaders/pbr.fxi");
	Shader skyboxShader("res/shaders/skybox.fxi");
	Shader lightShader("res/shaders/shaderSingleColor.fxi");
	Shader quadShader("res/shaders/empty.fxi");
	Shader equirectangularCubemap("res/shaders/equirectangularCubemap.fxi");
	Shader environmentCubemap("res/shaders/cubemapTest.fxi");
	Shader debugShader("res/shaders/debugShader.fxi");

	//Model dragon("res/objects/dragon/dragon.obj");
	//Model sponza("res/objects/sponza/sponza.obj");
	Model backpack("res/objects/backpack/backpack.obj");

	// Material ----------------------------------------------------------------------------
	float roughness = 0.0f;
	float metalness = 0.0f;
	float ao = 0.1f;


	// IBL - Precompute Environment Irradiance Map  (Equirectangular -> Cubemap -> Irradiance Map)
	// ----------------------------------------------------------------------------------------------
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	radianceImage hdrIceLake("res/textures/hdrCubemap/Ice_Lake/Ice_Lake_Env.hdr");
	

	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// convert HDR equirectangular environment map to cubemap equivalent-----------------------------------------------------------------

	equirectangularCubemap.Bind();
	equirectangularCubemap.SetUniform1i("equirectangularMap", 0);
	equirectangularCubemap.SetUniformMatrix4fv("model", glm::mat4(1.0f));
	equirectangularCubemap.SetUniformMatrix4fv("projection", captureProjection);
	hdrIceLake.setActive(0);

	glViewport(0, 0, 1024, 1024); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularCubemap.SetUniformMatrix4fv("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyboxVA.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}



	// --------------------------------------------------------------------------------------
	// Lighting Information
	// --------------------------------------------------------------------------------------
	const unsigned int N_LIGHTS = 32;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	srand(13);
	for (unsigned int i = 0; i < N_LIGHTS; i++)
	{
		// Calculate random offsets for N lights
		float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
		float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
		float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
		float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	// --------------------------------------------------------------------------------------
	// Gbuffer Config
	// --------------------------------------------------------------------------------------

	gBuffer gbuffer(scrWidth, scrHeight); //glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID) called on initialization
	gbuffer.isBufferComplete();
	gbuffer.Unbind();

	lightingPass.Bind();
	lightingPass.SetUniform1i("gPosition", 0);
	lightingPass.SetUniform1i("gNormal", 1);
	lightingPass.SetUniform1i("gAlbedoSpec", 2);
	lightingPass.SetUniform1i("gBrightColor", 3);
	lightingPass.SetUniform1i("gMetallic", 4);
	lightingPass.SetUniform1i("gRoughness", 5);
	lightingPass.SetUniform1i("gAO", 6);
	lightingPass.Unbind();
	
	//glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	//glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	//lightingPass.SetUniform1i("irradianceMap",3);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// Post Procesisng Frame Buffer --------------------------------------------------------
	//unsigned int PostProcessingBuffer1;
	//unsigned int fullscreenBuffers[2];
	//glGenFramebuffers(1, &PostProcessingBuffer1);
	//glBindFramebuffer(GL_FRAMEBUFFER, PostProcessingBuffer1);

	//glGenTextures(2, fullscreenBuffers);
	//for (unsigned int i = 0; i < 2; i++)
	//{
	//	glBindTexture(GL_TEXTURE_2D, fullscreenBuffers[i]);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fullscreenBuffers[i], 0);
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, PostProcessingBuffer1);
	//quadShader.Bind();
	//quadShader.SetUniform1f
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// IMGUI create init ------------------------------------------
	const char* glsl_version = "#version 430 core";
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Our state  [IMGUI]
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	

	glViewport(0, 0, scrWidth, scrHeight);

	// ------------------------------------------------------------------------------
	//  Render Loop
	// ------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(win))
	{

		// Test debug
		glViewport(0, 0, scrWidth, scrHeight);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//	- Input 
		processInput(win);
		g_camera.ProcessKeyboard(win, deltaTime);

		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(win, mouseCallback);
			glfwSetScrollCallback(win, scrollCallback);
		}
		g_camera.ProcessKeyboard(win, deltaTime);
		g_camera.RecalculateViewMatrix();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. Geometry pass: Render all geometric/color data to gbuffer -------------------------------------
		
		// For shadow mapping 
		//glClear(GL_DEPTH_BUFFER_BIT);
		//{
		//	float near_plane = 1.0f, far_plane = 7.5f;
		//	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//	glm::mat4 lightView = glm::lookAt(lightPosition,
		//										glm::vec3(0.0f, 0.0f, 0.0f),
		//										glm::vec3(0.0f, 1.0f, 0.0f));
		//	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		//}
	
		gbuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		normalmap.Bind(1);
		albedomap.Bind(2);
		metallicmap.Bind(4);	// Bind according to gbuffer slots
		roughnessmap.Bind(5);
		aomap.Bind(6);

		glm::mat4 model = glm::mat4(1.0f);
		{

			geometry.Bind();
			geometry.SetUniformMatrix4fv("PV", g_camera.GetViewProjectionMatrix());
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translate);
			if (rotation_X || rotation_Y || rotation_Z > -6.28f)
			{
				model = glm::rotate(model, rotation_X, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, rotation_Y, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, rotation_Z, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			geometry.SetUniformMatrix4fv("model", model);
			geometry.SetUniform1i("texture_diffuse1", 2);
			geometry.SetUniform1i("normal1", 1);
			geometry.SetUniform1i("metallic1", 4);
			geometry.SetUniform1i("roughness1", 5);
			geometry.SetUniform1i("AO1", 6);

			if (spawnModel)
			{
				backpack.Draw(geometry);
			}
			else
			{
				glm::mat4 model = glm::mat4(1.0f);
				for (int row = 0; row < nrRows; ++row)
				{
					for (int col = 0; col < nrColumns; ++col)
					{
						model = glm::mat4(1.0f);
						model = glm::translate(model, glm::vec3(
							(float)(col - (nrColumns / 2)) * spacing,
							(float)(row - (nrRows / 2)) * spacing,
							0.0f
						));
						geometry.SetUniformMatrix4fv("model", model);
						renderer.Draw(cubeVA, geometry);
						//renderSphere();
					}
				}
			}
		}

		gbuffer.Unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 2. Lighting pass: use g-buffer to calculate the scene's lighting
		lightingPass.Bind();
		gbuffer.activate(); // Set active textures for lighting sahder to use




		if (DrawManyLights)
		{
			for (unsigned int i = 0; i < lightPositions.size(); i++)
			{
				lightingPass.SetUniform3fv("light[" + std::to_string(i) + "].Position", lightPositions[i]);
				lightingPass.SetUniform3fv("light[" + std::to_string(i) + "].Color", lightColors[i]);
				// update attenuation parameters and calculate radius
				const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
				const float linear = 0.7f;
				const float quadratic = 1.8f;
				lightingPass.SetUniform1f("light[" + std::to_string(i) + "].Linear", linear);
				lightingPass.SetUniform1f("light[" + std::to_string(i) + "].Quadratic", quadratic);
				// then calculate radius of light volume/sphere
				const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
				float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
				lightingPass.SetUniform1f("light[" + std::to_string(i) + "].Radius", radius);
			}
			lightingPass.SetUniform3fv("viewPos", g_camera.GetCameraPosition());

			renderQuad();
			
		}
		else

		{
			float constant = 1.0;
			float linear = 0.7;
			float quadratic = 1.8;
			float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
			float radius =
				(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
				/ (2 * quadratic);

			// Bind Lighting Pass Shader * Set Uniforms
			lightingPass.SetUniform3fv("worldNormal", glm::vec3(0.0f, 1.0f, 0.0f));
			lightingPass.SetUniformMatrix4fv("P", g_camera.GetProjectionMatrix());
			lightingPass.SetUniformMatrix4fv("V", g_camera.GetViewMatrix());
			lightingPass.SetUniform3fv("viewPosition", g_camera.GetCameraPosition());
			lightingPass.SetUniform3fv("light.lightPosition", lightPosition);
			lightingPass.SetUniform3fv("light.lightColor", lightColor);
			lightingPass.SetUniform3fv("lightAmbient", g_lightAmbient);
			lightingPass.SetUniform1f("intensity", intensity);
			// Material Properties -< multipliers
			lightingPass.SetUniform3fv("albedoVal", g_albedo);
			lightingPass.SetUniform1f("roughnessVal", roughness);
			lightingPass.SetUniform1f("metalnessVal", metalness);
			lightingPass.SetUniform1f("aoVal", ao);
			renderQuad();
		}

		// 3. Forward Rendering

		gbuffer.readbuffer();
		gbuffer.writetodefault();
		glBlitFramebuffer(0, 0, scrWidth, scrHeight, 0, 0, scrWidth, scrHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		gbuffer.Unbind();
		

		 //Foward Render Light Representation
		{
			glm::mat4 light_model = glm::translate(glm::mat4(1.0f), lightPosition);
			light_model = glm::scale(light_model, glm::vec3(0.2f));
			lightShader.Bind();
			lightShader.SetUniformMatrix4fv("u_Pv", g_camera.GetViewProjectionMatrix());
			//lightShader.SetUniformMatrix4fv("u_V", g_camera.GetViewMatrix());
			//lightShader.SetUniformMatrix4fv("u_P", g_camera.GetProjectionMatrix());
			lightShader.SetUniformMatrix4fv("model", light_model);
			lightShader.SetUniform3fv("lightColor", lightColor);
			renderer.Draw(lightVA, lightShader);
			lightShader.Unbind();
		}

		{
			glDepthFunc(GL_LEQUAL);
			glm::mat4 skybox_view = glm::mat4(glm::mat3(g_camera.GetViewMatrix())); // remove translation from view matrix			
			skyboxShader.Bind();
			skybox.Bind(0);
			skyboxShader.SetUniformMatrix4fv("model", glm::mat4(1.0f));
			skyboxShader.SetUniformMatrix4fv("view", skybox_view);
			skyboxShader.SetUniformMatrix4fv("projection", g_camera.GetProjectionMatrix());
			skyboxVA.Bind();
			renderer.Draw(skyboxVA, skyboxShader);
			glDepthFunc(GL_LESS);
		}



		 // Test
		{
		glDepthFunc(GL_LEQUAL);
			glm::mat4 environmentMap_view = glm::mat4(g_camera.GetViewMatrix()); // remove translation from view matrix			
			environmentCubemap.Bind();
			skybox.PushUniforms(environmentCubemap, 0, environmentMap_view, g_camera.GetProjectionMatrix(), g_camera.GetCameraPosition(), lightPosition, lightColor, g);
			renderer.Draw(skyboxVA, environmentCubemap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		}

		// Debug Shader Test
		{

			if (!notInitialized)
			{
				// Initialize shader and vao w/ NDC vertex coords at top-right of screen
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				debugShader.Bind();
				if (debugNormal)
				{
					debugShader.SetUniform1i("fboAttachment", 1);
				}
				else if (debugAlbedo)
				{
					debugShader.SetUniform1i("fboAttachment", 2);
				}
				else if (debugMetallic)
				{
					debugShader.SetUniform1i("fboAttachment", 4);
				}
				else if (debugRoughness)
				{
					debugShader.SetUniform1i("fboAttachment", 5);
				}
				else if (debugAO)
				{
					debugShader.SetUniform1i("fboAttachment", 6);
				}
				else if (debugPosition)
				{
					debugShader.SetUniform1i("fboAttachment", 0);
				}
				renderQuad();
				debugShader.Unbind();
			}


		}
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		if (isDebugEnabled > 0)
		{
			{
				ImGui::Begin("IMGUI TEST");                          // Create a window called "Hello, world!" and append into it.

				ImGui::SliderFloat3("Translate Object", &translate.x, -10.0f, 100.0f);    // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat3("Light Position", &lightPosition.x, -10.0, 10.0f);
				ImGui::SliderFloat("Light Intensity", &intensity, 0.0f, 10.0f);
				ImGui::ColorEdit3("Light Color", (float*)&lightColor);
				ImGui::SliderAngle("Rotate X", &rotation_X);
				ImGui::SliderAngle("Rotate Y", &rotation_Y);
				ImGui::SliderAngle("Rotate Z", &rotation_Z);
				ImGui::ColorEdit3("Albedo Color", (float*)&g_albedo);// Edit 3 floats representing a color
				ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
				ImGui::SliderFloat("Metalness", &metalness, 0.0f, 1.0f);
				ImGui::SliderFloat("AO", &ao, 0.0f, 1.0f);
				ImGui::SliderFloat("Atmospheric Asymmetry", &g, -1.0f, 1.0f);


				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

		}
		
		// IMGUI Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(win, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// Be clean
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	// Be clean
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLboolean wireframe = GL_FALSE;
void processInput(GLFWwindow* win)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);
	if (glfwGetKey(win, GLFW_KEY_G) == GLFW_PRESS)
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS)
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		wireframe = GL_TRUE;
	}
	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS)
	{
		u_MixValue += .001f;
		if (u_MixValue >= 1.0f)
			u_MixValue = 1.0f;
	}
	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		u_MixValue -= .001f;
		if (u_MixValue <= 0.0f)
			u_MixValue = 0.0f;
	}
	if (glfwGetKey(win, GLFW_KEY_Y) == GLFW_PRESS)
	{
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_PRESS)
	{
		isDebugEnabled = 1;
	}
	if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_PRESS)
	{
		isDebugEnabled = 0;
	}
	if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS && isDebugEnabled == 1)
	{
		notInitialized = true;
		debugAlbedo = false;
		debugNormal = true;
		debugMetallic = false;
		debugRoughness = false;
		debugAO = false;
	}
	if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS && isDebugEnabled == 1)
	{
		notInitialized = true;
		debugAlbedo = true;
		debugNormal = false;
		debugMetallic = false;
		debugRoughness = false;
		debugAO = false;
	}
	if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS && isDebugEnabled == 1)
	{
		notInitialized = true;
		debugNormal = false;
		debugAlbedo = false;
		debugPosition = true;
		debugMetallic = false;
		debugRoughness = false;
		debugAO = false;
	}
	if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS && isDebugEnabled == 1)
	{
		notInitialized = true;
		debugNormal = false;
		debugAlbedo = false;
		debugPosition = false;
		debugMetallic = true;
		debugRoughness = false;
		debugAO = false;
	}
	if (glfwGetKey(win, GLFW_KEY_5) == GLFW_PRESS && isDebugEnabled == 1)
	{
		notInitialized = true;
		debugNormal = false;
		debugAlbedo = false;
		debugPosition = false;
		debugMetallic = false;
		debugRoughness = true;
		debugAO = false;
	}
	if (glfwGetKey(win, GLFW_KEY_6) == GLFW_PRESS && isDebugEnabled == 1)
	{
		notInitialized = true;
		debugNormal = false;
		debugAlbedo = false;
		debugPosition = false;
		debugMetallic = false;
		debugRoughness = false;
		debugAO = true;
	}
	if (glfwGetKey(win, GLFW_KEY_0) == GLFW_PRESS && isDebugEnabled == 1)
		notInitialized = false;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359f;
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}


void DisplayFramebuffer(unsigned int textureID)
{
	unsigned int vaoDebugTexturedRect = 0;
	if (!notInitialized)
	{
		// Initialize shader and vao w/ NDC vertex coords at top-right of screen
	}

	glActiveTexture(GL_TEXTURE0);
	//debugShader.Bind();
	//glUseProgram(shaderDisplayFBOOoutput);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(vaoDebugTexturedRect);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//debugShader.Unbind();
	glUseProgram(0);
}

void mouse_botton_callback(GLFWwindow* win, int button, int action, double xpos, double ypos)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(win, &xpos, &ypos);
		std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
	}

}

void mouseCallback(GLFWwindow* win, double xpos, double ypos)
{
	if (g_firstMouse)
	{
		lastX = -xpos;
		lastY = -ypos;
		g_firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Camera Rotation ----------------------------------------------------------------------
	glm::vec3 rotation(1.0f);
	rotation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	rotation.y = sin(glm::radians(pitch));
	rotation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	glm::vec3 cameraFront = glm::normalize(rotation);
	g_camera.SetRotation(cameraFront);
}

void scrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{
	FOV -= (float)yoffset;
	if (FOV < 1.0f)
		FOV = 1.0f;
	if (FOV > 75.0f)
		FOV = 75.0f;
	// Update fov value passed into camera.m_projectionMatrix;
}
