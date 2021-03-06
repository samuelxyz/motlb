/*
 * TriangleBatch.cpp
 *
 *  Created on: Aug 6, 2018
 *      Author: Samuel Tan
 */

#include <TriangleBatch.h>
#include "Renderer.h"

namespace graphics
{

  TriangleBatch::TriangleBatch(unsigned int maxVertices,
      ShaderProgram& sp)
  : maxVertices(maxVertices),
    shaderProgram(sp), vertexArray(),
    vertexBuffer(nullptr, maxVertices * FLOATS_PER_VERTEX * sizeof(float), GL_STREAM_DRAW),
    indexBuffer(nullptr, maxVertices * 3, GL_STREAM_DRAW),
    iIndex(0), vIndex(0), vertexCount(0),
    iLength(maxVertices * 3), vLength(maxVertices * FLOATS_PER_VERTEX),
    iData(new GLuint[iLength]), vData(new float[vLength])
  {
    vertexArray.addAttribute("color", GL_FLOAT, 4);
    vertexArray.addAttribute("position", GL_FLOAT, 3);
    vertexArray.applyAttributesWithBuffer(vertexBuffer, shaderProgram);

    indexBuffer.forceBind();
  }

  TriangleBatch::~TriangleBatch()
  {
    delete[] iData;
    delete[] vData;
  }

  bool TriangleBatch::addTriangle(const Values::Triangle& tri)
  {
    if (vertexCount + 3 > maxVertices)
      return false;

    for (const Values::ColoredVertex& vertex : tri)
    {
      vData[vIndex++] = vertex.color.r;
      vData[vIndex++] = vertex.color.g;
      vData[vIndex++] = vertex.color.b;
      vData[vIndex++] = vertex.color.a;

      vData[vIndex++] = vertex.x;
      vData[vIndex++] = vertex.y;
      vData[vIndex++] = vertex.z;
    }

    iData[iIndex++] = vertexCount + 0;
    iData[iIndex++] = vertexCount + 1;
    iData[iIndex++] = vertexCount + 2;

    vertexCount += 3;

    return true;
  }

  bool TriangleBatch::addQuad(const Values::Quad& quad)
  {
    if (vertexCount + 4 > maxVertices)
      return false;

    // do vertex buffer, four vertices
    for (const Values::ColoredVertex& vertex : quad)
    {
      vData[vIndex++] = vertex.color.r;
      vData[vIndex++] = vertex.color.g;
      vData[vIndex++] = vertex.color.b;
      vData[vIndex++] = vertex.color.a;

      vData[vIndex++] = vertex.x;
      vData[vIndex++] = vertex.y;
      vData[vIndex++] = vertex.z;
    }

    // do index buffer, two triangles
    iData[iIndex++] = vertexCount + 0;    //    1---------0
    iData[iIndex++] = vertexCount + 1;    //    |         |
    iData[iIndex++] = vertexCount + 3;    //    |         |
    iData[iIndex++] = vertexCount + 1;    //    |         |
    iData[iIndex++] = vertexCount + 3;    //    |         |
    iData[iIndex++] = vertexCount + 2;    //    2---------3

    vertexCount += 4;

    return true;
  }

  bool TriangleBatch::addCenteredPoly(const Values::CenteredPoly& cp)
  {

    if (vertexCount + cp.size() > maxVertices)
      return false;

    assert(cp.size() >= 3);

    // do vertex buffer
    for (const Values::ColoredVertex& vertex : cp)
    {
      vData[vIndex++] = vertex.color.r;
      vData[vIndex++] = vertex.color.g;
      vData[vIndex++] = vertex.color.b;
      vData[vIndex++] = vertex.color.a;

      vData[vIndex++] = vertex.x;
      vData[vIndex++] = vertex.y;
      vData[vIndex++] = vertex.z;
    }

    // do index buffer - this is the tricky part
    for (unsigned int cpVertex = 1; cpVertex < cp.size() - 1; ++cpVertex)
    {
      // triangle
      iData[iIndex++] = vertexCount; // central vertex
      iData[iIndex++] = vertexCount + cpVertex;
      iData[iIndex++] = vertexCount + cpVertex + 1;
    }

    // last triangle in index buffer
    iData[iIndex++] = vertexCount;                 // central vertex
    iData[iIndex++] = vertexCount + cp.size() - 1; // last vertex
    iData[iIndex++] = vertexCount + 1;             // first non-center vertex

    // prepare for next cp
    vertexCount += cp.size();

    return true;
  }

  void TriangleBatch::renderAndClearAll()
  {
    // fill the rest of the arrays with 0
    for (; vIndex < vLength; ++vIndex)
      vData[vIndex] = 0;
    for (; iIndex < iLength; ++iIndex)
      iData[iIndex] = 0;


    // load array data into GL buffer objects
    indexBuffer.updateData(iData, iLength);
    vertexBuffer.updateData(vData, vLength * sizeof(float));

    Renderer::draw(GL_TRIANGLES, vertexArray, indexBuffer, shaderProgram);

    // next render session, whatever's in our member arrays will be overwritten
    iIndex = 0;
    vIndex = 0;
    vertexCount = 0;
  }

} /* namespace graphics */

