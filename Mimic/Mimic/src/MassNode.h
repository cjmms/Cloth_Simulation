#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Mesh.h"

class MassNode
{
public:
	MassNode(glm::vec3 position);

	void Simulate(float timeStamp);
	inline glm::vec3 Position(void) const { return vertex.Position; }

public:
	// physics
	float mass;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 force;

	Vertex vertex;

	bool isFixed;
};