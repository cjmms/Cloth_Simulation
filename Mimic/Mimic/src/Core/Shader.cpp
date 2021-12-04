#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>


Shader::Shader(const std::string& path)
    :rendererID(0), shaderFilePath(path), textureUnit(0)
{
    ShaderProgramSource source = parseShader(shaderFilePath);
    rendererID = createShader(source.VertexSource,
        source.FragmentSource,
        source.GeometrySource);
}

Shader::~Shader()
{
    glDeleteProgram(rendererID);
}



ShaderProgramSource Shader::parseShader(const std::string& path)
{
    enum class ShaderMode {
        NONE = -1, VERTEX_MODE = 0, FRAGMENT_MODE = 1, GEOMETRY_MODE = 2
    };
    std::fstream stream(path);
    std::string line;

    // 3 string streams, one for vertex shader, one for fragment shader, one for geometry shader
    std::stringstream ss[3];
    ShaderMode currentMode = ShaderMode::NONE;

    // shader file has 2 lines: #shader fragment and #shader vertex 
    // if any or these lines are detected, switch mode, output to another stringstream
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                currentMode = ShaderMode::VERTEX_MODE;
            else if (line.find("fragment") != std::string::npos)
                currentMode = ShaderMode::FRAGMENT_MODE;
            else if (line.find("geometry") != std::string::npos &&
                !(line.find("//") != std::string::npos))
                currentMode = ShaderMode::GEOMETRY_MODE;
        }
        else
        {
            ss[(int)currentMode] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str(), ss[2].str() };
}


unsigned int Shader::createShader(const std::string& vertexShader,
    const std::string& fragmentShader,
    const std::string& geometryShader)
{
    bool hasGeometryShader = geometryShader.length() > 0;


    unsigned int shaderProgram = glCreateProgram();
    unsigned int vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    unsigned int geometryShaderID;

    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);

    if (hasGeometryShader) {
        geometryShaderID = compileShader(GL_GEOMETRY_SHADER, geometryShader);
        glAttachShader(shaderProgram, geometryShaderID);
    }


    // TODO: catch exceptions
    glLinkProgram(shaderProgram);

    //TODO: catch exceptions
    glValidateProgram(shaderProgram);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    if (hasGeometryShader) glDeleteShader(geometryShaderID);

    return shaderProgram;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& shaderSource)
{
    unsigned int id = glCreateShader(type);
    const char* source = shaderSource.c_str();

    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    shaderErrorInfo(id, type);
    return id;
}

void Shader::shaderErrorInfo(unsigned int shader, unsigned int type)
{
    GLint success;
    char infoLog[512];
    // success is a flag, it will be set to False if things went wrong
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            std::cout << "File Name: " << shaderFilePath << " ERROR: Vertex Shader Compilation Error. " << infoLog << std::endl;
        else if (type == GL_FRAGMENT_SHADER)
            std::cout << "File Name: " << shaderFilePath << " ERROR: Fragment Shader Compilation Error. " << infoLog << std::endl;
        else
            std::cout << "File Name: " << shaderFilePath << " ERROR: Geometry Shader Compilation Error. " << infoLog << std::endl;
    }
}

void Shader::Bind()
{
    glUseProgram(rendererID);
}

void Shader::unBind()
{
    glUseProgram(0);
}


int Shader::getUniformLocation(const char* name)
{
    return glGetUniformLocation(rendererID, name);
}

unsigned int Shader::getRendererID()
{
    return rendererID;
}


void Shader::setMat4(const char* name, glm::mat4 matrix)
{
    this->Bind();
    unsigned int location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    this->unBind();
}


void Shader::setMat4(const std::string& name, glm::mat4 matrix)
{
    setMat4(name.c_str(), matrix);
}



void Shader::setVec3(const char* name, glm::vec3 vec)
{
    this->Bind();
    unsigned int location = getUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(vec));
    this->unBind();
}


void Shader::setVec3(const std::string& name, glm::vec3 vec)
{
    setVec3(name.c_str(), vec);
}



void Shader::setVec2(const char* name, glm::vec2 vec)
{
    unsigned int location = getUniformLocation(name);
    glUniform2fv(location, 1, glm::value_ptr(vec));
}


void Shader::setVec2(const std::string& name, glm::vec2 vec)
{
    setVec2(name.c_str(), vec);
}



void Shader::setFloat(const char* name, float value)
{
    this->Bind();
    unsigned int location = getUniformLocation(name);
    glUniform1f(location, value);
    this->unBind();
}



void Shader::setFloat(const std::string& name, float value)
{
    setFloat(name.c_str(), value);
}



void Shader::setInt(const char* name, int value) 
{
    this->Bind();
    unsigned int location = getUniformLocation(name);
    glUniform1i(location, value);
    this->unBind();
}



void Shader::setInt(const std::string& name, int value)
{
    setInt(name.c_str(), value);
}


void Shader::setTexture(const char* name, unsigned int texture)
{
    
    auto it = std::find(textures.begin(), textures.end(), name);

    // If element was found
    if (it != textures.end())
    {
        // calculating the index
        int index = it - textures.begin();
        
        this->setInt(name, index);

        this->Bind();
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, texture);
        this->unBind();
    }
    else {
        // If the element is not
        // present in the vector

        this->setInt(name, textureUnit);

        this->Bind();
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture);
        this->unBind();

        textureUnit++;
        textures.push_back(name);
    }
}


void Shader::setTexture(const std::string& name, unsigned int texture, int index)
{
    setTexture(name.c_str(), texture, index);
}
