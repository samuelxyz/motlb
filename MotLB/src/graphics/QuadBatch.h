/*
 * ColoredBoxBatch.h
 *
 *  Created on: Jul 31, 2018
 *      Author: Samuel Tan
 */

#ifndef QUADBATCH_H_
#define QUADBATCH_H_

#include <IndexBuffer.h>
#include <ShaderProgram.h>
#include <Values.h>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <vector>


namespace geometry {
  class Box;
}

namespace graphics
{

  class QuadBatch
  { // a component for more efficiently storing and drawing lots of quads
    public:

      static constexpr unsigned short FLOATS_PER_QUAD = 24;
      // ( 4 floats for color + 2 floats per position ) * 4 vertices per quad

    private:

      const unsigned int maxQuads;
      std::vector<Values::Quad> quads;
      ShaderProgram& shaderProgram;
      VertexArray vertexArray;
      VertexBuffer vertexBuffer;
      IndexBuffer indexBuffer;

    public:
      QuadBatch(unsigned int maxBoxes, ShaderProgram& sp);
      ~QuadBatch();

      bool add(const Values::Quad&);
      void clearAll();

      void renderAll();
  };

} /* namespace render */

#endif /* QUADBATCH_H_ */
