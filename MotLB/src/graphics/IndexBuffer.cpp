/*
 * IndexBuffer.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include "../graphics/IndexBuffer.h"

namespace graphics
{

  GLuint IndexBuffer::currentlyBound = 0;

  IndexBuffer::IndexBuffer(const GLuint* data, GLuint numIndices, GLenum usage)
  : numIndices(numIndices), usage(usage)
  {
    glGenBuffers(1, &ID);

    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), data, usage);
  }

  IndexBuffer::~IndexBuffer()
  {
    if (currentlyBound == ID)
      currentlyBound = 0;
    glDeleteBuffers(1, &ID);
  }

  void IndexBuffer::updateData(const GLuint* data, GLuint numIndices)
  {
    bind();

    if (numIndices > this->numIndices)
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), data, usage);
    }
    else
    {
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numIndices * sizeof(GLuint), data);
    }
  }

  void IndexBuffer::bind() const
  {
    if (currentlyBound != ID)
      forceBind();
  }

  void IndexBuffer::forceBind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    currentlyBound = ID;
  }

  void IndexBuffer::unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    currentlyBound = 0;
  }

} /* namespace render */

