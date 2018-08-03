/*
 * ColoredBoxBatch.cpp
 *
 *  Created on: Jul 31, 2018
 *      Author: Samuel Tan
 */

#include "QuadBatch.h"
#include "Box.h"
#include "Renderer.h"

namespace graphics
{

  QuadBatch::QuadBatch(unsigned int maxQuads, ShaderProgram& sp)
  : maxQuads(maxQuads), quads(),
    shaderProgram(sp), vertexArray(),
    vertexBuffer(nullptr, maxQuads * FLOATS_PER_QUAD * sizeof(float), GL_DYNAMIC_DRAW),
    indexBuffer(nullptr, maxQuads * 6, GL_DYNAMIC_DRAW)
  {
    vertexArray.addAttribute("color", GL_FLOAT, 4);
    vertexArray.addAttribute("position", GL_FLOAT, 2);
    vertexArray.applyAttributesWithBuffer(vertexBuffer, shaderProgram);

    quads.reserve(maxQuads);

    indexBuffer.forceBind();
  }

  QuadBatch::~QuadBatch()
  {
  }

  bool QuadBatch::add(Values::Quad quad)
  {
    if (quads.size() < maxQuads)
    {
      quads.push_back(quad);
      return true;
    }
    return false;
  }

  void QuadBatch::clearAll()
  {
    quads.clear();
  }

  void QuadBatch::renderAll()
  {
    // load quads into buffers
    unsigned int iIndex = 0, vIndex = 0, iVertexElement = 0;
    const unsigned int iLength = maxQuads * 6, vLength = maxQuads * FLOATS_PER_QUAD;
    GLuint* iData = new GLuint[iLength];
    float* vData = new float[vLength];

    for (Values::Quad& quad : quads)
    {

      // do vertex buffer, four vertices
      for (Values::ColoredVertex& vertex : quad)
      {
        vData[vIndex++] = vertex.color.r;
        vData[vIndex++] = vertex.color.g;
        vData[vIndex++] = vertex.color.b;
        vData[vIndex++] = vertex.color.a;

        vData[vIndex++] = vertex.x;
        vData[vIndex++] = vertex.y;
      }

      // do index buffer, two triangles  //    absCorners:
      iData[iIndex++] = iVertexElement + 0;    //    1---------0
      iData[iIndex++] = iVertexElement + 1;    //    |         |
      iData[iIndex++] = iVertexElement + 3;    //    |         |
      iData[iIndex++] = iVertexElement + 1;    //    |         |
      iData[iIndex++] = iVertexElement + 3;    //    |         |
      iData[iIndex++] = iVertexElement + 2;    //    2---------3
      iVertexElement += 4;                     //
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
