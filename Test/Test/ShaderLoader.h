/***
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2019 Media Design School

  File Name   :   ShaderLoader.h
  Description :   Class Declaration for Shader Loaders
  Date		  :	  02/10/2019
  Author      :   Wayd Barton-Redgrave
  Mail        :   wayd.bar8374@mediadesign.school.nz
***/
#pragma once

// Library Includes
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
// Dependency Includes
#include <glad/glad.h>

// Class Declaration
class ShaderLoader
{
public:
	ShaderLoader(void);
	~ShaderLoader(void);
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);

	static std::map<const char*, GLuint> shaderMap;
	static std::map<std::string, GLuint> programMap;
	ShaderLoader& GetInstance();
private:
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};
