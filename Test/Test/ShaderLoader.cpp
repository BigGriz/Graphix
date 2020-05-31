/***
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2019 Media Design School

  File Name   :   ShaderLoader.cpp
  Description :   Implementation of ShaderLoader Class Functions
  Date		  :	  02/10/2019
  Author      :   Wayd Barton-Redgrave
  Mail        :   wayd.bar8374@mediadesign.school.nz
***/
#pragma once
// Header Includes
#include "ShaderLoader.h"
// Static Maps for Shaders + Programs
std::map<const char*, GLuint> ShaderLoader::shaderMap;
std::map<std::string, GLuint> ShaderLoader::programMap;

//****************Constructor & Deconstructor*******************//
// Description      : Sets up Shader Loader.					//
//**************************************************************// 
ShaderLoader::ShaderLoader(void) {}
ShaderLoader::~ShaderLoader(void) {}

//*******************Create Shader Program**********************//
// Description      : Create program from vertex + frag shader.	//
// @params			: Pathname for vertex + frag shaders		//
// @return GLuint	: Program # to Store						//
//**************************************************************// 
GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* geometryShaderFilename)
{
	// Append Shader Filenames
	std::string programName(vertexShaderFilename);
	programName.append(fragmentShaderFilename);

	// Check if Program with Filename = Appended already exists
	// If so return program
	if (programMap.find(programName) != programMap.end())
	{
		GLuint program = programMap.find(programName)->second;
	}
	// If not create new program
	else
	{
		GLuint program = glCreateProgram();

		GLuint vertexshader;
		GLuint fragmentshader;
		GLuint geoshader;

		// Check if vertex already exists in map
		// If not, create shader
		if (shaderMap.find(vertexShaderFilename) == shaderMap.end())
		{
			vertexshader = CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
			shaderMap[vertexShaderFilename] = vertexshader;
		}
		// If so, load shader
		else
		{
			vertexshader = shaderMap.find(vertexShaderFilename)->second;
		}

		// Check if fragment already exists in map
		// If not, create shader
		if (shaderMap.find(fragmentShaderFilename) == shaderMap.end())
		{
			fragmentshader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
			shaderMap[fragmentShaderFilename] = fragmentshader;
		}
		// If so, load shader
		else
		{
			fragmentshader = shaderMap.find(fragmentShaderFilename)->second;
		}

		// Check if geometry already exists in map
		// If not, create shader
		if (shaderMap.find(geometryShaderFilename) == shaderMap.end())
		{
			geoshader = CreateShader(GL_GEOMETRY_SHADER, geometryShaderFilename);
			shaderMap[geometryShaderFilename] = geoshader;
		}
		// If so, load shader
		else
		{
			geoshader = shaderMap.find(geometryShaderFilename)->second;
		}

		// Attach shaders to program
		glAttachShader(program, vertexshader);
		glAttachShader(program, fragmentshader);
		glAttachShader(program, geoshader);

		// Link program
		glLinkProgram(program);

		// Check for link errors
		int link_result = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &link_result);
		if (link_result == GL_FALSE)
		{
			std::string programName = vertexShaderFilename + *fragmentShaderFilename + *geometryShaderFilename;
			PrintErrorDetails(false, program, programName.c_str());
			return (0);
		}

		// Save program to map
		programMap[programName] = program;

		return program;
	}

	return (0);
}
GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	// Append Shader Filenames
	std::string programName(vertexShaderFilename);
	programName.append(fragmentShaderFilename);

	// Check if Program with Filename = Appended already exists
	// If so return program
	if (programMap.find(programName) != programMap.end())
	{
		GLuint program = programMap.find(programName)->second;
	}
	// If not create new program
	else
	{
		GLuint program = glCreateProgram();

		GLuint vertexshader;
		GLuint fragmentshader;
		GLuint geoshader;

		// Check if vertex already exists in map
		// If not, create shader
		if (shaderMap.find(vertexShaderFilename) == shaderMap.end())
		{
			vertexshader = CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
			shaderMap[vertexShaderFilename] = vertexshader;
		}
		// If so, load shader
		else
		{
			vertexshader = shaderMap.find(vertexShaderFilename)->second;
		}

		// Check if fragment already exists in map
		// If not, create shader
		if (shaderMap.find(fragmentShaderFilename) == shaderMap.end())
		{
			fragmentshader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
			shaderMap[fragmentShaderFilename] = fragmentshader;
		}
		// If so, load shader
		else
		{
			fragmentshader = shaderMap.find(fragmentShaderFilename)->second;
		}

		
		// Attach shaders to program
		glAttachShader(program, vertexshader);
		glAttachShader(program, fragmentshader);

		// Link program
		glLinkProgram(program);

		// Check for link errors
		int link_result = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &link_result);
		if (link_result == GL_FALSE)
		{
			std::string programName = vertexShaderFilename + *fragmentShaderFilename;
			PrintErrorDetails(false, program, programName.c_str());
			return (0);
		}

		// Save program to map
		programMap[programName] = program;

		return program;
	}

	return (0);
}
GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename)
{
	// Append Shader Filenames
	std::string programName(vertexShaderFilename);
	programName.append(fragmentShaderFilename);
	programName.append(TessControlShaderFilename);
	programName.append(TessEvalShaderFilename);

	// Check if Program with Filename = Appended already exists
	// If so return program
	if (programMap.find(programName) != programMap.end())
	{
		GLuint program = programMap.find(programName)->second;
	}
	// If not create new program
	else
	{
		GLuint program = glCreateProgram();

		GLuint vertexshader;
		GLuint fragmentshader;
		GLuint tcsshader;
		GLuint tesshader;

		// Check if vertex already exists in map
		// If not, create shader
		if (shaderMap.find(vertexShaderFilename) == shaderMap.end())
		{
			vertexshader = CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
			shaderMap[vertexShaderFilename] = vertexshader;
		}
		// If so, load shader
		else
		{
			vertexshader = shaderMap.find(vertexShaderFilename)->second;
		}

		// Check if fragment already exists in map
		// If not, create shader
		if (shaderMap.find(fragmentShaderFilename) == shaderMap.end())
		{
			fragmentshader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
			shaderMap[fragmentShaderFilename] = fragmentshader;
		}
		// If so, load shader
		else
		{
			fragmentshader = shaderMap.find(fragmentShaderFilename)->second;
		}

		// Check if tcs already exists in map
		// If not, create shader
		if (shaderMap.find(TessControlShaderFilename) == shaderMap.end())
		{
			tcsshader = CreateShader(GL_TESS_CONTROL_SHADER, TessControlShaderFilename);
			shaderMap[TessControlShaderFilename] = tcsshader;
		}
		// If so, load shader
		else
		{
			tcsshader = shaderMap.find(TessControlShaderFilename)->second;
		}

		// Check if tes already exists in map
		// If not, create shader
		if (shaderMap.find(TessEvalShaderFilename) == shaderMap.end())
		{
			tesshader = CreateShader(GL_TESS_EVALUATION_SHADER, TessEvalShaderFilename);
			shaderMap[TessEvalShaderFilename] = tesshader;
		}
		// If so, load shader
		else
		{
			tesshader = shaderMap.find(TessEvalShaderFilename)->second;
		}


		// Attach shaders to program
		glAttachShader(program, vertexshader);
		glAttachShader(program, fragmentshader);
		glAttachShader(program, tcsshader);
		glAttachShader(program, tesshader);

		// Link program
		glLinkProgram(program);

		// Check for link errors
		int link_result = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &link_result);
		if (link_result == GL_FALSE)
		{
			std::string programName = vertexShaderFilename + *fragmentShaderFilename;
			PrintErrorDetails(false, program, programName.c_str());
			return (0);
		}

		// Save program to map
		programMap[programName] = program;

		return program;
	}

	return (0);
}

//***********************Create Shader**************************//
// Description      : Create Shader.							//
// @param shaderType: Define Shader Type.						//
// @param fragfilena: Pointer to Shader.						//
// @return GLuint	: Shader # to Store							//
//**************************************************************// 
GLuint ShaderLoader::CreateShader(GLenum shaderType, const char* shaderName)
{
	std::string shaderCode = ReadShaderFile(shaderName);

	const char* shader_code_ptr = shaderCode.c_str();
	const int shader_code_size = shaderCode.size();

	GLuint shaderID = glCreateShader(shaderType);

	glShaderSource(shaderID, 1, &shader_code_ptr, &shader_code_size);

	glCompileShader(shaderID);

	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails(true, shaderID, shaderName);
		return 0;
	}
	return shaderID;
}

//*********************Read Shader File*************************//
// Description      : Get details from file.					//
// @param filename	: File Pathname containing details.			//
// @return string   : Returns details from Shader				//
//**************************************************************// 
std::string ShaderLoader::ReadShaderFile(const char* filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

//**********************Error Response**************************//
// Description      : Prints Error if Error.					//
// @param isShader	: Confirmation if Shader/Program			//
// @param id		: ID # for Shader/Program.					//
// @param name		: Name for Shader/Program.					//
//**************************************************************// 
void ShaderLoader::PrintErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(isShader == true) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);
	std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << name << std::endl;
	std::cout << &log[0] << std::endl;
}

//*******************Get Shader Instance************************//
// Description      : Returns ShaderLoader Reference.			//
// @return Loader&  : Reference to ShaderLoader.				//
//**************************************************************// 
ShaderLoader& ShaderLoader::GetInstance()
{
	return(*this);
}