/*
 * ColoredBoxBatch.h
 *
 *  Created on: Jul 31, 2018
 *      Author: Samuel Tan
 */

#ifndef COLOREDBOXBATCH_H_
#define COLOREDBOXBATCH_H_

#include <vector>

#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class Box;

namespace render
{

  class ColoredBoxBatch
  { // a component for more efficiently storing and drawing lots of boxes
    public:

      static constexpr unsigned short FLOATS_PER_BOX = 24;
      // ( 4 floats for color + 2 floats per position ) * 4 vertices per box

      struct ColoredBox
      {
        float r, g, b, a;
        Box& box;
      };

    private:

      const unsigned int maxBoxes;
      std::vector<ColoredBox> boxes;
      ShaderProgram& shaderProgram;
      VertexArray vertexArray;
      VertexBuffer vertexBuffer;
      IndexBuffer indexBuffer;

    public:
      ColoredBoxBatch(unsigned int maxBoxes, ShaderProgram& sp);
      ~ColoredBoxBatch();

      bool add(ColoredBox);
      void clearAll();

      void renderAll();
  };

} /* namespace render */

#endif /* COLOREDBOXBATCH_H_ */
