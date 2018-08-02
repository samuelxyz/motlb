/*
 * Renderer.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>

#include "ColoredBoxBatch.h"
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

      static void clearScreen();
      static void draw(GLenum mode, const VertexArray& va,
          const IndexBuffer& ib, const ShaderProgram& sp);

      void renderAndClearAll();

      void addColoredBox(const Values::Color& color, Box& box);

    private:
      ShaderProgram cbbShader;
      ColoredBoxBatch cbb;
  };

} /* namespace render */

#endif /* RENDERER_H_ */
