#include "Cloth.h"
#include <iostream>

Cloth::Cloth(glm::vec3 position, int width, int height, int nodesDensity)
	:position(position), width(width), height(height), nodesDensity(nodesDensity)
{
	nodesPerRow = width * nodesDensity;
	nodesPerColumn = height * nodesDensity;

    for (int i = 0; i < nodesPerRow; i++) 
        for (int j = 0; j < nodesPerColumn; j++)  CreateMassNode(i, j);  // new mass node
  
    for (int i = 0; i < nodesPerRow; i++)
        for (int j = 0; j < nodesPerColumn; j++)  CreateSprings(i, j);     // new springs
       

    getNode(0, 0)->isFixed = true;
    getNode(nodesPerRow - 1, 0)->isFixed = true;
}

void Cloth::CreateMassNode(int rowIndex, int ColumnIndex)
{
    glm::vec3 position;
    position.x = (float)ColumnIndex / (float)nodesDensity;
    position.y = (float)(-rowIndex) / (float)nodesDensity;
    position.z = 0;

    MassNode* node = new MassNode(position);

    node->vertex.TexCoord.x = (float)ColumnIndex / (nodesPerRow - 1);
    node->vertex.TexCoord.y = (float)rowIndex / (1 - nodesPerColumn);

   // printf("\t[%d, %d] (%f, %f, %f) - (%f, %f)\n", rowIndex, ColumnIndex, position.x, position.y, position.z, node->vertex.TexCoord.x, node->vertex.TexCoord.y);
    nodes.push_back(node);
}



void Cloth::CreateSprings(int rowIndex, int ColumnIndex)
{
    /** Structural **/
    if (rowIndex < nodesPerRow - 1) springs.push_back(new Spring(getNode(rowIndex, ColumnIndex), getNode(rowIndex + 1, ColumnIndex), structuralCoef, dampCoef));
    if (ColumnIndex < nodesPerColumn - 1) springs.push_back(new Spring(getNode(rowIndex, ColumnIndex), getNode(rowIndex, ColumnIndex + 1), structuralCoef, dampCoef));
    /** Shear **/
    if (rowIndex < nodesPerRow - 1 && ColumnIndex < nodesPerColumn - 1) {
        springs.push_back(new Spring(getNode(rowIndex, ColumnIndex), getNode(rowIndex + 1, ColumnIndex + 1), shearCoef, dampCoef));
        springs.push_back(new Spring(getNode(rowIndex + 1, ColumnIndex), getNode(rowIndex, ColumnIndex + 1), shearCoef, dampCoef));
    }
    /** Bending **/
    if (rowIndex < nodesPerRow - 2) springs.push_back(new Spring(getNode(rowIndex, ColumnIndex), getNode(rowIndex + 2, ColumnIndex), bendingCoef, dampCoef));
    if (ColumnIndex < nodesPerColumn - 2) springs.push_back(new Spring(getNode(rowIndex, ColumnIndex), getNode(rowIndex, ColumnIndex + 2), bendingCoef, dampCoef));
}



std::vector<Vertex>& Cloth::GetVertices()
{
    vertices.clear();

    // iterate every node
    for (MassNode* node : nodes)
        vertices.push_back(node->vertex);

    return vertices;
}



void Cloth::SimulateGravity(void)
{
    for (MassNode* node: nodes)
        node->force += (glm::vec3(0.0, -9.8 * 0.1, 0.0) * node->mass);    // G = g * m,    g = 9.8 
}



void Cloth::SimulateInternalForce(float timeStamp)
{
    for (Spring* spring: springs)  spring->Simulate(timeStamp);
}


void Cloth::Simulate(float timeStamp)
{
    SimulateGravity();
    SimulateInternalForce(timeStamp);

    for (MassNode* node : nodes) node->Simulate(timeStamp); // update physical propertis of every node
}