/*
 * ShaderProgram.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include "ShaderProgram.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>

namespace render
{

  ShaderProgram::ShaderProgram(const std::string& filepath)
  : programID(glCreateProgram())
  {
    parseShaders(filepath);
    vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    assert(vertexShaderID != 0 && fragmentShaderID != 0);

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    glValidateProgram(programID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
  }

  ShaderProgram::~ShaderProgram()
  {
    glDeleteProgram(programID);
  }

  void ShaderProgram::parseShaders(const std::string& filepath)
  {
    std::ifstream stream(filepath);
    std::stringstream streams[2];
    std::string line;

    enum class ShaderType {
        NONE = -1, VERTEX, FRAGMENT
    } type(ShaderType::NONE);

    while (getline(stream, line))
    {
      if (line.find("#shader") != std::string::npos)
      {
        if (line.find("vertex") != std::string::npos)
          type = ShaderType::VERTEX;
        else if (line.find("fragment") != std::string::npos)
          type = ShaderType::FRAGMENT;
      }
      else
      {
        if (type != ShaderType::NONE)
          streams[(int)type] << line << '\n';
      }
    }

    vertexShaderSource = streams[0].str();
    fragmentShaderSource = streams[1].str();

  }

  void ShaderProgram::bind()
  {
    glUseProgram(programID);
  }

  GLuint ShaderProgram::compileShader(GLenum type, std::string& source)
  {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // error handling
    int compileSuccess;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE)
    {
      int length;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

      char* msg = new char[length];
      glGetShaderInfoLog(shader, length, &length, msg);
      std::cerr << "Failed to compile " << ((type == GL_VERTEX_SHADER) ?
          "vertex" : "fragment") << " shader!" << std::endl;
      std::cerr << msg << std::endl;

      delete msg;

      glDeleteShader(shader);
      return 0;
    }

    return shader;
  }

  GLuint ShaderProgram::getUniformLocation(const std::string& uniformName)
  {
    auto cachedLoc = uniformCache.find(uniformName);

    GLuint location;

    if (cachedLoc == uniformCache.end())
    {
      location = glGetUniformLocation(programID, uniformName.c_str());
      uniformCache[uniformName] = location;
    }
    else
    {
      location = uniformCache[uniformName];
    }

    return location;
  }

  void ShaderProgram::setUniform4f(const std::string& uniformName, float float1,
      float float2, float float3, float float4)
  {

    glUniform4f(getUniformLocation(uniformName), float1, float2, float3, float4);
  }




} /* namespace render */

