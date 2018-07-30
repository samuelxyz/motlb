/*
 * IndexBuffer.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_

#include <GL/glew.h>

namespace render
{

  class IndexBuffer
  {
    private:
      GLuint ID;
      GLsizeiptr bufferSize;
      GLenum usage;

    public:
      IndexBuffer(const void* data, GLsizeiptr size, GLenum usage);
      ~IndexBuffer();

      void updateData(const void* data, GLsizeiptr size);
      void bind();
      void unbind();
  };

} /* namespace render */

#endif /* INDEXBUFFER_H_ */
