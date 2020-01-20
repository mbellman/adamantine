#pragma once

#include "glut.h"
#include "subsystem/Texture.h"

class OpenGLTexture {
public:
  OpenGLTexture(const Texture* texture, GLenum unit);
  ~OpenGLTexture();

  void use();

private:
  GLuint id;
  GLenum unit;
};