/*
 * CenteredPolygonBatch.h
 *
 *  Created on: Aug 3, 2018
 *      Author: Samuel Tan
 */

#ifndef CENTEREDPOLYBATCH_H_
#define CENTEREDPOLYBATCH_H_

#include <IndexBuffer.h>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <vector>

#include "../Values.h"

namespace graphics
{

  class CenteredPolyBatch
  { // a component for more efficiently storing and drawing lots of CPs
    public:

          static constexpr unsigned short FLOATS_PER_VERTEX = 6;
          // ( 4 floats for color + 2 floats per position )

        private:

          const unsigned int maxVertices;
          unsigned int vertexCount;
          std::vector<Values::CenteredPoly> list;
          ShaderProgram& shaderProgram;
          VertexArray vertexArray;
          VertexBuffer vertexBuffer;
          IndexBuffer indexBuffer;

        public:
          CenteredPolyBatch(unsigned int maxVertices, ShaderProgram& sp);
          ~CenteredPolyBatch();

          bool add(const Values::CenteredPoly&);
          void clearAll();

          void renderAll();
  };

} /* namespace graphics */

#endif /* CENTEREDPOLYBATCH_H_ */
