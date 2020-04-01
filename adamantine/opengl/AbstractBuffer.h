#pragma once

#include "opengl/FrameBuffer.h"
#include "opengl/OpenGLPipeline.h"

class AbstractBuffer {
public:
  virtual ~AbstractBuffer();

  virtual void createFrameBuffer(unsigned int width, unsigned int height) = 0;
  FrameBuffer* getFrameBuffer();
  void renderScreenQuad();
  void startReading();
  void startWriting();

protected:
  FrameBuffer* frameBuffer = nullptr;
  OpenGLPipeline* glScreenQuad = nullptr;

  virtual void createShaderPrograms() = 0;
};