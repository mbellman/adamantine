#include "opengl/GBuffer.h"

void GBuffer::attach(Attachment attachment) {
  useDefaultFrameBuffer();
  startReading();
  glReadBuffer(attachment);
}

void GBuffer::bindTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, colorTexture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, normalDepthTexture);
}

void GBuffer::initialize(int width, int height) {
  glGenFramebuffers(1, &buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, buffer);

  // Create color buffer
  glGenTextures(1, &colorTexture);
  glBindTexture(GL_TEXTURE_2D, colorTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment::COLOR, GL_TEXTURE_2D, colorTexture, 0);

  // Create normal + depth buffer
  glGenTextures(1, &normalDepthTexture);
  glBindTexture(GL_TEXTURE_2D, normalDepthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment::NORMAL_DEPTH, GL_TEXTURE_2D, normalDepthTexture, 0);

  // Create depth buffer
  glGenRenderbuffers(1, &depthRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, Attachment::DEPTH, GL_RENDERBUFFER, depthRBO);

  GLenum buffers[] = { Attachment::COLOR, Attachment::NORMAL_DEPTH };
  glDrawBuffers(2, buffers);

  useDefaultFrameBuffer();
}

void GBuffer::startReading() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer);
}

void GBuffer::startWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer);
}

void GBuffer::useDefaultFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}