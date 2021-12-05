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
	Shader* debugShader;

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

	bool enableDebug = false;

	float windx;
	float windy;
	float windz;

	void RenderUI();

	void RenderVertices(std::vector<Vertex>& vertices, int size, unsigned int type);

	void RenderCloth();

	void DebugDraw();

public:
	Renderer(Scene const* scene);


	~Renderer();
	
	void Render(Scene const* scene);
	
};

