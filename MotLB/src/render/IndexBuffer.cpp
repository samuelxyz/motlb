/*
 * IndexBuffer.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include "IndexBuffer.h"

namespace render
{

  IndexBuffer::IndexBuffer(const GLuint* data, GLuint numIndices, GLenum usage)
  : numIndices(numIndices), usage(usage)
  {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), data, usage);
  }

  IndexBuffer::~IndexBuffer()
  {
  }

  void IndexBuffer::updateData(const GLuint* data, GLuint numIndices)
  {
    if (numIndices > this->numIndices)
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), data, usage);
    }
    else
    {
      glBufferSubData(ID, 0, numIndices * sizeof(GLuint), data);
    }
  }

  void IndexBuffer::bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  }

  void IndexBuffer::unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

} /* namespace render */
