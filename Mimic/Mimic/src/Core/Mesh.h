#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "Shader.h"



struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
//	glm::vec3 Normal;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	Mesh( std::vector<Vertex> vertices, 
		  std::vector<Texture> textures, 
		  std::vector<unsigned int> indices );

	void Draw(Shader& shader) const;

private:
	unsigned int VAO;

	void setup();

};

