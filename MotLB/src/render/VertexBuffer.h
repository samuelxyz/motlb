/*
 * VertexBuffer.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <GL/glew.h>

namespace render
{

  class VertexBuffer
  {
    private:
      GLuint ID;
      GLsizeiptr bufferSize;
      GLenum usage;

    public:
      VertexBuffer(const void* data, GLsizeiptr size, GLenum usage);
      ~VertexBuffer();

      void updateData(const void* data, GLsizeiptr size);
      void bind();
      void unbind();
  };

} /* namespace render */

#endif /* VERTEXBUFFER_H_ */
