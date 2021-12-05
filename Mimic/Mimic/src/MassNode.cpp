#include "MassNode.h"
#include <iostream>

MassNode::MassNode(glm::vec3 position)
	:mass(1.0), velocity(0.0), acceleration(0), force(0), isFixed(false)
{
    vertex.Position = position;
    oldPos = position;
}

void MassNode::Simulate(float timeStamp)
{
    if (isFixed) return;

    acceleration = force / mass;
    velocity += acceleration * timeStamp;
   // vertex.Position += velocity * timeStamp;

    glm::vec3 temp = vertex.Position;
    vertex.Position += (vertex.Position - oldPos) + acceleration  * timeStamp * timeStamp;

    oldPos = temp;
    
    force = glm::vec3(0);   // reset
}