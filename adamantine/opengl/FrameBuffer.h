#pragma once

#include <vector>

#include "glew.h"
#include "glut.h"
#include "subsystem/Math.h"

struct ColorBuffer {
  unsigned int internalFormat;
  unsigned int format;
  GLuint texture;
  GLuint attachment;
};

class FrameBuffer {
public:
  FrameBuffer(int width, int height);
  ~FrameBuffer();

  void addColorBuffer(unsigned int internalFormat, unsigned int format);
  void addDepthBuffer();
  void initializeColorBuffers();
  void startReading();
  void startWriting();
  void useDefaultFramebuffer();

private:
  GLuint fbo;
  std::vector<ColorBuffer> colorBuffers;
  GLuint depthBuffer;
  Region2d<int> size;
};