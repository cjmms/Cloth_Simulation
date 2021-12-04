#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 intensity);
	~Light();

	inline glm::vec3 getPos() const { return position; }

	inline glm::vec3 getIntensity() const { return intensity; }

	glm::mat4 getModelMatrix() const;

	inline Model const* getModel() const { return lightSphere; }

private:
	Model *lightSphere;
	glm::vec3 position;
	glm::vec3 intensity;

};