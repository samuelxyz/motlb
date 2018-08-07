/*
 * CenteredPolygonBatch.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: Samuel Tan
 */

#include "CenteredPolyBatch.h"
#include "Renderer.h"

namespace graphics
{

  CenteredPolyBatch::CenteredPolyBatch(unsigned int maxVertices,
      ShaderProgram& sp)
  : maxVertices(maxVertices), vertexCount(0), list(),
    shaderProgram(sp), vertexArray(),
    vertexBuffer(nullptr, maxVertices * FLOATS_PER_VERTEX * sizeof(float), GL_STREAM_DRAW),
    indexBuffer(nullptr, maxVertices * 3, GL_STREAM_DRAW)
  {
    vertexArray.addAttribute("color", GL_FLOAT, 4);
    vertexArray.addAttribute("position", GL_FLOAT, 2);
    vertexArray.applyAttributesWithBuffer(vertexBuffer, shaderProgram);

    list.reserve(maxVertices / 5); // probably good

    indexBuffer.forceBind();
  }

  CenteredPolyBatch::~CenteredPolyBatch()
  {
  }

  bool CenteredPolyBatch::add(const Values::CenteredPoly& cp)
  {
    if (vertexCount + cp.size() < maxVertices)
    {
      list.push_back(cp);
      vertexCount += cp.size();
      return true;
    }
    return false;
  }

  void CenteredPolyBatch::clearAll()
  {
    list.clear();
    vertexCount = 0;
  }

  // emulate GL_TRIANGLE_FAN sort of behavior but with one draw call
  void CenteredPolyBatch::renderAll()
  {
    // load CPs into buffers
    unsigned int iIndex = 0, vIndex = 0, iVertexNumber = 0;
    const unsigned int iLength = maxVertices * 3, vLength = maxVertices * FLOATS_PER_VERTEX;
    GLuint* iData = new GLuint[iLength];
    float* vData = new float[vLength];

    for (Values::CenteredPoly& cp : list)
    {
      assert(cp.size() >= 3);

      // do vertex buffer
      for (Values::ColoredVertex& vertex : cp)
      {
        vData[vIndex++] = vertex.color.r;
        vData[vIndex++] = vertex.color.g;
        vData[vIndex++] = vertex.color.b;
        vData[vIndex++] = vertex.color.a;

        vData[vIndex++] = vertex.x;
        vData[vIndex++] = vertex.y;
      }

      // do index buffer - this is the tricky part
      for (unsigned int cpVertex = 1; cpVertex < cp.size() - 1; ++cpVertex)
      {
        // triangle
        iData[iIndex++] = iVertexNumber; // central vertex
        iData[iIndex++] = iVertexNumber + cpVertex;
        iData[iIndex++] = iVertexNumber + cpVertex + 1;
      }

      // last triangle in index buffer
      iData[iIndex++] = iVertexNumber;                 // central vertex
      iData[iIndex++] = iVertexNumber + cp.size() - 1; // last vertex
      iData[iIndex++] = iVertexNumber + 1;             // first non-center vertex

      // prepare for next cp
      iVertexNumber += cp.size();
    }

    // fill the rest of the arrays with 0
    for (; vIndex < vLength; ++vIndex)
      vData[vIndex] = 0;
    for (; iIndex < iLength; ++iIndex)
      iData[iIndex] = 0;


    // load array data into GL buffer objects
    indexBuffer.updateData(iData, iLength);
    vertexBuffer.updateData(vData, vLength * sizeof(float));

    Renderer::draw(GL_TRIANGLES, vertexArray, indexBuffer, shaderProgram);

    delete[] iData;
    delete[] vData;

  } // render()

} /* namespace graphics */
