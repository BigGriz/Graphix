/***
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2020 Media Design School

  File Name   :   Buffers.h
  Description :   Vertex & Index Buffer Class Declarations & Implementation
  Date		  :	  14/05/2020
  Author      :   Wayd Barton-Redgrave
  Mail        :   wayd.bar8374@mediadesign.school.nz
***/
#pragma once

// Library Includes
#include <vector>
// Dependency Includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Class Implementation
class VertexBuffer
{
	public:
		// Constructor & Destructor
		VertexBuffer(std::vector<glm::vec3> vertices)
		{
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &bufferId);

			glBindBuffer(GL_ARRAY_BUFFER, bufferId);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
		}
		~VertexBuffer()
		{
			glDeleteBuffers(1, &bufferId);
		}

		void setVertexAttribPointer(int _offset, int _loc, int _num, int _stride)
		{
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, bufferId);
			glVertexAttribPointer(_loc, _num, GL_FLOAT, GL_FALSE, _stride * sizeof(float), (GLvoid*)_offset);
		}

		GLuint VAO;
		GLuint bufferId;
};

// Class Implementation
class IndexBuffer
{
	public:
		// Constructor & Destructor
		IndexBuffer(std::vector<short> indexes)
		{
			glGenBuffers(1, &bufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(short), &indexes.front(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		~IndexBuffer()
		{
			glDeleteBuffers(1, &bufferId);
		}

		GLuint bufferId;
};