#include "Spring.h"
#include "MassNode.h"

Spring::Spring(MassNode* node1, MassNode *node2, float hookC, float dampC)
	:node1(node1), node2(node2), hookC(hookC), dampC(dampC)
{
	restLength = glm::length(node1->position - node2->position);
}


// Spring-Dampers
// Equations come from link below
// https://cseweb.ucsd.edu/classes/sp16/cse169-a/slides/CSE169_11.pdf page 12
void Spring::Simulate(float timeStamp)
{
	// Force Simulation
	// 	----------------------------------------------------------------- //
	// current distance between 2 nodes
	float currentLength = glm::length(node1->position - node2->position);
	
	// one of the two force directions
	glm::vec3 forceDir = glm::normalize(node1->position - node2->position);
	
	glm::vec3 velocityDiff = node1->velocity - node2->velocity;

	// hooke's law without direction, only force magnitude
	float HookMagnitude = (currentLength - restLength) * hookC;	

	// damping force
	float dampMagnitude = glm::dot(forceDir, velocityDiff) * dampC;

	glm::vec3 force = forceDir * (HookMagnitude + dampMagnitude);
	// 	----------------------------------------------------------------- //

	// apply force to two nodes
	// since the force direction is from node1 to node2
	// force can apply to node2 directly
	// force with opposite direction should be applied to node1

	node2->force = force;
	node1->force = -force;
}