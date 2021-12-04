#include "Cloth.h"
#include <iostream>

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

    node->vertex.TexCoord.x = (float)ColumnIndex / (nodesPerRow - 1);
    node->vertex.TexCoord.y = (float)rowIndex / (1 - nodesPerColumn);

   // printf("\t[%d, %d] (%f, %f, %f) - (%f, %f)\n", rowIndex, ColumnIndex, position.x, position.y, position.z, node->vertex.TexCoord.x, node->vertex.TexCoord.y);
    return node;
}


void Cloth::InitMassNodes()
{
    //printf("%d nodes per row\n");
    
    for (int i = 0; i < nodesPerRow; i++)
        for (int j = 0; j < nodesPerColumn; j++) nodes.push_back(CreateMassNode(i, j));
}



void Cloth::InitSprings(void)
{

}



std::vector<Vertex>& Cloth::GetVertices()
{
    vertices.clear();

    // iterate every node
    for (MassNode* node : nodes)
        vertices.push_back(node->vertex);


    //std::cout <<  "vertices size: " << vertices.size() << std::endl;
    return vertices;
}