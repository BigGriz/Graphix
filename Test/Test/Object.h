#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <soil/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>


class Object
{
	public:
		GLuint* program;
		GLuint* border;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		glm::vec3 position;
		glm::vec3 scale;
		bool fog = false;

		std::vector<unsigned int> textures;

		Object() {};
		Object(GLuint* _program, GLuint* _border, glm::vec3 _position, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), bool _fog = true)
		{
			position = _position;
			SetupObject(_program, _border);
			scale = _scale;
			fog = _fog;			
		}
		~Object() {};

		void SetupObject(GLuint* _program, GLuint* _border)
		{
			
			program = _program;
			border = _border;

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			//glGenBuffers(1, &tempEBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

			// Position Attributes
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// Texture Attributes
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		void LoadTexture(const char* _filename)
		{
			unsigned int tempTex;
			glGenTextures(1, &tempTex);
			glBindTexture(GL_TEXTURE_2D, tempTex); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			int width, height, nrChannels;

			unsigned char* data = SOIL_load_image(_filename, &width, &height, &nrChannels, SOIL_LOAD_RGBA);

			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			SOIL_free_image_data(data);

			glUseProgram(*program);
			(textures.empty()) ? glUniform1i(glGetUniformLocation(*program, "texture1"), 0) : glUniform1i(glGetUniformLocation(*program, "texture2"), 1);

			glUseProgram(*border);
			(textures.empty()) ? glUniform1i(glGetUniformLocation(*border, "texture1"), 0) : glUniform1i(glGetUniformLocation(*border, "texture2"), 1);

			textures.push_back(tempTex);
		}

		void Render(glm::vec3 _cameraPos)
		{
			// bind textures on corresponding texture units
			for (int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures.at(i));
			}

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			// First Pass
			glUseProgram(*program);
			glBindVertexArray(VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures.at(0));
			// Draw
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = glm::scale(model, scale);

			unsigned int modelLoc = glGetUniformLocation(*program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			unsigned int fogLoc = glGetUniformLocation(*program, "fog");
			glUniform1i(fogLoc, fog);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			
		}

		void DrawBorder(glm::vec3 _cameraPos)
		{
	
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			// Second Pass
			glUseProgram(*border);
			// Draw
			float scaleUp = 1.1;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = glm::scale(model, scale);
			model = glm::scale(model, glm::vec3(scaleUp, scaleUp, scaleUp));
			unsigned int modelLoc = glGetUniformLocation(*border, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			unsigned int fogLoc = glGetUniformLocation(*border, "fog");
			glUniform1i(fogLoc, fog);
			
			glBindVertexArray(0);
			
			glStencilMask(0xFF);
			glDisable(GL_STENCIL_TEST);
		}

		float vertices[180] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-left
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // bottom-right         
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // top-right
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // top-left
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // bottom-left
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // top-left
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // bottom-right
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			// Right face
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-left
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // top-right         
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-left
			 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // bottom-right
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // top-right
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // top-left
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // top-right     
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // bottom-right
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // top-left
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f  // bottom-left     
		};
		GLuint indices[36] = {  //Tell OpenGL What triangle uses what Vertecies
			 0, 1, 3,   //Back Quad
			 1, 2, 3,
			 0, 1, 4,    //Right Quad
			 1, 5, 4,
			 2, 3, 7,   //Left Quad
			 2, 6, 7,
			 4, 5, 7,   //Front Quad
			 5, 6, 7,
			 0, 3, 4,   //Top Quad
			 3, 4, 7,
			 1, 2, 5,   //Bottom Quad
			 2, 5, 6
		};
};