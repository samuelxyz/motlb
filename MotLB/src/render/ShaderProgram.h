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
#include <unordered_map>

namespace render
{

  class ShaderProgram
  {
    private:
      GLuint programID, vertexShaderID, fragmentShaderID;
      std::string vertexShaderSource, fragmentShaderSource;
      std::unordered_map<std::string, GLint> uniformCache;

    public:

      ShaderProgram(const std::string& filepath);
      virtual ~ShaderProgram();

      void bind() const;
      void setUniform4f(const std::string& uniformName, float, float, float, float);

      GLuint getID() const {return programID;}

    private:
      void parseShaders(const std::string& filepath);
      static GLuint compileShader(GLenum type, std::string& source);
      GLint getUniformLocation(const std::string& uniformName);
  };

} /* namespace render */

#endif /* SHADERPROGRAM_H_ */
