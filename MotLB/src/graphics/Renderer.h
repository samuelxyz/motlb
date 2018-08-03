/*
 * Renderer.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>
#include <QuadBatch.h>

#include "../graphics/ShaderProgram.h"

namespace graphics
{
  class IndexBuffer;
  class VertexArray;
} /* namespace render */

namespace graphics
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

      void addQuad(const Values::Quad&);

    private:
      ShaderProgram quadShader;
      QuadBatch quadBatch;
  };

} /* namespace render */

#endif /* RENDERER_H_ */
