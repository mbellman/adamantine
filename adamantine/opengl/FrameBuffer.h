#pragma once

#include <vector>

#include "glew.h"
#include "glut.h"
#include "subsystem/Math.h"

struct ColorTexture {
  unsigned int internalFormat;
  unsigned int format;
  GLuint id;
  GLuint attachment;
};

class FrameBuffer {
public:
  FrameBuffer(int width, int height);
  ~FrameBuffer();

  void addColorTexture(GLint internalFormat, GLenum format);
  void addColorTexture(GLint internalFormat, GLenum format, GLint clamp);
  void addDepthStencilBuffer();
  void bindColorTexture(GLenum attachment);
  void bindColorTextures();
  void clearColorTexture(GLint attachment);
  void shareDepthStencilBuffer(FrameBuffer* target);
  void startReading();
  void startWriting();
  void transferColorTexture(GLenum source, const GLenum* targets, int size);

private:
  GLuint fbo;
  GLuint depthStencilBuffer;
  std::vector<ColorTexture> colorTextures;
  Region2d<int> size;
};