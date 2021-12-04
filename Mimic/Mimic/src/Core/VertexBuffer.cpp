#include "VertexBuffer.h"



VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &ID);
}


void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}


void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}