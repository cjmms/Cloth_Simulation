#pragma once
#include <GL/glew.h>

class IndexBuffer
{
private:
	unsigned int ID;
	unsigned int m_count;

public:
	// size is a unit in byte
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_count; }
};

