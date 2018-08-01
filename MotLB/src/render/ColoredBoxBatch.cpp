/*
 * ColoredBoxBatch.cpp
 *
 *  Created on: Jul 31, 2018
 *      Author: Samuel Tan
 */

#include "ColoredBoxBatch.h"

#include "../Box.h"
#include "Renderer.h"

namespace render
{

  ColoredBoxBatch::ColoredBoxBatch(unsigned int maxBoxes, ShaderProgram& sp)
  : maxBoxes(maxBoxes), boxes(),
    shaderProgram(sp), vertexArray(),
    vertexBuffer(nullptr, maxBoxes * FLOATS_PER_BOX * sizeof(float), GL_DYNAMIC_DRAW),
    indexBuffer(nullptr, maxBoxes * 6, GL_DYNAMIC_DRAW)
  {
    vertexArray.addAttribute("color", GL_FLOAT, 4);
    vertexArray.addAttribute("position", GL_FLOAT, 2);
    vertexArray.applyAttributesWithBuffer(vertexBuffer, shaderProgram);

    boxes.reserve(maxBoxes);
  }

  ColoredBoxBatch::~ColoredBoxBatch()
  {
  }

  void ColoredBoxBatch::add(ColoredBox cbox)
  {
    boxes.push_back(cbox);
  }

  void ColoredBoxBatch::clear()
  {
    boxes.clear();
  }

  void ColoredBoxBatch::render()
  {
    // load boxes into buffers
    unsigned int iIndex = 0, vIndex = 0;
    const unsigned int iLength = maxBoxes * 6, vLength = maxBoxes * FLOATS_PER_BOX;
    GLuint* iData = new GLuint[iLength];
    float* vData = new float[vLength];

    for (ColoredBox& cb : boxes)
    {
      std::array<Vec2, 4> boxCorners;
      cb.box.absCorners(boxCorners);

      // do vertex buffer, four vertices
      for (Vec2& v : boxCorners)
      {
        vData[vIndex++] = cb.r;
        vData[vIndex++] = cb.g;
        vData[vIndex++] = cb.b;
        vData[vIndex++] = cb.a;

        vData[vIndex++] = static_cast<float>(v.getX());
        vData[vIndex++] = static_cast<float>(v.getY());
      }

      // do index buffer, two triangles  //    absCorners:
      iData[iIndex + 0] = iIndex + 0;    //    1---------0
      iData[iIndex + 1] = iIndex + 1;    //    |         |
      iData[iIndex + 2] = iIndex + 3;    //    |         |
      iData[iIndex + 3] = iIndex + 1;    //    |         |
      iData[iIndex + 4] = iIndex + 3;    //    |         |
      iData[iIndex + 5] = iIndex + 2;    //    2---------3
      iIndex += 6;                       //
    }

    // fill the rest of the arrays with 0
    for (; vIndex < vLength; ++vIndex)
      vData[vIndex] = 0;
    for (; iIndex < iLength; ++iIndex)
      iData[iIndex] = 0;


    // load buffers into GL buffer objects
    indexBuffer.updateData(iData, iLength);
    vertexBuffer.updateData(vData, vLength * sizeof(float));

    Renderer::draw(GL_TRIANGLES, vertexArray, indexBuffer, shaderProgram);

    delete[] iData;
    delete[] vData;

  } // render()

} /* namespace render */
