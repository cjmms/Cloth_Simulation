#pragma once

#include <string>
#include "glm\glm.hpp"

class Shader
{
public:
	Shader(const std::string& fileName);

	// bind shader
	void Bind();
	void Update(glm::mat4 MVP);

	virtual ~Shader();

private:
	// fragment shader and vertex shader
	static const unsigned int NUM_SHADERS = 2;	
	static const unsigned int NUM_UNIFORMS = 1;

	// load shader form filename
	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage);
	// create 2 shaders and bind together with current program
	unsigned int CreateShader(const std::string& text, unsigned int type);

	unsigned int m_program;

	// contains fragment shader and vertex shader
	unsigned int m_shaders[NUM_SHADERS];
	unsigned int m_uniforms[NUM_UNIFORMS];
};


