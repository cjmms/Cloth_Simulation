#include "scene.h"

#include <GL/glew.h>
#include <iostream>
#include "Shader.h"
#include "Cube.h"


void scene::Init(unsigned int width, unsigned int height)
{
	CalculateProjView(width, height);
	shader = new Shader("./res/shaders/basicShader");

	// init scene obj
	// target cube
	targetCube = new Cube(glm::vec3(1), glm::vec3(5.0f, 0.0f, 0.0f));

	int n = 5;	// number of cubes the scene has

	// create the list of cubes
	// push these cubes into a vector
	for (int i = 0; i < n; i++)
		cubearray.push_back(new Cube(glm::vec3(1, 1, 3), glm::vec3(0, 0.0f, 4.0f)));	// Cube(scale, translate)
}


void scene::drawAxis() {
	glBegin(GL_LINE_STRIP);
	glVertex3f(-9.0f, 0.0f, 0.0f-1.0f);
	glVertex3f(9.0f, 0.0f, 0.0f - 1.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f, 0.0f, -9.0f- 0.0f);
	glVertex3f(0.0f, 0.0f, 9.0f - 0.0f);
	glEnd();
}


scene::~scene()
{
	delete shader;
}


std::vector<glm::mat4> scene::draw() {
	std::vector<glm::mat4> box;
	
	this->m = glm::rotate(-90.0f, glm::vec3(1, 0, 0))*glm::rotate(rotate_x, glm::vec3(0, 0, 1))*glm::rotate(rotate_y, glm::vec3(1, 0, 0));
	
	RenderAxis();	// background

	RenderTargetCube();	// target cube
	
	// render the list of cubes
	glm::mat4 t1rt0 = glm::translate(glm::mat4(), glm::vec3(0, 0, -3));
	for (unsigned int i = 0; i < cubearray.size(); i++) {

		glm::mat4 cuberotate = cubearray.at(i)->getrotate();
		glm::mat4 cubescale = cubearray.at(i)->getscale();
		glm::mat4 cubetranslate = cubearray.at(i)->getTranslate();
		glm::mat4 cubetheta = cubearray.at(i)->gettheta();
		glm::mat4 cubepsi = cubearray.at(i)->getpsi();
		glm::mat4 cubephi = cubearray.at(i)->getphi();
		t1rt0 = t1rt0*glm::translate(glm::mat4(), glm::vec3(0, 0, -2))* cubetranslate *cuberotate*cubepsi*cubetheta*glm::translate(glm::mat4(),glm::vec3(0,0,2));
		
		glm::mat4 MVP = ProjView *m*t1rt0*cubescale;
		box.push_back(t1rt0*cubescale);
		shader->Bind();
		shader->Update(MVP);
	
		cubearray.at(i)->Draw();
	}
	return box;	// return the model matrix of all nodes
}



// calculate projection matrix * view matrix
// this is one time calculation, since camera never moves
void scene::CalculateProjView(unsigned int width, unsigned int height)
{
	glm::vec3 pos = glm::vec3(0, 10, -30);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	ProjView = glm::perspective(60.0f, (float)width / (float)height, 0.1f, 100.0f);
	ProjView = ProjView * glm::lookAt(pos, pos + forward, up);
}



void scene::RenderTargetCube()
{
	glm::mat4 scale = targetCube->getscale();
	glm::mat4 translate = targetCube->getTranslate();

	glm::mat4 bigphi = targetCube->getphi();
	glm::mat4 bigtheta = targetCube->gettheta();
	glm::mat4 bigpsi = targetCube->getpsi();
	glm::mat4 MVP = ProjView * m * translate * bigphi * bigtheta * bigpsi * scale;
	// update targetcube position
	this->cubepos = translate * bigphi * bigtheta * bigpsi;

	shader->Bind();	// bind shader
	shader->Update(MVP);	// pass uniform

	targetCube->Draw();	// render
}


void scene::RenderAxis()
{
	glm::mat4 mvp = ProjView * glm::rotate(-90.0f, glm::vec3(1, 0, 0));
	shader->Bind();
	shader->Update(mvp);
	drawAxis();
}