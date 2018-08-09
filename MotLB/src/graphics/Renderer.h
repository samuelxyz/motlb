/*
 * Renderer.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>
#include <ShaderProgram.h>
#include <TriangleBatch.h>

#include "../Values.h"

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

      void addTriangle(const Values::Triangle&);
      void addQuad(const Values::Quad&);
      void addCenteredPoly(const Values::CenteredPoly&);

    private:
      ShaderProgram shaderProgram;
//      CenteredPolyBatch cpBatch;
//      QuadBatch quadBatch;
      TriangleBatch tBatch;
  };

} /* namespace render */

#endif /* RENDERER_H_ */
