#pragma once
#include <GL/glew.h>

class VertexBuffer
{
private:
	unsigned int ID;

public:
	// size is a unit in byte
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

