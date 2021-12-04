#pragma once
#include <string>
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};


class Shader
{
private:
	std::string shaderFilePath;

	// program ID of shader
	unsigned int rendererID;
	unsigned int textureUnit;
	std::vector<std::string> textures;

public:
	Shader() :rendererID(0) {};
	Shader(const std::string& path);
	~Shader();
	void Bind();
	void unBind();
	int getUniformLocation(const char* name);
	unsigned int getRendererID();
	
	void setMat4(const char* name, glm::mat4 matrix);
	void setMat4(const std::string& name, glm::mat4 matrix);
	void setVec3(const char* name, glm::vec3 vec);
	void setVec3(const std::string& name, glm::vec3 vec);
	void setVec2(const char* name, glm::vec2 vec);
	void setVec2(const std::string& name, glm::vec2 vec);
	void setFloat(const char* name, float value);
	void setFloat(const std::string& name, float value);

	void setInt(const char* name, int value);
	void setInt(const std::string& name, int value);

	void setTexture(const char* name, unsigned int texture);
	void setTexture(const std::string& name, unsigned int texture, int index = -1);

private:
	// path: res/Shaders/basic.shader
	// path is relative a path
	// absolute path is ($SolutionDir)path
	ShaderProgramSource parseShader(const std::string& path);

	// type: GL_FRAGMENT_SHADER or GL_VERTEX_SHADER
	// return shader program ID
	unsigned int compileShader(unsigned int type, const std::string& shaderSource);
	void shaderErrorInfo(unsigned int shader, unsigned int type);

	// vertexShader: shader source as a string
	// fragmentShader: shader source as a string
	// return shader ID
	unsigned int createShader(const std::string& vertexShader,
		const std::string& fragmentShader,
		const std::string& geometryShader);
};