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
  {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
  }

  VertexBuffer::~VertexBuffer()
  {
  }

} /* namespace render */
