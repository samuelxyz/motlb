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
  : cbbShader("resources/shaders/ColoredBoxBatch.glsl"),
    cbb(200, cbbShader)
  {
  }

  Renderer::~Renderer()
  {
  }

  void Renderer::clearScreen()
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void Renderer::draw(GLenum mode, const VertexArray& va,
      const IndexBuffer& ib, const ShaderProgram& sp)
  {
    va.bind();
    ib.bind();
    sp.bind();
    glDrawElements(mode, ib.getNumIndices(), GL_UNSIGNED_INT, nullptr);
  }

  void Renderer::renderAndClearAll()
  {
    clearScreen();
    cbb.renderAll();
    cbb.clearAll();
  }

  void Renderer::addColoredBox(float r, float g, float b, float a, Box& box)
  {
    cbb.add({ r, g, b, a, box });
  }

} /* namespace render */
