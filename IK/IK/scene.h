#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "shader.h"

class Cube;


class scene
{
private:
	// since camera never moves, projview will not change
	glm::mat4 ProjView;

	// only 1 shader for all obj
	Shader *shader;
	
	void RenderTargetCube();
	void RenderAxis();

public:
	glm::mat4 m = glm::mat4();
	float rotate_y = 0;
	float rotate_x = 0;
	glm::mat4 cubepos;

	std::vector<Cube*> cubearray;		// all cubes except the target
	Cube* targetCube;					// target
	

	~scene();

	void Init(unsigned int width, unsigned int height);
	
	std::vector<glm::mat4> draw();
	void drawAxis();

	void CalculateProjView(unsigned int width, unsigned int height);
};