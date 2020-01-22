#include <string>

#include "glew.h"
#include "glut.h"
#include "opengl/ShaderProgram.h"
#include "opengl/ShaderLoader.h"

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(program);
}

void ShaderProgram::attachShader(GLuint shader) {
  glAttachShader(program, shader);
}

void ShaderProgram::bindVertexInputs() {
  for (auto& attribute : vertexAttributes) {
    setVertexAttribute(attribute);
  }
}

void ShaderProgram::create() {
  program = glCreateProgram();
}

GLint ShaderProgram::getUniformLocation(const char* name) const {
  return glGetUniformLocation(program, name);
}

GLint ShaderProgram::getUniformLocation(std::string name) const {
  return glGetUniformLocation(program, name.c_str());
}

void ShaderProgram::link() {
  glLinkProgram(program);
}

void ShaderProgram::setFragmentShaderOutput(const char* name) {
  glBindFragDataLocation(program, 0, name);
}

void ShaderProgram::setVertexAttribute(const VertexAttribute& attribute) {
  GLint location = glGetAttribLocation(program, attribute.name);

  glEnableVertexAttribArray(location);
  glVertexAttribPointer(location, attribute.size, attribute.type, GL_FALSE, attribute.stride, (void*)attribute.offset);
}

void ShaderProgram::use() const {
  glUseProgram(program);
}