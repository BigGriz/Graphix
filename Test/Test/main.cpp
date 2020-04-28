// Library Includes
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <math.h>
// Dependency Includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <soil/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Local Includes
#include "ShaderLoader.h"
#include "Camera.h"
#include "Object.h"
#include "Terrain.h"

int width = 800, height = 600;
GLFWwindow* window = nullptr;
GLuint terrainprogram;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;


#pragma region CallbacksMethods

	glm::vec3 rayDirection;
	float mouseX, mouseY;
	bool firstMouse = true;
	bool camControl = false;
	float lastX = width/2, lastY = height/2;

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void processInput(GLFWwindow* window)
	{
		const float cameraSpeed = 2.5f * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyBoard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyBoard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyBoard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyBoard(RIGHT, deltaTime);
		}
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (camControl)
		{
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;
			lastX = xpos;
			lastY = ypos;

			mouseX = (2.0f * xpos) / (float)width - 1.0f;
			mouseY = 1.0f - (2.0f * ypos) / (float)height;

			camera.ProcessMouseMovement(xoffset, yoffset);
		}
		else
		{
			lastX = xpos;
			lastY = ypos;

			mouseX = (2.0f * xpos) / (float)width - 1.0f;
			mouseY = 1.0f - (2.0f * ypos) / (float)height;
		}
	}

	void scroll_callback(GLFWwindow* window, double _xoffset, double _yoffset)
	{
		camera.ProcessMouseScroll(_yoffset);
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			// Screen Pos
			glm::vec2 normalizedScreenPos = glm::vec2(mouseX, mouseY);

			// Screen to Proj Space
			glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);

			glm::mat4 proj = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);
			// Proj to Eye Space
			glm::mat4 invProjMat = glm::inverse(proj);
			glm::vec4 eyeCoords = invProjMat * clipCoords;
			eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

			// Eye to World Space
			glm::mat4 invViewMat = glm::inverse(camera.GetViewMatrix());
			glm::vec4 rayWorld = invViewMat * eyeCoords;
			rayDirection = glm::normalize(glm::vec3(rayWorld));
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			camControl = true;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			camControl = false;
		}
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_F && action == GLFW_PRESS)
		{

		}
	}

#pragma endregion CallbackMethods

#pragma region UpdateMethods

	void updateCamera()
	{
		glUseProgram(terrainprogram);
		// Update Camera
		glm::mat4 proj = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);
		unsigned int projLoc = glGetUniformLocation(terrainprogram, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		glm::mat4 view = camera.GetViewMatrix();
		unsigned int viewLoc = glGetUniformLocation(terrainprogram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	}

	void updateScreen()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//updateCamera();
	}

	void updateDeltaTime()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

#pragma endregion UpdateMethods

#pragma region terrainNoise
// Generic Noise Function
float random(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return (1.0f - double(t) * 0.931322574615478515625e-9);
}

// Generic Smooth Function
float smooth(int x, int y)
{
	float corners;
	float sides;
	float center;

	corners = (random(x - 1, y - 1) + random(x + 1, y - 1) + random(x - 1, y + 1) + random(x + 1, y + 1)) / 16;
	sides = (random(x - 1, y) + random(x + 1, y) + random(x, y - 1) + random(x, y + 1)) / 8;
	center = random(x, y) / 4;

	return (corners + sides + center);
}

// Interpolation of Noise
float interpolate(float a, float b, float x)
{
	return(a * (1 - x) + b * x);
}

// Perlin Noise
float linear_interpolate(float a, float b, float x)
{
	return (a * (1 - x) + b * x);
}

float cosine_interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * 0.5f;

	return (a * (1 - f) + b * f);
}

float cubic_interpolate(float v0, float v1, float v2, float v3, float x)
{
	float P = v3 - v2 - v0 - v1;
	float Q = v0 - v1 - P;
	float R = v2 - v0;
	float S = v1;

	return (P * powf(x, 3) + Q * powf(x, 2) + R * x + S);
}

float noise(float x, float y)
{
	float fractional_X = x - int(x);
	float fractional_Y = y - int(y);

	// Smooths
	float v1 = smooth(int(x), int(y));
	float v2 = smooth(int(x) + 1, int(y));
	float v3 = smooth(int(x), int(y) + 1);
	float v4 = smooth(int(x) + 1, int(y) + 1);

	// Interpolates
	float i1 = interpolate(v1, v2, fractional_X);
	float i2 = interpolate(v3, v4, fractional_X);

	return (interpolate(i1, i2, fractional_Y));
}

float totalNoisePerPoint(int x, int y)
{
	int octaves = 8;
	float zoom = 20.0f;
	float persistence = 0.5f;
	float total = 0.0f;

	for (int i = 0; i < octaves - 1; i++)
	{
		float frequency = pow(2, i) / zoom;
		float amplitude = pow(persistence,i);

		total += noise(x * frequency, y * frequency) * amplitude;
	}

	return (total);
}
#pragma endregion terrainNoise

void glSetup()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to init Window" << std::endl;
		glfwTerminate();
		system("PAUSE");
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	// hide mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		system("PAUSE");
	}
	
	glViewport(0, 0, width, height);

	// AntiAliasing
	glEnable(GL_MULTISAMPLE);

	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main()
{
	// Setup OpenGL Stuff
	glSetup();

	// Setup Programs
	terrainprogram = ShaderLoader::CreateProgram("Resources/Shaders/terrainvs.vs", "Resources/Shaders/terrainfs.fs");
	Terrain terrain("Resources/Textures/height.png", &terrainprogram);

	if (terrain.getHeight() != 0 && terrain.getWidth() != 0) {

		glm::mat4 view;
		glm::mat4 projection;


		projection = glm::perspective(camera.fov, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		// Game Loop
		while (!glfwWindowShouldClose(window))
		{
			updateDeltaTime();
			processInput(window);

			updateScreen();

			glm::mat4 view;
			view = camera.GetViewMatrix();
			glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
			glm::mat4 model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			glUseProgram(terrainprogram);
			terrain.setUniforms(model, view, projection);
			terrain.bindData();

			terrain.draw();



			// Poll Events & Swap Buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	else
	{
		std::cout << "terrain size = 0" << std::endl;
	}

	// End Program
	glfwTerminate();

	return(0);
}