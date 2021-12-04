#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


class Camera
{
private:
	// camera direction = cameraFront - cameraPos
	glm::vec3 cameraPos, cameraFront, cameraUp;

	// delta time: time difference from last frame to current frame
	// last Frame: time of last frame
	float deltaTime, lastFrame;

	// x and y position from last frame
	float lastX, lastY;
	float yaw, pitch;

	float sensitivity;
	float cameraSpeed;

	// flag, test if it's the first time detects mouse input 
	bool firstMouse;

	float fov;


	bool disabled;


public:
	Camera();
	float getSensitivity();
	void setSensitivity(float sensitivity);
	void setCameraSpeed(float speed);
	float getCameraSpeed();



	glm::vec3 getCameraPos() const;
	glm::vec3 getCameraDir() const;

	void disable();
	void enable();


	// update cameraFrontS base on current x and y
	void updateCameraDirection(float currentX, float currentY);

	// WASD 
	// W: forward
	// S: backward
	// A: move left
	// D: move right
	void setCameraKey(GLFWwindow* window);

	// update delta time and last frame
	void cameraUpdateFrameTime();


	// using lookat() to calculate view matrix
	glm::mat4 getViewMatrix();
	glm::mat4 getInverseViewMatrix();

	// zoom in camera base on scroller offset
	void zoomIn(float yoffset);

	// using perspective() to calculate projection matrix
	glm::mat4 getProjectionMatrix();
	glm::mat4 getInverseProjectionMatrix();

	void Print() const;


	void SetMouseStatus(int button, int action);


private:
	// calculate and normalize camera front vector base on yaw and pitch
	void calculateCameraFront();

	// update yaw and pitch base on x, y difference
	// pitch value can't be 90 degree
	void calculateCameraAngle(float currentX, float currentY);

	bool rightDown;
};

