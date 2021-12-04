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

	// Hooke's coefficients
	float structuralCoef = 2000.0;
	float shearCoef = 50.0;
	float bendingCoef = 400.0;

	// damping coefficient
	float dampCoef = 2.0;

private:

	// init helper functions
	//-----------------------------------------//
	void CreateMassNode(int rowIndex, int ColumnIndex);
	void CreateSprings(int rowIndex, int ColumnIndex);

	//-----------------------------------------//

	inline MassNode* getNode(int x, int y) { return nodes[y * nodesPerRow + x]; }

	void SimulateGravity(void);
	void SimulateInternalForce(float timeStamp);	// damp + Hooke

public:
	Cloth(glm::vec3 position, int width, int height, int nodesDensity);
	void Simulate(float timeStamp);

	std::vector<Vertex>& GetVertices();
	std::vector<Vertex>& GetTriangles();
};