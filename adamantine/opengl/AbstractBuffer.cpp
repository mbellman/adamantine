#include "opengl/AbstractBuffer.h"

AbstractBuffer::~AbstractBuffer() {
  delete frameBuffer;
}

FrameBuffer* AbstractBuffer::getFrameBuffer() {
  return frameBuffer;
}

void AbstractBuffer::startReading() {
  frameBuffer->startReading();
}

void AbstractBuffer::startWriting() {
  frameBuffer->startWriting();
}