#pragma once

class MassNode;

class Spring
{
public:
	Spring(MassNode* node1, MassNode *node2, float hookC, float dampC);
	void Update(float timeStamp);	// simulate force, apply force to 2 nodes 

public:
	MassNode* node1;
	MassNode* node2;

	float restLength;	
	float hookC;		// hook coefficient
	float dampC;		// dump coefficient
};