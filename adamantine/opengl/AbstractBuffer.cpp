#include "opengl/AbstractBuffer.h"

AbstractBuffer::~AbstractBuffer() {
  delete frameBuffer;
  delete glScreenQuad;
}

FrameBuffer* AbstractBuffer::getFrameBuffer() {
  return frameBuffer;
}

void AbstractBuffer::renderScreenQuad() {
  glScreenQuad->render();
}

void AbstractBuffer::startReading() {
  frameBuffer->startReading();
}

void AbstractBuffer::startWriting() {
  frameBuffer->startWriting();
}