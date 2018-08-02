/*
 * Renderer.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include "Renderer.h"

#include "../Values.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace graphics
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

  void Renderer::addColoredBox(const Values::Color& color, geometry::Box& box)
  {
    cbb.add( ColoredBoxBatch::ColoredBox(const_cast<Values::Color&>(color), box ) );
  }

} /* namespace render */
