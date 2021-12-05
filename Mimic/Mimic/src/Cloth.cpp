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
       
    std::cout << "Init Cloth with " << nodes.size() << " mass nodes." << std::endl;
    std::cout << "Init Cloth with " << springs.size() << " springs." << std::endl;


    // pin first line of nodes
    for (int i = 0; i < nodesPerRow; ++i) getNode(i, 0)->isFixed = true;
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


std::vector<Vertex>& Cloth::GetTriangles()
{
    vertices.clear();

    for (int i = 0; i < nodesPerRow - 1; i++) {
        for (int j = 0; j < nodesPerColumn - 1; j++) {
            // Left upper triangle
            vertices.push_back(getNode(i + 1, j)->vertex);
            vertices.push_back(getNode(i, j)->vertex);
            vertices.push_back(getNode(i, j + 1)->vertex);
            // Right bottom triangle
            vertices.push_back(getNode(i + 1, j + 1)->vertex);
            vertices.push_back(getNode(i + 1, j)->vertex);
            vertices.push_back(getNode(i, j + 1)->vertex);
        }
    }
    return vertices;
}



void Cloth::UpdateNormal(void)
{
    glm::vec3 normal(0.0, 0.0, 0.0);

    // reset normal
    for (MassNode* node : nodes) node->vertex.Normal = glm::vec3(0.0);

    for (int i = 0; i < nodesPerRow - 1; i++) {
        for (int j = 0; j < nodesPerColumn - 1; j++) {
            // Left upper triangle
            MassNode* node1 = getNode(i + 1, j);
            MassNode* node2 = getNode(i, j);
            MassNode* node3 = getNode(i, j + 1);

            normal = ComputeTriangleNormal(node1->vertex, node2->vertex, node3->vertex);

            node1->vertex.Normal += normal;
            node2->vertex.Normal += normal;
            node3->vertex.Normal += normal;

            // Right bottom triangle
            node1 = getNode(i + 1, j + 1);
            node2 = getNode(i + 1, j);
            node3 = getNode(i, j + 1);

            normal = ComputeTriangleNormal(node1->vertex, node2->vertex, node3->vertex);

            node1->vertex.Normal += normal;
            node2->vertex.Normal += normal;
            node3->vertex.Normal += normal;
        }
    }
}