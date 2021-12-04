#include "MassNode.h"


MassNode::MassNode(glm::vec3 position)
	:mass(1.0), position(position), velocity(0.0), acceleration(0), force(0), isFixed(false)
{}

void MassNode::Simulate(float timeStamp)
{
    if (isFixed) return;

    acceleration = force / mass;
    velocity += acceleration * timeStamp;
    position += velocity * timeStamp;
    
    force = glm::vec3(0);   // reset
}