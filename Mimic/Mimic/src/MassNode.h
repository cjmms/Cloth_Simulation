#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MassNode
{
public:
	MassNode(glm::vec3 position);

	void Simulate(float timeStamp);

public:
	// physics
	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 force;

	// graphics
	glm::vec2 texCoord;
	glm::vec3 normal;

	bool isFixed;
};