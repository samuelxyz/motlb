/*
 * IndexBuffer.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include "IndexBuffer.h"

namespace render
{

  IndexBuffer::IndexBuffer(const void* data, GLsizeiptr size, GLenum usage)
  : bufferSize(size), usage(usage)
  {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
  }

  IndexBuffer::~IndexBuffer()
  {
  }

  void IndexBuffer::updateData(const void* data, GLsizeiptr size)
  {
    if (size > bufferSize)
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    }
    else
    {
      glBufferSubData(ID, 0, size, data);
    }
  }

  void IndexBuffer::bind()
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  }

  void IndexBuffer::unbind()
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

} /* namespace render */
