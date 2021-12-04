#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Core/Camera.h"

extern Camera camera;

class UI_Manager
{
public:

	bool init();
	void update();

	void close();

	void disableCursor();
	void enableCursor();

	//inline const Camera& getCamera() { return camera; }

	void setScreenSize(unsigned int window_width, unsigned int window_height);
	
	inline unsigned int getScreenHeight() const { return window_height; }
	inline unsigned int getScreenWidth() const { return window_width; }

	inline bool windowClosed() { return glfwWindowShouldClose(window); }

	inline GLFWwindow* getWindow() { return window; }

	void NewUIFrame();
	void RenderUI();

private:
	unsigned int window_width, window_height;
	GLFWwindow* window;

};

