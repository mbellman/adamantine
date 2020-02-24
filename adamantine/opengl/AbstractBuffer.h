#pragma once

#include "opengl/FrameBuffer.h"

class AbstractBuffer {
public:
  virtual ~AbstractBuffer();

  virtual void createFrameBuffer(unsigned int width, unsigned int height) = 0;
  FrameBuffer* getFrameBuffer();
  void startReading();
  void startWriting();

protected:
  FrameBuffer* frameBuffer = nullptr;

  virtual void createShaderPrograms() = 0;
};