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
      GLuint ID, numIndices;
      GLenum usage;

    public:
      IndexBuffer(const GLuint* data, GLuint numIndices, GLenum usage);
      ~IndexBuffer();

      void updateData(const GLuint* data, GLuint numIndices);
      void bind() const;
      void unbind() const;
      GLuint getNumIndices() const { return numIndices; }
  };

} /* namespace render */

#endif /* INDEXBUFFER_H_ */
