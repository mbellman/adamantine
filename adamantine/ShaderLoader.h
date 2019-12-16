#pragma once

#include "glut.h"

struct ShaderLoader {
  static GLuint load(GLenum shaderType, const char* path);
  static GLuint loadFragmentShader(const char* path);
  static GLuint loadVertexShader(const char* path);
};