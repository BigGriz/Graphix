/***
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2020 Media Design School

  File Name   :   Terrain.cpp
  Description :   Terrain Class Implementation
  Date		  :	  14/05/2020
  Author      :   Wayd Barton-Redgrave
  Mail        :   wayd.bar8374@mediadesign.school.nz
***/

// Local Includes
#include "Terrain.h"

// Constructor
Terrain::Terrain(const char* imagePath, GLuint* _program) 
{
	//createVertexData(imagePath);
	createVertexData();
	createIndexData();

	if (width == 0 || height == 0) {
		std::cout << "Image loading failed";
		return;
	}

	vertexBuffer = new VertexBuffer(vertices);
	indexBuffer = new IndexBuffer(indexes);

	program = _program;
}

// Destructor
Terrain::~Terrain()
{
	delete vertexBuffer;
	delete indexBuffer;
}

// Rendering Functionality
void Terrain::Render() 
{
	//glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);

	glUseProgram(*program);

	glBindVertexArray(vertexBuffer->VAO);
	vertexBuffer->setVertexAttribPointer(0, 0, 3, 3);

	//glm::mat4 view = _cam->GetViewMatrix();
	//glm::mat4 projection = glm::perspective(_cam->fov, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glm::mat4 model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	GLint modelLoc = glGetUniformLocation(*program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	/*GLint viewLoc = glGetUniformLocation(*program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint projLoc = glGetUniformLocation(*program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->bufferId);
	//glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_SHORT, 0);
	glDrawArrays(GL_TRIANGLES, indexes[0], indexes.size());
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	//glBindVertexArray(0);
}

// Create Vertices for Image
void Terrain::createVertexData(const char* imagePath) {
	unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);

	for (int row = 0; row < height; row++) 
	{
		for (int col = 0; col < width; col++) 
		{
			int r = image[(col + row * width) * 3 + 0];
			int g = image[(col + row * width) * 3 + 1];
			int b = image[(col + row * width) * 3 + 2];

			int gValue = (r + g + b) / 3;

			float xpos = ((float)col / (float)(width - 1)) - 0.5f;
			float ypos = (float)gValue / (float)255;
			float zpos = ((float)row / (float)(height - 1)) - 0.5f;

			vertices.push_back(glm::vec3(xpos, ypos, zpos));
		}
	}

	delete (image);
}

// Create Indices of Triangles
void Terrain::createIndexData() 
{
	for (int row = 0; row < height - 1; row++) 
	{
		for (int col = 0; col < width - 1; col++) 
		{
			int topLeftIndexNum = row * width + col;
			int topRightIndexNum = row * width + col + 1;
			int bottomLeftIndexNum = (row + 1) * width + col;
			int bottomRightIndexNum = (row + 1) * width + col + 1;

			indexes.push_back(topLeftIndexNum);
			indexes.push_back(bottomLeftIndexNum);
			indexes.push_back(topRightIndexNum);

			indexes.push_back(topRightIndexNum);
			indexes.push_back(bottomLeftIndexNum);
			indexes.push_back(bottomRightIndexNum);
		}
	}
}

// Create Vertices using Noise
void Terrain::createVertexData()
{
	height = 40;
	width = 40;

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			float gValue = totalNoisePerPoint(width, height);

			float xpos = col;
			float ypos = gValue;
			float zpos = row;

			vertices.push_back(glm::vec3(xpos, ypos, zpos));
		}
	}
}

#pragma region terrainNoise
// Generic Noise Function - pass through random from seeded RNG
float Terrain::random(int x, int y, float _rand = 0.0f)
{
	int n = x + y * 57 + _rand;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return (1.0f - double(t) * 0.931322574615478515625e-9);
}
// Generic Smooth Function
float Terrain::smooth(int x, int y)
{
	float corners;
	float sides;
	float center;

	// Generate Random Num
	float randNum = rand() % 100;

	corners = (random(x - 1, y - 1, randNum) + random(x + 1, y - 1, randNum) + random(x - 1, y + 1, randNum) + random(x + 1, y + 1, randNum)) / 16;
	sides = (random(x - 1, y, randNum) + random(x + 1, y, randNum) + random(x, y - 1, randNum) + random(x, y + 1, randNum)) / 8;
	center = random(x, y, randNum) / 4;

	return (corners + sides + center);
}
// Interpolation of Noise
float Terrain::interpolate(float a, float b, float x)
{
	return(a * (1 - x) + b * x);
}

// Perlin Noise
float Terrain::linear_interpolate(float a, float b, float x)
{
	return (a * (1 - x) + b * x);
}
float Terrain::cosine_interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * 0.5f;

	return (a * (1 - f) + b * f);
}
float Terrain::cubic_interpolate(float v0, float v1, float v2, float v3, float x)
{
	float P = v3 - v2 - v0 - v1;
	float Q = v0 - v1 - P;
	float R = v2 - v0;
	float S = v1;

	return (P * powf(x, 3) + Q * powf(x, 2) + R * x + S);
}
float Terrain::noise(float x, float y)
{
	float fractional_X = x - int(x);
	float fractional_Y = y - int(y);

	// Smooths
	float v1 = smooth(int(x), int(y));
	float v2 = smooth(int(x) + 1, int(y));
	float v3 = smooth(int(x), int(y) + 1);
	float v4 = smooth(int(x) + 1, int(y) + 1);

	// Interpolates
	float i1 = cosine_interpolate(v1, v2, fractional_X);
	float i2 = cosine_interpolate(v3, v4, fractional_X);

	return (cosine_interpolate(i1, i2, fractional_Y));
}

// Pass Through X + Z - Return Y
float Terrain::totalNoisePerPoint(int x, int y)
{
	int octaves = 12;
	float zoom = 20.0f;
	float persistence = 0.25f;
	float total = 0.0f;

	for (int i = 0; i < octaves - 1; i++)
	{
		float frequency = pow(2, i) / zoom;
		float amplitude = pow(persistence, i);

		total += noise(x * frequency, y * frequency) * amplitude;
	}

	return (total);
}
#pragma endregion terrainNoise