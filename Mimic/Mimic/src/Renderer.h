#pragma once

#include "MassNode.h"
#include <vector>

class Scene;
class Shader;
class Cloth;




class Renderer
{
private:
	Shader *clothShader;
	Shader* sphereShader;

	bool debugMode;
	Cloth* cloth;

	int vertexSize = 1;
	unsigned int TextureID;


	// debug properties
	double windSimTime;
	double springSimTime;
	double nodesSimTime;
	double normalUpdateTime;
	double gravitySimTime;

	void RenderUI();

	void RenderVertices(std::vector<Vertex>& vertices, int size);

public:
	Renderer(Scene const* scene);


	~Renderer();
	
	void Render(Scene const* scene);
	
};

