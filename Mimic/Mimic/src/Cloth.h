#pragma once

#include "Spring.h"
#include "MassNode.h"
#include <vector>

class Cloth
{
public:
	glm::vec3 position;
	int width, height;
	int nodesPerRow, nodesPerColumn;
	int nodesDensity;

	std::vector<MassNode*> nodes;
	std::vector<Spring*> springs;
	std::vector<Vertex> vertices;


public:
	Cloth(glm::vec3 position, int width, int height, int nodesDensity);

	// init helper functions
	//-----------------------------------------//
	MassNode* CreateMassNode(int rowIndex, int ColumnIndex);
	void InitMassNodes(void);

	void InitSprings(void);

	//-----------------------------------------//

	std::vector<Vertex>& GetVertices();
};