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
	float dampCoef = 1.0;

	glm::vec3 gravity = glm::vec3(0, -0.98, 0);

private:

	// init helper functions
	//-----------------------------------------//
	void CreateMassNode(int rowIndex, int ColumnIndex);
	void CreateSprings(int rowIndex, int ColumnIndex);
	//-----------------------------------------//

	inline MassNode* getNode(int x, int y) { return nodes[y * nodesPerRow + x]; }

	inline glm::vec3 ComputeTriangleNormal(Vertex &n1, Vertex& n2, Vertex& n3) const
	{ return glm::cross(n2.Position - n1.Position, n3.Position - n1.Position); }

	

	

public:
	Cloth(glm::vec3 position, int width, int height, int nodesDensity);

	std::vector<Vertex>& GetTriangles();

	// simulation functions
	inline void SimulateNodes(float timeStamp) { for (MassNode* node : nodes) node->Simulate(timeStamp); }
	
	inline void SimulateInternalForce(float timeStamp) { for (Spring* spring : springs)  spring->Simulate(timeStamp); }

	inline void SimulateWind(glm::vec3 windDir) { for (MassNode* node : nodes) node->force += (windDir * (float)node->mass); }

	void SimulateGravity(void) { for (MassNode* node : nodes) node->force += (gravity * node->mass); }

	void UpdateNormal(void);
};