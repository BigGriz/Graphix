/***
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2020 Media Design School

  File Name   :   TessModel.h
  Description :   Tesselation Model Class Declaration & Implementation
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
#include "Camera.h"

// Class Declaration
class TessModel
{
	public:
		GLuint* program;
		Camera* camera;

		GLuint VAO;
		GLuint VBO;
		glm::mat4 model;
		glm::vec3 position = glm::vec3(6.0f, -2.0f, 0.0f);

		// Constructor & Destructor
		TessModel(GLuint* _program, Camera* _camera) 
		{		
			GLfloat points[] =
			{
				-1.0f, -1.0f, 0.0f,
				 1.0f, -1.0f, 0.0f,
				 1.0f,  1.0f, 0.0f,
				-1.0,   1.0,  0.0f
			};

			this->program = _program;
			this->camera = _camera;
		

			glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch

			glGenBuffers(1, &VBO); 

			glGenVertexArrays(1, &VAO); 

			glBindVertexArray(VAO); 
			glBindBuffer(GL_ARRAY_BUFFER, VBO); 
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0); 
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			glBindVertexArray(0);
		}
		~TessModel() {};

		// Rendering Functionality
		void Render() 
		{
			glUseProgram(*program);

			glm::mat4 model; 
			model = glm::translate(model, position); 
			glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (float)800 / (float)600, 0.1f, 100.0f);
			glm::mat4 mvp = proj * camera->GetViewMatrix() * model;

			GLint mvLoc = glGetUniformLocation(*program, "mvp"); 
			glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			glBindVertexArray(VAO); 
			glDrawArrays(GL_PATCHES, 0, 4); 
			glBindVertexArray(0);
		}
};