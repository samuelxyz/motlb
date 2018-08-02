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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace graphics
{

  Renderer::Renderer()
  : cbbShader("resources/shaders/MotLB_generic.glsl"),
    cbb(200, cbbShader)
  {
    glm::mat4 projMatrix = glm::ortho(
        0.0f, static_cast<float>(Values::BATTLE_WIDTH),
        0.0f, static_cast<float>(Values::BATTLE_HEIGHT),
       -1.0f, 1.0f);

    cbbShader.bind();
    cbbShader.setUniformMat4f("u_projMatrix", projMatrix);
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

  void Renderer::addColoredBox(const Values::Color& color, const geometry::Box& box)
  {
    cbb.add(ColoredBoxBatch::ColoredBox(color, box));
  }

} /* namespace render */
