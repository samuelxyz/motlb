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

    public:
      VertexBuffer(const void* data, GLsizeiptr size, GLenum usage);
      virtual ~VertexBuffer();
  };

} /* namespace render */

#endif /* VERTEXBUFFER_H_ */
