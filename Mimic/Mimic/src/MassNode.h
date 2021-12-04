#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MassNode
{
public:
	MassNode(glm::vec3 position);

	void Update(float timeStamp);

public:
	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 force;

	bool isFixed;
};