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

// Class Implementation
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
		void Render(Camera* _cam);

		// Vertices/Indices for Buffers
		std::vector<glm::vec3> vertices;
		std::vector<short> indexes;

		int width, height;
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
		GLuint* program;
};

