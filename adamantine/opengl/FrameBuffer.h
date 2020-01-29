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
  void addColorDepthBuffer();
  void addDepthBuffer();
  void initializeColorBuffers();
  void startReading();
  void startWriting();

private:
  GLuint fbo;
  std::vector<ColorBuffer> colorBuffers;
  GLuint depthBuffer;
  Region2d<int> size;
};