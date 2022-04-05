#include <iostream>
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "vertexbufferlayout.h"
#include "camera.h"

#include "shader.h"
#include "texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


void framebuffer_size_callback(GLFWwindow* win, int width, int height);
void processInput(GLFWwindow* win);
void mouseCallback(GLFWwindow* win, double xpos, double ypos);
void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);

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


// Compile + Link Shaders into Program
//static int checkCompileErrors(unsigned int type, unsigned int shader)
//{
//	int success;
//	char infoLog[1024];
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
//		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
//		glDeleteShader(shader);
//		return 0;
//	}
//}

//static unsigned int CompileShader(unsigned int type, const std::string& src)
//{
//	unsigned int id = glCreateShader(type);
//	const char* source = src.c_str();
//	glShaderSource(id, 1, &source, nullptr);
//	glCompileShader(id);
//
//	checkCompileErrors(type, id);
//
//	return id;
//}

//static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
//{
//	unsigned int program = glCreateProgram();
//	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//	// Attach files and link
//	glAttachShader(program, vs);
//	glAttachShader(program, fs);
//	glLinkProgram(program);
//	glValidateProgram(program);
//
//	//glDetachShader(vs);
//	//glDetachShader(fs);
//
//	glDeleteShader(vs);
//	glDeleteShader(fs);
//
//	return program;
//}

int main()
{
	//instantiate + configure GLFW window after including the properly linked header files
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	GLFWwindow* win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "learn_opengl", NULL, NULL);
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

	// Draw Rect with EBO

	float vertices[32] = {
		// positions		 // colors			// Texture coords
		-0.5f,  -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f
	};

	float vert[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
	};


	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	glm::vec3 transforms[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int indices[] = {
		0, 1, 2,	// 1st triangle
		 2, 3, 0		// 2nd triangle
	};

	unsigned int cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
	};


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	// Cube Object
	VertexArray	VA;
	VertexBuffer VB(vert, 36 * 8 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	VA.AddBuffer(VB, layout);

	// Cube Lightsource
	VertexArray lightVA;
	VertexBuffer lightVB(points, 36 * 3 * sizeof(float));
	VertexBufferLayout lightLayout;
	lightLayout.Push<float>(3);
	lightVA.AddBuffer(lightVB, lightLayout);

	// Plane
	VertexArray planeVA;
	VertexBuffer planeVB(planeVertices, sizeof(planeVertices));
	VertexBufferLayout planeLayout;
	planeLayout.Push<float>(3);
	planeLayout.Push<float>(2);
	planeVA.AddBuffer(planeVB, planeLayout);




	// MVP ---------------------------------------------------------------------------------
	// Cube Test: move model matrix to render loop to update every frame
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

	//glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -2.25f, 2.25f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(FOV), 8.0f / 4.5f, 0.1f, 100.0f);

	// --------------------------------------------------------------------------------------


	//Shader shader("res/shaders/basic.fxi");
	Shader shader("res/shaders/objectshader.fxi");
	shader.Bind();
	// Material ----------------------------------------------------------------------------
	//shader.SetUniform3fv("u_mat.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	//shader.SetUniform3fv("u_mat.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.SetUniform1f("u_mat.shininess", 32.0f);
	// Point Light Params ------------------------------------------------------------------
	glm::vec3 g_lightDiffuse(0.8f, 0.8f, 0.8f);
	glm::vec3 g_lightAmbient(0.1f, 0.1f, 0.1f);
	glm::vec3 g_lightSpecular(1.0f, 1.0f, 1.0f);
	glm::vec3 g_lightDirection(-0.2f, -1.0f, -0.3f);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	shader.SetUniform3fv("dirLight.specular", g_lightSpecular);
	shader.SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

	shader.SetUniform3fv("dirLight.direction", g_lightDirection);
	// -------------------------------------------------------------------------------------
	Texture texture("res/textures/container.png", "texture_diffuse");
	texture.Bind(0);
	shader.SetUniform1i("u_mat.diffuse", 0);
	Texture texture1("res/textures/container_specular.png", "texture_specular");
	texture1.Bind(1);
	//Texture texture2("");
	//texture2.Bind(2);
	shader.SetUniform1i("u_mat.specular", 1);
	VA.Unbind();
	VB.Unbind();
	lightVA.Unbind();
	lightVB.Unbind();
	//IB.Unbind();
	shader.Unbind();
	texture.Unbind();
	texture1.Unbind();

	// Light Shader -----------------------------------------------------------------------
	Shader lightShader("res/shaders/basic_light.fxi");
	lightShader.Bind();


	// Set Light Scene ---------------------------------------------------------------------


	// Plane Shader
	Shader planeShader("res/shaders/basic_light.fxi");

	// Matrix transform 
	//lightShader.SetUniformMatrix4fv("u_Pv", g_camera.GetViewProjectionMatrix());

	lightShader.Unbind();

	//ASSERT(location != -1);
	//glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);

	//glUniform1i(glGetUniformLocation(source.ID, "u_Texture"),0);
	Renderer renderer;

	// Simple Render Loop
	// ------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(win))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(win);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		processInput(win);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(win, mouseCallback);
		glfwSetScrollCallback(win, scrollCallback);

		// Gradually change color over time
		float bVal = (sin(currentFrame) / 2.0f) + 0.8f;

		//glUniform4f(vertexColorLocation, 0.0f, bVal, 0.0f, 1.0f);



		g_camera.ProcessKeyboard(win, deltaTime);
		g_camera.RecalculateViewMatrix();

		glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);
		glm::vec3 lightColor(0.0f, 0.0f, 1.0f);
		// Change light color experiment ----------------------------------------------
		//lightColor.x = sin(glfwGetTime() * 2.0f);
		//lightColor.y = sin(glfwGetTime() * 0.7f);
		//lightColor.z = sin(glfwGetTime() * 1.3f);
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				glm::mat4 light_model = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
				light_model = glm::scale(light_model, glm::vec3(0.2f));

				lightShader.Bind();
				lightShader.SetUniformMatrix4fv("u_Pv", g_camera.GetViewProjectionMatrix());
				lightShader.SetUniformMatrix4fv("model", light_model);
				lightShader.SetUniform3fv("u_lightColor", lightColor);
				renderer.Draw(lightVA, lightShader);
				lightShader.Unbind();
			}

		}

		{
			glm::mat4 plane_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
			planeShader.Bind();
			planeShader.SetUniformMatrix4fv("u_Pv", g_camera.GetViewProjectionMatrix());
			planeShader.SetUniformMatrix4fv("model", plane_model);
			planeShader.SetUniform3fv("u_lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
			renderer.Draw(planeVA, planeShader);
			planeShader.Unbind();


		}

		g_lightDiffuse = lightColor * glm::vec3(1.0f);
		g_lightAmbient = lightColor * glm::vec3(0.1f);
		{

			glm::vec3 objectColor(1.0f, 0.5f, 0.31f);

			shader.Bind();
			texture.Bind();
			texture1.Bind(1);
			const float radius = 10.0f;
			float camX = sin(currentFrame) * radius;
			float camZ = cos(currentFrame) * radius;
			// Directional
			shader.SetUniform3fv("dirLight.diffuse", g_lightDiffuse * 0.3f);
			shader.SetUniform3fv("dirLight.ambient", g_lightAmbient);
			shader.SetUniform3fv("u_viewPosition", g_camera.GetCameraPosition());
			// Point
			shader.SetUniform3fv("pointLight[0].position", pointLightPositions[0]);
			shader.SetUniform3fv("pointLight[0].diffuse", g_lightDiffuse);
			shader.SetUniform3fv("pointLight[0].ambient", g_lightAmbient);
			shader.SetUniform3fv("pointLight[0].specular", g_lightSpecular);
			shader.SetUniform1f("pointLight[0].constant", 1.0f);
			shader.SetUniform1f("pointLight[0].linear", 0.09f);
			shader.SetUniform1f("pointLight[0].quadratic", 0.032f);

			shader.SetUniform3fv("pointLight[1].position", pointLightPositions[1]);
			shader.SetUniform3fv("pointLight[1].diffuse", g_lightDiffuse);
			shader.SetUniform3fv("pointLight[1].ambient", g_lightAmbient);
			shader.SetUniform3fv("pointLight[1].specular", g_lightSpecular);
			shader.SetUniform1f("pointLight[1].constant", 1.0f);
			shader.SetUniform1f("pointLight[1].linear", 0.09f);
			shader.SetUniform1f("pointLight[1].quadratic", 0.032f);

			shader.SetUniform3fv("pointLight[2].position", pointLightPositions[2]);
			shader.SetUniform3fv("pointLight[2].diffuse", g_lightDiffuse);
			shader.SetUniform3fv("pointLight[2].ambient", g_lightAmbient);
			shader.SetUniform3fv("pointLight[2].specular", g_lightSpecular);
			shader.SetUniform1f("pointLight[2].constant", 1.0f);
			shader.SetUniform1f("pointLight[2].linear", 0.09f);
			shader.SetUniform1f("pointLight[2].quadratic", 0.032f);

			shader.SetUniform3fv("pointLight[3].position", pointLightPositions[3]);
			shader.SetUniform3fv("pointLight[3].diffuse", g_lightDiffuse);
			shader.SetUniform3fv("pointLight[3].ambient", g_lightAmbient);
			shader.SetUniform3fv("pointLight[3].specular", g_lightSpecular);
			shader.SetUniform1f("pointLight[3].constant", 1.0f);
			shader.SetUniform1f("pointLight[3].linear", 0.09f);
			shader.SetUniform1f("pointLight[3].quadratic", 0.032f);

			// Flashlight
			shader.SetUniform3fv("spotLight.position", g_camera.GetCameraPosition());
			shader.SetUniform3fv("spotLight.direction", g_camera.GetCameraFront());
			shader.SetUniform3fv("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			shader.SetUniform3fv("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			shader.SetUniform3fv("spotLight.specular", g_lightSpecular);
			shader.SetUniform1f("spotLight.constant", 1.0f);
			shader.SetUniform1f("spotLight.linear", 0.09f);
			shader.SetUniform1f("spotLight.quadratic", 0.032f);


			shader.SetUniform3fv("u_objectColor", objectColor);
			shader.SetUniform1f("u_MixValue", u_MixValue);
			// Flashlight -----------------------------------------------------------
			//shader.SetUniform3fv("u_light.position", g_camera.GetCameraPosition());
			//shader.SetUniform3fv("u_light.direction", g_camera.GetCameraFront());
			for (unsigned int i = 0; i < 10; i++)
			{

				// ----------------------------------------------------------------------
				shader.SetUniformMatrix4fv("u_Pv", g_camera.GetViewProjectionMatrix());
				glm::mat4 model = glm::translate(glm::mat4(1.0f), transforms[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shader.SetUniformMatrix4fv("u_Model", model);
				renderer.Draw(VA, shader);
			}
			shader.Unbind();

		}

		glfwPollEvents();
		glfwSwapBuffers(win);
	}

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
