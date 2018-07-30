/*
 * VertexArray.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef VERTEXARRAY_H_
#define VERTEXARRAY_H_

#include "ShaderProgram.h"
#include "VertexBuffer.h"

#include <GL/glew.h>
#include <string>
#include <vector>

namespace render
{

  class VertexArray
  {
    public:

      struct Attribute
      {
        std::string varName;
        GLenum dataType;
        GLuint numComponents;
      };

    private:
      std::vector<Attribute> attributes;
      GLuint ID, stride;

    public:
      VertexArray();
      ~VertexArray();

      void addAttribute(std::string varName, GLenum dataType, GLuint numComponents);
      void applyAttributesWithBuffer(render::VertexBuffer& vb, render::ShaderProgram& sp);

      void bind();
      void unbind();

    private:
      GLsizei getSizeOfType(GLenum dataType);
  };

} /* namespace render */

#endif /* VERTEXARRAY_H_ */
