#include "Terrain.h"

Terrain::Terrain(const char* imagePath, GLuint* _program) 
{
	createVertexData(imagePath);
	createIndexData();

	if (width == 0 || height == 0) {
		std::cout << "Image loading failed";
		return;
	}

	vertexBuffer = new VertexBuffer(vertices);
	indexBuffer = new IndexBuffer(indexes);

	program = _program;
}

Terrain::~Terrain()
{
	delete vertexBuffer;
	delete indexBuffer;
}

void Terrain::Render(Camera* _cam) 
{	
	vertexBuffer->setVertexAttribPointer(0, 0, 3, 3);

	glm::mat4 view = _cam->GetViewMatrix();
	glm::mat4 projection = glm::perspective(_cam->fov, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glm::mat4 model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	GLint modelLoc = glGetUniformLocation(*program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint viewLoc = glGetUniformLocation(*program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint projLoc = glGetUniformLocation(*program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glUseProgram(*program);
	glBindVertexArray(vertexBuffer->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->bufferId);
	glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_SHORT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
}

void Terrain::createVertexData(const char* imagePath) {
	unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			unsigned char r = image[(col + row * width) * 3 + 0];
			unsigned char g = image[(col + row * width) * 3 + 1];
			unsigned char b = image[(col + row * width) * 3 + 2];
			int gValue = (r + g + b) / 3;;
			float xpos = ((float)col / (float)(width - 1)) - 0.5f;
			float ypos = (float)gValue / (float)255;
			float zpos = ((float)row / (float)(height - 1)) - 0.5f;

			vertices.push_back(glm::vec3(xpos, ypos, zpos));
		}
	}

	delete image;
}

void Terrain::createIndexData() {
	for (int row = 0; row < height - 1; row++) {
		for (int col = 0; col < width - 1; col++) {
			short topLeftIndexNum = (short)(row * width + col);
			short topRightIndexNum = (short)(row * width + col + 1);
			short bottomLeftIndexNum = (short)((row + 1) * width + col);
			short bottomRightIndexNum = (short)((row + 1) * width + col + 1);

			indexes.push_back(topLeftIndexNum);
			indexes.push_back(bottomLeftIndexNum);
			indexes.push_back(topRightIndexNum);

			indexes.push_back(topRightIndexNum);
			indexes.push_back(bottomLeftIndexNum);
			indexes.push_back(bottomRightIndexNum);
		}
	}
}
