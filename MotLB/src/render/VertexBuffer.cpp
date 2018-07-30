/*
 * VertexBuffer.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include "VertexBuffer.h"

namespace render
{

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr size, GLenum usage)
: bufferSize(size), usage(usage)
  {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
  }

  VertexBuffer::~VertexBuffer()
  {
    glDeleteBuffers(1, &ID);
  }

  void VertexBuffer::bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
  }

  void VertexBuffer::unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void VertexBuffer::updateData(const void* data, GLsizeiptr size)
  {
    if (size > bufferSize)
    {
      glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }
    else
    {
      glBufferSubData(ID, 0, size, data);
    }
  }
} /* namespace render */

