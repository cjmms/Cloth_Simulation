#pragma once

class Shader;

class Scene
{
public:
	Scene();
	~Scene();

	static unsigned int loadTexture(char const* path, bool gamma = false);


	void initPlane();
	void initCube();
	void RenderCube(Shader* shader) const;
	void RenderPlane(Shader* shader) const;

private:

	unsigned int planeVAO;
	unsigned int cubeVAO;
};

