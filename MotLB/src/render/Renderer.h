/*
 * Renderer.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "ShaderProgram.h"

namespace render
{
  class IndexBuffer;
  class VertexArray;
} /* namespace render */

namespace render
{

  class Renderer
  {
    public:
      Renderer();
      ~Renderer();

      static void clear();
      static void draw(const VertexArray& va, const IndexBuffer& ib, const ShaderProgram& sp);
  };

} /* namespace render */

#endif /* RENDERER_H_ */
