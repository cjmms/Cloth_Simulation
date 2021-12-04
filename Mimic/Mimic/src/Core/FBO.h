#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
* This file contains two type of FBOs, one for color buffer, one for depth buffer(depth only)
* There are more options to form a FBO(like both color buffer and depth buffer are textures)
* But it's suffice for now.
* 
* I didn't use templete nor inheritance to implement this file
* Because 2 classes with 2 different purposes seem easy to use, easy to understand.
* If it's necessary, templete or inheritance might be introduced for future use.
*/


class FBO_Color
{
private:
	unsigned int fbo;
	//unsigned int colorBuffer;
	//unsigned int depthBuffer;

public:
	/*
	* Generate a FBO, buffer size = width * height
	* color buffer is a texture, depth buffer and stencil buffer are renderbuffer
	*/
	FBO_Color(unsigned int width, unsigned int height);

	inline void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo); };
	inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

	inline unsigned int getColorAttachment() const { return colorBuffer; };

	unsigned int depthBuffer;
	unsigned int colorBuffer;
};




class FBO_Depth
{
private:
	unsigned int fbo;
	unsigned int depthBuffer;

public:
	FBO_Depth(unsigned int width, unsigned int height);

	inline void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo); };
	inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

	inline unsigned int getDepthAttachment() const { return depthBuffer; };
};
