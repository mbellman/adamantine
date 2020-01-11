#include "opengl/FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height) {
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  size.width = width;
  size.height = height;
}

FrameBuffer::~FrameBuffer() {
  
}

void FrameBuffer::addColorBuffer(unsigned int internalFormat, unsigned int format) {
  ColorBuffer buffer;

  buffer.internalFormat = internalFormat;
  buffer.format = format;
  buffer.attachment = GL_COLOR_ATTACHMENT0 + colorBuffers.size();

  glGenTextures(1, &buffer.texture);
  glBindTexture(GL_TEXTURE_2D, buffer.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, buffer.internalFormat, size.width, size.height, 0, buffer.format, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, buffer.attachment, GL_TEXTURE_2D, buffer.texture, 0);

  colorBuffers.push_back(buffer);
}

void FrameBuffer::addDepthBuffer() {
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.width, size.height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

void FrameBuffer::initializeColorBuffers() {
  GLuint* attachments = new GLuint[colorBuffers.size()];

  for (int i = 0; i < colorBuffers.size(); i++) {
    attachments[i] = colorBuffers[i].attachment;
  }

  glDrawBuffers(colorBuffers.size(), attachments);

  delete[] attachments;
}

void FrameBuffer::startReading() {
  for (int i = 0; i < colorBuffers.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[i].texture);
  }

  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void FrameBuffer::startWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void FrameBuffer::useDefaultFramebuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}