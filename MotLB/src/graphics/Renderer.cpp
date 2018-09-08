/*
 * Renderer.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include <Values.h>
#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexArray.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace graphics
{

  Renderer::Renderer()
  : shaderProgram("resources/shaders/MotLB_generic.glsl"),
    tBatch(50000, shaderProgram)
  {
    glm::mat4 projMatrix = glm::ortho(
        0.0f, static_cast<float>(Values::WINDOW_WIDTH),
        0.0f, static_cast<float>(Values::WINDOW_HEIGHT),
       -1.0f, 1.0f);

    shaderProgram.bind();
    shaderProgram.setUniformMat4f("u_projMatrix", projMatrix); // @suppress("Invalid arguments")
  }

  Renderer::~Renderer()
  {
  }

  void Renderer::clearScreen()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    tBatch.renderAndClearAll();
  }

  void Renderer::addTriangle(const Values::Triangle& tri)
  {
    tBatch.addTriangle(tri);
  }

  void Renderer::addQuad(const Values::Quad& quad)
  {
    tBatch.addQuad(quad);
  }

  void Renderer::addCenteredPoly(const Values::CenteredPoly& cp)
  {
    tBatch.addCenteredPoly(cp);
  }

} /* namespace render */
