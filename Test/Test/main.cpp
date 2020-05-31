/***
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2020 Media Design School

  File Name   :   main.cpp
  Description :   Main Implementation
  Date		  :	  14/05/2020
  Author      :   Wayd Barton-Redgrave
  Mail        :   wayd.bar8374@mediadesign.school.nz
***/

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
#include "Terrain.h"
#include "Object.h"
// Test Includes
#include "GeometryModel.h"
#include "TessModel.h"

// Window Dimensions
int width = 800, height = 600;
GLFWwindow* window = nullptr;

// Shader Programs
GLuint terrainprogram;
GLuint objprogram;

// Shader Tests
GeometryModel* geomModel;
GLuint starprogram;
TessModel* tessModel;
GLuint tesprogram;
// Moveable Object and Terrain
Object* object;
Terrain* terrain;
// Camera 
Camera camera(glm::vec3(44.39f, 6.8f, 45.7f), glm::vec3(-0.131, 0.98, -0.146));
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
		// Quit Window
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		// Cam Control
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
		// Object Control
		/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			if (object->x < 255)
			{
				object->x++;
			}
			object->position = terrain->vertices[object->x + object->z * 256];
			object->position.y = terrain->vertices[object->x + object->z * 256].y + 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			if (object->x > 0)
			{
				object->x--;
			}
			object->position = terrain->vertices[object->x + object->z * 256];
			object->position.y = terrain->vertices[object->x + object->z * 256].y + 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			if (object->z < 255)
			{
				object->z++;
			}
			object->position = terrain->vertices[object->x + object->z * 256];
			object->position.y = terrain->vertices[object->x + object->z * 256].y + 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			if (object->z > 0)
			{
				object->z--;
			}
			object->position = terrain->vertices[object->x + object->z * 256];
			object->position.y = terrain->vertices[object->x + object->z * 256].y + 0.1f;
		}*/
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			std::cout << "x: " << camera.pos.x << std::endl;
			std::cout << "y: " << camera.pos.y << std::endl;
			std::cout << "z: " << camera.pos.z << std::endl;
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
		/*glUseProgram(objprogram);
		// Update Camera
		glm::mat4 proj = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);
		unsigned int projLoc = glGetUniformLocation(objprogram, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		glm::mat4 view = camera.GetViewMatrix();
		unsigned int viewLoc = glGetUniformLocation(objprogram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));*/

		glUseProgram(terrainprogram);
		// Update Camera
		glm::mat4 proj = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);
		unsigned int projLoc = glGetUniformLocation(terrainprogram, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		glm::mat4 view = camera.GetViewMatrix();
		unsigned int viewLoc = glGetUniformLocation(terrainprogram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		/*glUseProgram(starprogram);
		// Update Camera
		projLoc = glGetUniformLocation(starprogram, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		viewLoc = glGetUniformLocation(starprogram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));*/
	}

	void updateScreen()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updateCamera();
	}

	void updateDeltaTime()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

#pragma endregion UpdateMethods

void glSetup()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
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

	// Polygon Mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Culling
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

int main()
{
	// Setup OpenGL Stuff
	glSetup();

	// Seed RNG
	srand((unsigned)time(0));
	// Heightmap Program + Object
	terrainprogram = ShaderLoader::CreateProgram("Resources/Shaders/terrainvs.vs", "Resources/Shaders/terrainfs.fs");
	terrain = new Terrain("Resources/Textures/height.png", &terrainprogram);
	//// Object Program + Object
	//objprogram = ShaderLoader::CreateProgram("Resources/Shaders/vertexshader.vs", "Resources/Shaders/fragshader.fs");	
	//object = new Object(&objprogram, "Resources/Textures/awesomeface.png", glm::vec3(terrain->vertices.at(0)), glm::vec3(0.01f, 0.01f, 0.01f));
	//// Geo Program + Star
	//starprogram = ShaderLoader::CreateProgram("Resources/Shaders/startestvertex.vs", "Resources/Shaders/startestfrag.fs", "Resources/Shaders/geoshader.gs");
	//// Tess Program + Test
	//tesprogram = ShaderLoader::CreateProgram("Resources/Shaders/tessvertex.vs", "Resources/Shaders/tessfrag.fs", "Resources/Shaders/tessolationshader.tcs", "Resources/Shaders/quadpatch.tes");
	//
	//// Tests
	//geomModel = new GeometryModel(&starprogram, &camera);
	//geomModel->position = terrain->vertices.at(0);
	
	// Constructor crashed on generating buffer for VBO and I don't know why
	//tessModel = new TessModel(&tesprogram, &camera);

	// Game Loop
	while (!glfwWindowShouldClose(window))
	{
		updateDeltaTime();
		processInput(window);
		updateScreen();
		

		//object->Render();
		terrain->Render();

		//geomModel->Render();
		//tessModel->Render();

		// Poll Events & Swap Buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	

	// End Program
	glfwTerminate();

	return(0);
}