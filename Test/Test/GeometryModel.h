/***
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2020 Media Design School

  File Name   :   GeometryModel.h
  Description :   Geometry Class Declaration & Implementation
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
class GeometryModel
{
	public:
		GLuint* program;
		Camera* camera;
	
		GLuint VAO, VBO;
		glm::mat4 model;
		glm::vec3 position;

		// Constructor & Destructor
		GeometryModel(GLuint* program, Camera* camera)
		{
			this->program = program; 
			this->camera = camera;
		
			GLfloat points[] = { 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f }; //passing in 1 point };

			glBindVertexArray(VAO); 
			glGenBuffers(1, &VBO);

			glGenVertexArrays(1, &VAO); 
			glBindBuffer(GL_ARRAY_BUFFER, VBO); 
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0); 

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0); 
			glEnableVertexAttribArray(1); 

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glBindVertexArray(0);
		}
		~GeometryModel() {};
		// Rendering Functionality
		void Render() 
		{
			glUseProgram(*program);
			glm::mat4 model; 
			model = glm::translate(model, position); 
			glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (float)800 / (float)600, 0.1f, 100.0f);
			glm::mat4 mvp = proj * camera->GetViewMatrix() * model;
			GLint vpLoc = glGetUniformLocation(*program, "mvp"); 
			glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
			glBindVertexArray(VAO); glDrawArrays(GL_POINTS, 0, 1); 
			glBindVertexArray(0);
		}
};