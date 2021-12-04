#pragma once
#include <vector>
#include "Core/Light.h"


struct Object
{
	Model* model;
	glm::vec3 scale;

	Object(Model* model, glm::vec3 scale);
	~Object();

	glm::mat4 getModelMatrix() const;
	inline Model const* getModel() { return model; }

};


class Scene
{
public:
	Scene();
	~Scene();

	inline void addLightSource(glm::vec3 position, glm::vec3 intensity) { lightSources.push_back(new Light(position, intensity)); }

	void addObjects(const char* address, glm::vec3 scale);

	inline const std::vector<Light* >& getLightSources() const { return lightSources; }
	inline const std::vector<Object* >& getObjects() const { return objects; }
	void InitTextures(std::string str);
	void BindTextures(Shader* shader) const;

	void initPlane();
	void initCube();
	void RenderCube(Shader* shader) const;
	void RenderPlane(Shader* shader) const;

private:
	std::vector<Light* > lightSources;
	std::vector<Object* > objects;

	unsigned int albedo, normal, roughness, metallic;

	unsigned int planeVAO;
	unsigned int cubeVAO;
};

