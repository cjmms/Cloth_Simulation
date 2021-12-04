#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Cube.h"
#include "scene.h"



using namespace glm;

GLFWwindow* m_window;
bool spaceKey=false;
int startindex = 0;
const double PI = 3.141592653589793238463;
static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 800;

scene s;

std::vector<glm::mat4> box;




void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		glm::mat4 b = box[0];
		glm::mat4 cube = s.cubepos;
		cube = glm::translate(cube, vec3(-2, 0, -1));
		glm::vec4 p1 = b*glm::vec4(1, 1, 1, 1);
		glm::vec4 p2 = cube*glm::vec4(1, 1, 1, 1);
		float distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));

		if (distance > -1+4 * s.cubearray.size())
			std::cout << "cant reach" << std::endl;	
		
		spaceKey = !spaceKey;
	}

	// close the window
	if( GLFW_KEY_ESCAPE == key && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	glm::mat4 oldTranslation = s.targetCube->getTranslate();

	// control of target cube
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)	
		s.targetCube->setTranslate(glm::translate(oldTranslation, glm::vec3(0, -0.2, 0)));
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		s.targetCube->setTranslate(glm::translate(oldTranslation, glm::vec3(0, 0.2, 0)));
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		s.targetCube->setTranslate(glm::translate(oldTranslation, glm::vec3(0.2, 0.0, 0)));
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		s.targetCube->setTranslate(glm::translate(oldTranslation, glm::vec3(-0.2, 0, 0)));
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		s.targetCube->setTranslate(glm::translate(oldTranslation, glm::vec3(0, 0, 0.2)));
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		s.targetCube->setTranslate(glm::translate(oldTranslation, glm::vec3(0, 0, -0.2)));
}




void CCD() {
	// calculate top most cube position
	glm::mat4 topCubeModelMatrix = box[s.cubearray.size() - 1];
	glm::vec4 topCubePos = topCubeModelMatrix * glm::vec4(1, 1, 1, 1);

	// // calculate target cube position
	glm::mat4 targetModelMatrix = s.cubepos;
	glm::vec4 targetPos = targetModelMatrix * glm::vec4(1, 1, 1, 1);

	// distance
	float distance = sqrt(pow(topCubePos.x - targetPos.x, 2) + pow(topCubePos.y - targetPos.y, 2) + pow(topCubePos.z - targetPos.z, 2));
	
	// check if target is reached
	if (distance > 0.01)
	{
		// get current cube position
		glm::vec4 thisbox = glm::translate(box[startindex], vec3(0, 0, -2))* vec4(1, 1, 1, 1);


		// vector start from 
		glm::vec4 ej = topCubePos - thisbox;//e-j
		glm::vec4 tj = targetPos - thisbox;

		glm::vec4 normalizedej = glm::normalize(ej);
		glm::vec4 normalizedtj = glm::normalize(tj);


		float dot =glm::dot(vec3(normalizedej), vec3(normalizedtj));
		glm::vec3 c = glm::normalize(glm::cross(vec3(normalizedej), vec3(normalizedtj)));
		float cosangle = glm::clamp(dot, -1.0f, 1.0f);
		float angle = glm::acos(cosangle) * 180 / (30 * PI);
		glm::mat4 rot = s.cubearray.at(startindex)->getrotate();

		s.cubearray.at(startindex)->setrotate(glm::rotate(rot, angle, c));
	}
	

	// move to next cube
	startindex -= 1;

	// if reach the bottom of the cube
	// startover from the beginning
	if (startindex < 0) startindex = s.cubearray.size() - 1;
}


bool InitGLFW()
{
	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "GLFW failed to init." << std::endl;
		return false;
	}

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "IK", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		std::cout << "GLFW window failed to init." << std::endl;
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);

	glfwSetKeyCallback(m_window, key_callback);
	return true;
}

bool InitGLEW()
{
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "GLEW init error" << std::endl;
		return false;
	}

	// enable depth test
	glEnable(GL_DEPTH_TEST);
	return true;
}


int main(int argc, char** argv)
{
	InitGLFW();

	InitGLEW();	

	s.Init(DISPLAY_WIDTH, DISPLAY_HEIGHT);

	// game loop
	while(!glfwWindowShouldClose(m_window))
	{
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (spaceKey) CCD();
		
		box=s.draw();	// render the scene
		

		glfwSwapBuffers(m_window);// swap buffer
		glfwPollEvents();
	}

	// destroy window, terminate glfw
	glfwDestroyWindow(m_window);
	glfwTerminate();
	
	return 0;
}