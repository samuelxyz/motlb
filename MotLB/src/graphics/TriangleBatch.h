/*
 * TriangleBatch.h
 *
 *  Created on: Aug 6, 2018
 *      Author: Samuel Tan
 */

#ifndef TRIANGLEBATCH_H_
#define TRIANGLEBATCH_H_

#include <IndexBuffer.h>
#include <ShaderProgram.h>
#include <VertexArray.h>
#include <VertexBuffer.h>

#include "../Values.h"

namespace graphics
{

  class TriangleBatch
  {
    public:
      static constexpr unsigned short FLOATS_PER_QUAD = 24;
      static constexpr unsigned short FLOATS_PER_VERTEX = 6;

    private:
      const unsigned int maxVertices;
      ShaderProgram& shaderProgram;
      VertexArray vertexArray;
      VertexBuffer vertexBuffer;
      IndexBuffer indexBuffer;

      unsigned int iIndex, vIndex, vertexCount;
      const unsigned int iLength, vLength;
      GLuint* iData;
      float* vData;

    public:
      TriangleBatch(unsigned int maxVertices, ShaderProgram& sp);
      ~TriangleBatch();

      bool addTriangle(const Values::Triangle&);
      bool addQuad(const Values::Quad&);
      bool addCenteredPoly(const Values::CenteredPoly&);

      void renderAndClearAll();
  };

} /* namespace graphics */

#endif /* TRIANGLEBATCH_H_ */
