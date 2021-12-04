#pragma once

#include "Core/Mesh.h"

class Scene;
class Shader;
class Cloth;


class Renderer
{
private:
	Shader *lightShader;
	bool debugMode;
	Cloth* cloth;

	int vertexSize = 1;
	unsigned int TextureID;

	void RenderUI();

	void RenderVertices(std::vector<Vertex>& vertices, int size);

public:
	Renderer(Scene const* scene);


	~Renderer();
	
	void Render(Scene const* scene);
	
};

