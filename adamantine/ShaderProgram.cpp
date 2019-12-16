#include "glew.h"
#include "glut.h"
#include "ShaderProgram.h"
#include "ShaderLoader.h"

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(program);
}

void ShaderProgram::attachShader(GLuint shader) {
  glAttachShader(program, shader);
}

void ShaderProgram::create() {
  program = glCreateProgram();
}

GLint ShaderProgram::createUniformInput(const char* name) {
  return glGetUniformLocation(program, name);
}

void ShaderProgram::activate() {
  glLinkProgram(program);
  glUseProgram(program);
}

void ShaderProgram::setFragmentShaderOutput(const char* name) {
  glBindFragDataLocation(program, 0, name);
}