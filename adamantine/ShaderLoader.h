#pragma once

#include "SDL.h"
#include "glut.h"

struct ShaderLoader {
  GLuint load(GLenum shaderType, const char* path);
};