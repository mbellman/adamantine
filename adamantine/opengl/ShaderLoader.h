#pragma once

#include "glut.h"

namespace ShaderLoader {
  GLuint load(GLenum shaderType, const char* path);
  GLuint loadFragmentShader(const char* path);
  GLuint loadGeometryShader(const char* path);
  GLuint loadVertexShader(const char* path);
};