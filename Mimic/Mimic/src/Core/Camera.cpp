#include "Camera.h"
#include <iostream>

Camera::Camera()
	: sensitivity(0.1f), deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), fov(45.0f),
	disabled(false)
{
	cameraPos = glm::vec3(5.0f, -5.0f, 30.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	lastX = 600;
	lastY = 500;

	yaw = -90.0f;
	pitch = 0.0f;
}



float Camera::getSensitivity()
{
	return sensitivity;
}



void Camera::setSensitivity(float sensitivity)
{
	this->sensitivity = sensitivity;
}



void Camera::setCameraSpeed(float speedModifier)
{
	this->cameraSpeed = speedModifier * deltaTime;
}



float Camera::getCameraSpeed()
{
	return cameraSpeed;
}

void Camera::disable()
{
	disabled = true;
}


void Camera::enable()
{
	disabled = false;
}


void Camera::updateCameraDirection(float currentX, float currentY)
{
	if (disabled) return;
	//if (!rightDown) return;

	// make sure camera doesn't suddenly move at the beginning
	if (firstMouse)
	{
		lastX = currentX;
		lastY = currentY;
		firstMouse = false;
	}

	// move camera direction when left button clicked
	//if (!leftDown) return;	

	calculateCameraAngle(currentX, currentY);

	// update last x and y to current x and y
	lastX = currentX;
	lastY = currentY;

	calculateCameraFront();
}





void Camera::setCameraKey(GLFWwindow* window)
{
	//if (disabled) return;
	setCameraSpeed(15.5f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;

}



void Camera::cameraUpdateFrameTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}



glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}



void Camera::calculateCameraFront()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}



void Camera::calculateCameraAngle(float currentX, float currentY)
{
	float offset_x = currentX - lastX;
	float offset_y = lastY - currentY;

	offset_x *= sensitivity;
	offset_y *= sensitivity;

	yaw += offset_x;
	pitch += offset_y;

	// avoid 90 degree
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}


void Camera::zoomIn(float yoffset)
{
	fov -= yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}



glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(fov), 1200.0f / 860.0f, 0.1f, 300.0f);
}



glm::vec3 Camera::getCameraPos() const
{
	/*std::cout << "camera pos: " <<
		"x: " << cameraPos.x <<
		"y: " << cameraPos.y <<
		"z: " << cameraPos.z <<
		std::endl; */
	return cameraPos;
}

glm::vec3 Camera::getCameraDir() const
{
	return cameraFront;
}



glm::mat4 Camera::getInverseViewMatrix()
{
	return glm::inverse(getViewMatrix());
}

glm::mat4 Camera::getInverseProjectionMatrix()
{
	return glm::inverse(getProjectionMatrix());
}



void Camera::Print() const
{
	std::cout << "Camera Position: " << cameraPos.x << ", " 
									 << cameraPos.y << ", " 
									 << cameraPos.z << std::endl;

	std::cout << "Camera Center: " << cameraFront.x + cameraPos.x << ", "
		<< cameraFront.y + cameraPos.y << ", "
		<< cameraFront.z + cameraPos.z << std::endl;

	std::cout << "Camera Up: " << cameraUp.x << ", "
		<< cameraUp.y << ", "
		<< cameraUp.z << std::endl;
}




void Camera::SetMouseStatus(int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		rightDown = (action == GLFW_PRESS);
	}
}