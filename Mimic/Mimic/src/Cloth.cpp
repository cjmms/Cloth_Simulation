#include "Cloth.h"

Cloth::Cloth(glm::vec3 position, int width, int height, int nodesDensity)
	:position(position), width(width), height(height), nodesDensity(nodesDensity)
{
	nodesPerRow = width * nodesDensity;
	nodesPerColumn = height * nodesDensity;

	InitMassNodes();
	InitSprings();
}

MassNode* Cloth::CreateMassNode(int rowIndex, int ColumnIndex)
{
    glm::vec3 position;
    position.x = (float)ColumnIndex / (float)nodesDensity;
    position.y = (float)(-rowIndex) / (float)nodesDensity;
    position.z = 0;

    MassNode* node = new MassNode(position);

    node->texCoord.x = (float)ColumnIndex / (nodesPerRow - 1);
    node->texCoord.y = (float)rowIndex / (1 - nodesPerColumn);
    return node;
}


void Cloth::InitMassNodes()
{
    for (int i = 0; i < nodesPerRow; i++)
        for (int j = 0; j < nodesPerColumn; j++) nodes.push_back(CreateMassNode(nodesPerRow, nodesPerColumn));
}



void Cloth::InitSprings(void)
{

}