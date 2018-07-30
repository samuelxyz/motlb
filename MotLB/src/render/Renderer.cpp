/*
 * Renderer.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include "Renderer.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

namespace render
{

  Renderer::Renderer()
  {
  }

  Renderer::~Renderer()
  {
  }

  void Renderer::clear()
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void Renderer::draw(const VertexArray& va, const IndexBuffer& ib,
      const ShaderProgram& sp)
  {
    va.bind();
    ib.bind();
    sp.bind();
    glDrawElements(GL_TRIANGLES, ib.getNumIndices(), GL_UNSIGNED_INT, nullptr);
  }

} /* namespace render */
