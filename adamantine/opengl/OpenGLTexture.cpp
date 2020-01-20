#include "glew.h"
#include "glut.h"
#include "opengl/OpenGLTexture.h"

OpenGLTexture::OpenGLTexture(const Texture* texture, GLenum unit) {
  this->unit = unit;

  bool hasAlpha = texture->getData()->format->BytesPerPixel == 4;
  int format = hasAlpha ? GL_RGBA : GL_RGB;
  auto* surface = texture->getData();

  glGenTextures(1, &id);

  use();

  glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

OpenGLTexture::~OpenGLTexture() {
  glDeleteTextures(1, &id);
}

void OpenGLTexture::use() {
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, id);
}