/*
 * ShaderProgram.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <string>
#include <GL/glew.h>

namespace render
{

  class ShaderProgram
  {
    private:
      GLuint programID, vertexShaderID, fragmentShaderID;
      std::string vertexShaderSource, fragmentShaderSource;

    public:

      ShaderProgram(const std::string& filepath);
      virtual ~ShaderProgram();

      void bind();

      void setUniform4f(const std::string& uniformName, float, float, float, float);

    private:
      void parseShaders(const std::string& filepath);
      GLuint compileShader(GLenum type, std::string& source);
  };

} /* namespace render */

#endif /* SHADERPROGRAM_H_ */
