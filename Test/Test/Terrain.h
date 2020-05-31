/***
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2020 Media Design School

  File Name   :   Terrain.h
  Description :   Terrain Class Declaration
  Date		  :	  14/05/2020
  Author      :   Wayd Barton-Redgrave
  Mail        :   wayd.bar8374@mediadesign.school.nz
***/

#pragma once
// Library Includes
#include <iostream>
#include <vector>
// Dependency Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
// Local Includes
#include "Buffers.h"
#include "ShaderLoader.h"
#include "Camera.h"

// Class Declaration
class Terrain
{
	public:
		// Constructor & Destructor
		Terrain(const char* imagePath, GLuint* _program);
		~Terrain();

		// Create Buffers
		void createVertexData(const char* imagePath);
		void createIndexData();

		// Render Call
		void Render();

		// Vertices/Indices for Buffers
		std::vector<glm::vec3> vertices;
		std::vector<short> indexes;

		int width, height;
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
		GLuint* program;

		void createVertexData();
		float random(int x, int y, float _rand);
		float smooth(int x, int y);
		float interpolate(float a, float b, float x);
		float linear_interpolate(float a, float b, float x);
		float cosine_interpolate(float a, float b, float x);
		float cubic_interpolate(float v0, float v1, float v2, float v3, float x);
		float noise(float x, float y);
		float totalNoisePerPoint(int x, int y);
};

